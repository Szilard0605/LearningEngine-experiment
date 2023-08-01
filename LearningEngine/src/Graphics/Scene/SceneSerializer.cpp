
#include "SceneSerializer.h"

#include "json.hpp"
#include <iostream>

#include "Entity.h"
#include "Components.h"

#include <fstream>

#include <Log/Log.h>

#include "gtc/type_ptr.hpp"

#include "Utils/JSONHelper.h"

using json = nlohmann::json;
static json s_JSON;

void SceneSerializer::Serialize(Scene* scene)
{

    s_JSON = nullptr;

    scene->Registry.each([scene](auto entityID)
    {
        TagComponent& tag = scene->Registry.get<TagComponent>(entityID);
        const char* tagstr = tag.Tag.c_str();

        s_JSON[tagstr] = json::object();

        TransformComponent* transformComponent = scene->Registry.try_get<TransformComponent>(entityID);
        if (transformComponent)
        {
            //Save transform component
            TransformComponent& tc = *transformComponent;

            s_JSON[tagstr][tc.ID]["Position"] = { tc.Position.x, tc.Position.y, tc.Position.z };
            s_JSON[tagstr][tc.ID]["Rotation"] = { tc.Rotation.x, tc.Rotation.y, tc.Rotation.z };
            s_JSON[tagstr][tc.ID]["Size"] =     { tc.Size.x,     tc.Size.y,     tc.Size.z     };
        }

        QuadRendererComponent* quadRendererComponent = scene->Registry.try_get<QuadRendererComponent>(entityID);

        if (quadRendererComponent)
        {
            QuadRendererComponent& qrc = *quadRendererComponent;

            s_JSON[tagstr][qrc.ID]["Scale"] = { qrc.Scale.x, qrc.Scale.y, qrc.Scale.z };
            s_JSON[tagstr][qrc.ID]["Color"] = { qrc.Color.r, qrc.Color.g, qrc.Color.b, qrc.Color.a };
        }


        PerspectiveCameraComponent perspectiveCamComponent = scene->Registry.try_get<PerspectiveCameraComponent>(entityID);
        if (perspectiveCamComponent)
        {
            PerspectiveCameraComponent& pcc = *perspectiveCamComponent;

            s_JSON[tagstr][pcc.ID]["MainCamera"] = pcc.MainCamera;

            s_JSON[tagstr][pcc.ID]["FocalPoint"] = { pcc.FocalPoint.x, pcc.FocalPoint.y, pcc.FocalPoint.z };

            s_JSON[tagstr][pcc.ID]["Distance"] = pcc.Distance;
            s_JSON[tagstr][pcc.ID]["Pitch"] = pcc.Pitch;
            s_JSON[tagstr][pcc.ID]["Yaw"] = pcc.Yaw;
            s_JSON[tagstr][pcc.ID]["FOV"] = pcc.FOV;
            s_JSON[tagstr][pcc.ID]["AspectRatio"] = pcc.AspectRatio;
            s_JSON[tagstr][pcc.ID]["FixedAspectRatio"] = pcc.AspectRatio;
            s_JSON[tagstr][pcc.ID]["NearClip"] = pcc.NearClip;
            s_JSON[tagstr][pcc.ID]["FarClip"] = pcc.FarClip;
        }
    });

    std::string path = "res/scenes/" + scene->GetName() + ".lescene";
    std::ofstream SceneFile(path.c_str());
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

    // Iterate through the first-level keys
    for (json::iterator it = s_JSON.begin(); it != s_JSON.end(); ++it)
    {
        std::string key = it.key();
        json head = it.value();
        
        Entity entity = scene->NewEntity(key);

        TagComponent& tag = entity.GetComponent<TagComponent>();
        tag.Tag = key;

        if (head.contains("TransformComponent"))
        {
            TransformComponent tc;

            for (int i = 0; i < 3; i++)
            {
                tc.Position[i] = (float)head[tc.ID]["Position"][i];
                tc.Rotation[i] = (float)head[tc.ID]["Rotation"][i];
                tc.Size[i]     = (float)head[tc.ID]["Size"][i];
            }
            
            entity.AddOrReplaceComponent<TransformComponent>(tc);
        }

        if (head.contains("QuadRendererComponent"))
        {
            QuadRendererComponent qrc;

            for (int i = 0; i < 3; i++)
                qrc.Scale[i] = (float)head[qrc.ID]["Scale"][i];

            for (int i = 0; i < 4; i++)
                qrc.Color[i] = (float)head[qrc.ID]["Color"][i];
            
            entity.AddOrReplaceComponent<QuadRendererComponent>(qrc);
        }

        if (head.contains("PerspectiveCameraComponent"))
        {
            PerspectiveCameraComponent pcc;

            pcc.MainCamera = head[pcc.ID]["MainCamera"];

            for (int i = 0; i < 3; i++)
                pcc.FocalPoint[i] = (float)head[pcc.ID]["FocalPoint"][i];

            pcc.Distance         =  (float)head[pcc.ID]["Distance"];
            pcc.Pitch            =  (float)head[pcc.ID]["Pitch"];
            pcc.Yaw              =  (float)head[pcc.ID]["Yaw"];
            pcc.FOV              =  (float)head[pcc.ID]["FOV"];
            pcc.AspectRatio      =  (float)head[pcc.ID]["AspectRatio"];
            pcc.FixedAspectRatio =  (float)head[pcc.ID]["FixedAspectRatio"];
            pcc.NearClip         =  (float)head[pcc.ID]["NearClip"];
            pcc.FarClip          =  (float)head[pcc.ID]["FarClip"];

            pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);

            pcc.Camera->SetPitch(pcc.Pitch);
            pcc.Camera->SetYaw(pcc.Yaw);

            entity.AddOrReplaceComponent<PerspectiveCameraComponent>(pcc);
        }
    }

    return scene;
}
