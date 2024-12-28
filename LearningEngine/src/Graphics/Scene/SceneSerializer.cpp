
#include "SceneSerializer.h"
#include "Log/Log.h"
#include "Entity.h"
#include "Components.h"
#include "Utils/JSONHelper.h"

#include "gtc/type_ptr.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;
static json s_JSON;

void SceneSerializer::Serialize(Scene* scene, std::string filepath)
{
    s_JSON = nullptr;

    AmbientLight& ambientLight = scene->GetAmbientLight();
    s_JSON["AmbientLight"]["Color"] = { ambientLight.Color.x, ambientLight.Color.y, ambientLight.Color.z };
    s_JSON["AmbientLight"]["Intensity"] = ambientLight.Intensity;

    scene->Registry.each([scene](auto entityID)
    {
        TagComponent& tag = scene->Registry.get<TagComponent>(entityID);
        auto enttID = std::to_string(tag.ID);
        auto entityName = tag.Tag;

        s_JSON[enttID] = json::object();
        s_JSON[enttID]["Name"] = entityName;
        s_JSON[enttID]["ID"] = tag.ID;

        HierarchyComponent* hc = scene->Registry.try_get<HierarchyComponent>(entityID);
        if (hc)
        {
            LE_CORE_INFO("%d parent: %d", entityID, hc->Parent);
            s_JSON[enttID][hc->ID]["Parent"] = (int)hc->Parent;
        }

        TransformComponent* tc = scene->Registry.try_get<TransformComponent>(entityID);
        if (tc)
        {
            //Save transform component
            s_JSON[enttID][tc->ID]["Position"] = {tc->Transform.Position.x, tc->Transform.Position.y, tc->Transform.Position.z};
            s_JSON[enttID][tc->ID]["Rotation"] = {tc->Transform.Rotation.x, tc->Transform.Rotation.y, tc->Transform.Rotation.z};
            s_JSON[enttID][tc->ID]["Scale"] =    {tc->Transform.Scale.x,    tc->Transform.Scale.y,    tc->Transform.Scale.z   };
        }

        QuadRendererComponent* qrc = scene->Registry.try_get<QuadRendererComponent>(entityID);
        if (qrc)
        {
            s_JSON[enttID][qrc->ID]["Color"] = { qrc->Color.r, qrc->Color.g, qrc->Color.b, qrc->Color.a };
        }


        PerspectiveCameraComponent* pcc = scene->Registry.try_get<PerspectiveCameraComponent>(entityID);
        if (pcc)
        {
            s_JSON[enttID][pcc->ID]["MainCamera"] = pcc->MainCamera;

            s_JSON[enttID][pcc->ID]["FocalPoint"] = { pcc->FocalPoint.x, pcc->FocalPoint.y, pcc->FocalPoint.z };

            //s_JSON[enttID][pcc->ID]["FocalPoint"] = glm::value_ptr(pcc->FocalPoint);

            s_JSON[enttID][pcc->ID]["Distance"] = pcc->Distance;
            s_JSON[enttID][pcc->ID]["Pitch"] = pcc->Pitch;
            s_JSON[enttID][pcc->ID]["Yaw"] = pcc->Yaw;
            s_JSON[enttID][pcc->ID]["FOV"] = pcc->FOV;
            s_JSON[enttID][pcc->ID]["AspectRatio"] = pcc->AspectRatio;
            s_JSON[enttID][pcc->ID]["FixedAspectRatio"] = pcc->FixedAspectRatio;
            s_JSON[enttID][pcc->ID]["NearClip"] = pcc->NearClip;
            s_JSON[enttID][pcc->ID]["FarClip"] = pcc->FarClip;
        }

        StaticModelComponent* smc = scene->Registry.try_get<StaticModelComponent>(entityID);
        if (smc)
        {
            s_JSON[enttID][smc->ID]["SourcePath"] = smc->StaticModel->GetSourceFilePath().string();
        }

        PointLightComponent* plc = scene->Registry.try_get<PointLightComponent>(entityID);
        if(plc)
        {
            s_JSON[enttID][plc->ID]["Color"] = { plc->Color.x, plc->Color.y, plc->Color.z };
            s_JSON[enttID][plc->ID]["Intensity"] = plc->Intensity;
            s_JSON[enttID][plc->ID]["SpecularPower"] = plc->SpecularPower;
        }

        DirectionalLightComponent* dlc = scene->Registry.try_get<DirectionalLightComponent>(entityID);
        if (dlc)
        {
            s_JSON[enttID][dlc->ID]["Color"] = { dlc->Color.x, dlc->Color.y, dlc->Color.z };
            s_JSON[enttID][dlc->ID]["Intensity"] = dlc->Intensity;
            s_JSON[enttID][dlc->ID]["SpecularPower"] = dlc->SpecularPower;
        }

        RigidbodyComponent* rc = scene->Registry.try_get<RigidbodyComponent>(entityID);
        if (rc)
        {
            s_JSON[enttID][rc->ID]["Mass"] = rc->Mass;
            s_JSON[enttID][rc->ID]["LinearDamping"] = rc->LinearDamping;
            s_JSON[enttID][rc->ID]["AngularDamping"] = rc->AngularDamping;
        }

        BoxColliderComponent* bcc = scene->Registry.try_get<BoxColliderComponent>(entityID);
        if (bcc)
        {
            s_JSON[enttID][bcc->ID]["Size"] = { bcc->Size.x, bcc->Size.y, bcc->Size.z };
        }

        SphereColliderComponent* scc = scene->Registry.try_get<SphereColliderComponent>(entityID);
        if (scc)
        {
            s_JSON[enttID][scc->ID]["Radius"] = scc->Radius;
        }

        LuaScriptComponent* lsc = scene->Registry.try_get<LuaScriptComponent>(entityID);
        if (lsc)
        {
            s_JSON[enttID][lsc->ID]["SourcePath"] = lsc->SourcePath;
        }
    });

    std::ofstream SceneFile(filepath.c_str());
    SceneFile << std::setw(4) << s_JSON << '\n';
    SceneFile.close();
}

Scene* SceneSerializer::Load(const std::filesystem::path path)
{
    s_JSON = nullptr;

    std::ifstream SceneFile(path.c_str());
    s_JSON = json::parse(SceneFile);
    SceneFile.close();

    Scene* scene = new Scene(path.filename().replace_extension().string());

    LE_CORE_INFO("Loading scene: %s", path.filename().replace_extension().string().c_str());

    // Loading ambient light
    if (s_JSON.contains("AmbientLight"))
    {
        AmbientLight ambientLight;
        
        const auto& alData = JSONHelper::GetObject(s_JSON, "AmbientLight");

        ambientLight.Color = JSONHelper::ReadVec3(alData, "Color");
        ambientLight.Intensity = JSONHelper::ReadAs<float>(alData, "Intensity");

        scene->SetAmbientLight(ambientLight);
    }

    for (const auto& it : s_JSON.items()) 
    {
        Entity entity;

        json entry = it.value();

        if (entry.contains("Name") && entry.contains("ID"))
        {
            entity = scene->NewEntity(entry["Name"]);
            TagComponent tag = entity.GetComponent<TagComponent>();
            tag.Tag = JSONHelper::ReadAs<std::string>(entry, "Name");
            tag.ID  = JSONHelper::ReadAs<int>(entry, "ID");
        }

        if (entry.contains("HierarchyComponent"))
        {
            HierarchyComponent hc;
            hc.Parent = (entt::entity)entry[hc.ID]["Parent"];
            entity.AddOrReplaceComponent<HierarchyComponent>(hc);
        }

        if (entry.contains("TransformComponent"))
        {
			TransformComponent tc;

            const auto& tcData = JSONHelper::GetObject(entry, "TransformComponent");
            tc.Transform.Position = JSONHelper::ReadVec3(tcData, "Position");
            tc.Transform.Rotation = JSONHelper::ReadVec3(tcData, "Rotation");
            tc.Transform.Scale = JSONHelper::ReadVec3(tcData, "Scale");

			entity.AddOrReplaceComponent<TransformComponent>(tc);
		}

        if (entry.contains("QuadRendererComponent")) 
        {
            QuadRendererComponent qrc;           

            const auto& tcData = JSONHelper::GetObject(entry, "QuadRendererComponent");
            qrc.Color = JSONHelper::ReadVec4(tcData, "Color");
      
            entity.AddOrReplaceComponent<QuadRendererComponent>(qrc);
        }

        if (entry.contains("PerspectiveCameraComponent"))
        {
            PerspectiveCameraComponent pcc;
            const auto& pccData = JSONHelper::GetObject(entry, "PerspectiveCameraComponent");

            pcc.FocalPoint = JSONHelper::ReadVec3(pccData, "FocalPoint");
            pcc.MainCamera = JSONHelper::ReadAs<bool>(pccData, "MainCamera");
            pcc.Pitch = JSONHelper::ReadAs<float>(pccData, "Pitch");
            pcc.Yaw = JSONHelper::ReadAs<float>(pccData, "Yaw");
            pcc.FOV = JSONHelper::ReadAs<float>(pccData, "FOV");
            pcc.AspectRatio = JSONHelper::ReadAs<float>(pccData, "AspectRatio");
            pcc.FixedAspectRatio = JSONHelper::ReadAs<bool>(pccData,  "FixedAspectRatio");
            pcc.NearClip = JSONHelper::ReadAs<float>(pccData, "NearClip");
            pcc.FarClip = JSONHelper::ReadAs<float>(pccData, "FarClip");
           
            pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);

            pcc.Camera->SetPitch(pcc.Pitch);
            pcc.Camera->SetYaw(pcc.Yaw);

            entity.AddOrReplaceComponent<PerspectiveCameraComponent>(pcc);
        }

        if (entry.contains("StaticModelComponent"))
        {
            StaticModelComponent smc;

            //std::string path = entry[smc.ID]["SourcePath"];
            const auto& smcData = JSONHelper::GetObject(entry, "StaticModelComponent");
            std::string path = JSONHelper::ReadAs<std::string>(smcData, "SourcePath");

            smc.StaticModel = new Model(path);
            entity.AddOrReplaceComponent<StaticModelComponent>(smc);
        }

        if (entry.contains("PointLightComponent"))
        {
            PointLightComponent plc;

            const auto& plcData = JSONHelper::GetObject(entry, "PointLightComponent");
            plc.Color = JSONHelper::ReadVec3(plcData, "Color");
            plc.Intensity = JSONHelper::ReadAs<float>(plcData, "Intensity");
            plc.SpecularPower = JSONHelper::ReadAs<float>(plcData, "SpecularPower");

            entity.AddOrReplaceComponent<PointLightComponent>(plc);
        }

        if (entry.contains("DirectionalLightComponent"))
        {
            DirectionalLightComponent dlc;

            const auto& dlcData = JSONHelper::GetObject(entry, "DirectionalLightComponent");

            dlc.Color = JSONHelper::ReadVec3(dlcData, "Color");
            dlc.Intensity = JSONHelper::ReadAs<float>(dlcData, "Intensity");
            dlc.SpecularPower = JSONHelper::ReadAs<float>(dlcData, "SpecularPower");

            entity.AddOrReplaceComponent<DirectionalLightComponent>(dlc);
        }

        if (entry.contains("RigidbodyComponent"))
        {
            RigidbodyComponent rc;

            TransformComponent& tc = scene->Registry.get<TransformComponent>(entity.GetHandle());
            //rc.Rigidbody = Rigidbody::Create(tc.Transform);

            const auto& rcData = JSONHelper::GetObject(entry, "RigidbodyComponent");

            rc.Mass = JSONHelper::ReadAs<float>(rcData, "Mass");
            rc.LinearDamping = JSONHelper::ReadAs<float>(rcData, "LinearDamping");
            rc.AngularDamping = JSONHelper::ReadAs<float>(rcData, "AngularDamping");

            entity.AddOrReplaceComponent<RigidbodyComponent>(rc);
        }

        if (entry.contains("BoxColliderComponent"))
        {
            BoxColliderComponent bcc;
            const auto& bccData = JSONHelper::GetObject(entry, "BoxColliderComponent");
            bcc.Size = JSONHelper::ReadVec3(bccData, "Size");

            entity.AddOrReplaceComponent<BoxColliderComponent>(bcc);
        }

        if (entry.contains("SphereColliderComponent"))
        {
            SphereColliderComponent scc;
            const auto& sccData = JSONHelper::GetObject(entry, "SphereColliderComponent");
            scc.Radius = JSONHelper::ReadAs<float>(sccData, "Radius");

            entity.AddOrReplaceComponent<SphereColliderComponent>(scc);
        }

        if (entry.contains("LuaScriptComponent"))
        {
            LuaScriptComponent lsc;
            const auto& lscData = JSONHelper::GetObject(entry, "LuaScriptComponent");
            lsc.SourcePath = JSONHelper::ReadAs<std::string>(lscData, "SourcePath");
            entity.AddOrReplaceComponent<LuaScriptComponent>(lsc);
        }
    }

    // Completing entity hieararchy

    auto view = scene->Registry.view<HierarchyComponent>();

    for (auto it = view.begin(); it < view.end(); it++)
    {
        Entity entity(view[it.index()], scene);
        HierarchyComponent& hc = entity.GetComponent<HierarchyComponent>();
        Entity parent(hc.Parent, scene);
        if (parent.IsValid())
        {
            parent.AddChildren(entity);
        }
    }

    return scene;
}
