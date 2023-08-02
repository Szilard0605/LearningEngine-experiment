
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

    scene->Registry.each([scene](auto entityID)
    {
        TagComponent& tag = scene->Registry.get<TagComponent>(entityID);
        auto enttID = std::to_string(tag.ID);
        auto entityName = tag.Tag;

        s_JSON[enttID] = json::object();
        s_JSON[enttID]["Name"] = entityName;
        s_JSON[enttID]["ID"] = tag.ID;

        TransformComponent* transformComponent = scene->Registry.try_get<TransformComponent>(entityID);
        if (transformComponent)
        {
            //Save transform component
            TransformComponent& tc = *transformComponent;

            s_JSON[enttID][tc.ID]["Position"] = {tc.Position.x, tc.Position.y, tc.Position.z};
            s_JSON[enttID][tc.ID]["Rotation"] = {tc.Rotation.x, tc.Rotation.y, tc.Rotation.z};
            s_JSON[enttID][tc.ID]["Size"] = {tc.Size.x,     tc.Size.y,     tc.Size.z};
        }

        QuadRendererComponent* quadRendererComponent = scene->Registry.try_get<QuadRendererComponent>(entityID);
        if (quadRendererComponent)
        {
            QuadRendererComponent& qrc = *quadRendererComponent;

            s_JSON[enttID][qrc.ID]["Scale"] = { qrc.Scale.x, qrc.Scale.y, qrc.Scale.z };
            s_JSON[enttID][qrc.ID]["Color"] = { qrc.Color.r, qrc.Color.g, qrc.Color.b, qrc.Color.a };
        }


        PerspectiveCameraComponent* perspectiveCamComponent = scene->Registry.try_get<PerspectiveCameraComponent>(entityID);
        if (perspectiveCamComponent)
        {
            PerspectiveCameraComponent& pcc = *perspectiveCamComponent;

            s_JSON[enttID][pcc.ID]["MainCamera"] = pcc.MainCamera;

            s_JSON[enttID][pcc.ID]["FocalPoint"] = { pcc.FocalPoint.x, pcc.FocalPoint.y, pcc.FocalPoint.z };

            s_JSON[enttID][pcc.ID]["Distance"] = pcc.Distance;
            s_JSON[enttID][pcc.ID]["Pitch"] = pcc.Pitch;
            s_JSON[enttID][pcc.ID]["Yaw"] = pcc.Yaw;
            s_JSON[enttID][pcc.ID]["FOV"] = pcc.FOV;
            s_JSON[enttID][pcc.ID]["AspectRatio"] = pcc.AspectRatio;
            s_JSON[enttID][pcc.ID]["FixedAspectRatio"] = pcc.AspectRatio;
            s_JSON[enttID][pcc.ID]["NearClip"] = pcc.NearClip;
            s_JSON[enttID][pcc.ID]["FarClip"] = pcc.FarClip;
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

    for (const auto& entry : s_JSON.items()) {
        auto entity = scene->NewEntity(entry.value()["Name"]);

        TagComponent& tag = entity.GetComponent<TagComponent>();
        tag.Tag = entry.value()["Name"];
        tag.ID = entry.value()["ID"];

        if (entry.value().contains("TransformComponent"))
        {
			TransformComponent tc;

            for (int i = 0; i < 3; i++)
            {
				tc.Position[i] = (float)entry.value()[tc.ID]["Position"][i];
				tc.Rotation[i] = (float)entry.value()[tc.ID]["Rotation"][i];
				tc.Size[i]     = (float)entry.value()[tc.ID]["Size"][i];
			}
			
			entity.AddOrReplaceComponent<TransformComponent>(tc);
		}

        if (entry.value().contains("QuadRendererComponent")) 
        {
            QuadRendererComponent qrc;

            for (int i = 0; i < 3; i++)
                qrc.Scale[i] = (float)entry.value()[qrc.ID]["Scale"][i];

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
    }

    return scene;
}
