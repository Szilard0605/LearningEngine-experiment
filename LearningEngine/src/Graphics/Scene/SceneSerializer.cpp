
#include "SceneSerializer.h"

#include "json.hpp"
#include <iostream>

#include "Entity.h"
#include "Components.h"

#include <fstream>

#include <Log/Log.h>

#include "gtc/type_ptr.hpp"

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
            s_JSON[enttID][pcc->ID]["FixedAspectRatio"] = pcc->AspectRatio;
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
        }

        DirectionalLightComponent* dlc = scene->Registry.try_get<DirectionalLightComponent>(entityID);
        if (dlc)
        {
            s_JSON[enttID][dlc->ID]["Color"] = { dlc->Color.x, dlc->Color.y, dlc->Color.z };
            s_JSON[enttID][dlc->ID]["Intensity"] = dlc->Intensity;
            s_JSON[enttID][dlc->ID]["Direction"] = { dlc->Direction.x,  dlc->Direction.y, dlc->Direction.z };
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

    LE_CORE_INFO(std::string("Loading scene: ") + path.filename().replace_extension().string());

    // Loading ambient light
    if (s_JSON.contains("AmbientLight"))
    {
        AmbientLight ambientLight;
        
        for (int i = 0; i < 3; i++)
            ambientLight.Color[i] = s_JSON.at("AmbientLight")["Color"][i];

        ambientLight.Intensity = s_JSON.at("AmbientLight")["Intensity"];

        scene->SetAmbientLight(ambientLight);
    }

    for (const auto& entry : s_JSON.items()) 
    {
     
        Entity entity;

        if (entry.value().contains("Name") && entry.value().contains("ID"))
        {
            entity = scene->NewEntity(entry.value()["Name"]);
            TagComponent tag = entity.GetComponent<TagComponent>();
            tag.Tag = entry.value()["Name"];
            tag.ID = entry.value()["ID"];
        }

        if (entry.value().contains("TransformComponent"))
        {
			TransformComponent tc;

            for (int i = 0; i < 3; i++)
            {
				tc.Transform.Position[i] = (float)entry.value()[tc.ID]["Position"][i];
				tc.Transform.Rotation[i] = (float)entry.value()[tc.ID]["Rotation"][i];
				tc.Transform.Scale[i]     = (float)entry.value()[tc.ID]["Scale"][i];
			}
			
			entity.AddOrReplaceComponent<TransformComponent>(tc);
		}

        if (entry.value().contains("QuadRendererComponent")) 
        {
            QuadRendererComponent qrc;           

            for (int i = 0; i < 4; i++)
                qrc.Color[i] = (float)entry.value()[qrc.ID]["Color"][i];

            entity.AddOrReplaceComponent<QuadRendererComponent>(qrc);
        }

        if (entry.value().contains("PerspectiveCameraComponent"))
        {
            PerspectiveCameraComponent pcc;

            pcc.MainCamera = entry.value()[pcc.ID]["MainCamera"];

            for (int i = 0; i < 3; i++)
                pcc.FocalPoint[i] = entry.value()[pcc.ID]["FocalPoint"][i];

            pcc.Distance = entry.value()[pcc.ID]["Distance"];
            pcc.Pitch = entry.value()[pcc.ID]["Pitch"];
            pcc.Yaw = entry.value()[pcc.ID]["Yaw"];
            pcc.FOV = entry.value()[pcc.ID]["FOV"];
            pcc.AspectRatio = entry.value()[pcc.ID]["AspectRatio"];
            pcc.FixedAspectRatio = (float)entry.value()[pcc.ID]["FixedAspectRatio"];
            pcc.NearClip = entry.value()[pcc.ID]["NearClip"];
            pcc.FarClip = entry.value()[pcc.ID]["FarClip"];
            pcc.MainCamera = entry.value()[pcc.ID]["MainCamera"];

            pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);

            pcc.Camera->SetPitch(pcc.Pitch);
            pcc.Camera->SetYaw(pcc.Yaw);

            entity.AddOrReplaceComponent<PerspectiveCameraComponent>(pcc);
        }

        if (entry.value().contains("StaticModelComponent"))
        {
            StaticModelComponent smc;

            std::string path = entry.value()[smc.ID]["SourcePath"];
            smc.StaticModel = new Model(path);
        
            entity.AddOrReplaceComponent<StaticModelComponent>(smc);
        }

        if (entry.value().contains("PointLightComponent"))
        {
            PointLightComponent plc;

            for (int i = 0; i < 3; i++)
                plc.Color[i] = entry.value()[plc.ID]["Color"][i];

            plc.Intensity = entry.value()[plc.ID]["Intensity"];

            entity.AddOrReplaceComponent<PointLightComponent>(plc);
        }

        if (entry.value().contains("DirectionalLightComponent"))
        {
            DirectionalLightComponent dlc;

            for (int i = 0; i < 3; i++)
            {
                dlc.Color[i] = entry.value()[dlc.ID]["Color"][i];
                dlc.Direction[i] = entry.value()[dlc.ID]["Direction"][i];
            }
            dlc.Intensity = entry.value()[dlc.ID]["Intensity"];
        
            entity.AddOrReplaceComponent<DirectionalLightComponent>(dlc);
        }

        if (entry.value().contains("RigidbodyComponent"))
        {
            RigidbodyComponent rc;

            TransformComponent& tc = scene->Registry.get<TransformComponent>(entity.GetHandle());
            //rc.Rigidbody = Rigidbody::Create(tc.Transform);

            rc.Mass = entry.value()[rc.ID]["Mass"];
            rc.LinearDamping = entry.value()[rc.ID]["LinearDamping"];
            rc.AngularDamping = entry.value()[rc.ID]["AngularDamping"];

            entity.AddOrReplaceComponent<RigidbodyComponent>(rc);
        }

        if (entry.value().contains("BoxColliderComponent"))
        {
            BoxColliderComponent bcc;

            for (int i = 0; i < 3; i++)
            {
                bcc.Size[i] = entry.value()[bcc.ID]["Size"][i];
            }

            entity.AddOrReplaceComponent<BoxColliderComponent>(bcc);
        }

        if (entry.value().contains("SphereColliderComponent"))
        {
            SphereColliderComponent scc;
            scc.Radius = entry.value()[scc.ID]["Radius"];
            entity.AddOrReplaceComponent<SphereColliderComponent>(scc);
        }
    }

    return scene;
}
