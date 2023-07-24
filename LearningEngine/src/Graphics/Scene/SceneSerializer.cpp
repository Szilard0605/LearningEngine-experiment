
#include "SceneSerializer.h"

#include "json.hpp"
#include <iostream>

#include "Entity.h"
#include "Components.h"

#include <fstream>

#include <Log/Log.h>

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

        if (scene->Registry.try_get<TransformComponent>(entityID))
        {
            //Save transform component
            TransformComponent& tc = scene->Registry.get<TransformComponent>(entityID);

            s_JSON[tagstr][tc.ID]["Position"][0] = tc.Position.x;
            s_JSON[tagstr][tc.ID]["Position"][1] = tc.Position.y;
            s_JSON[tagstr][tc.ID]["Position"][2] = tc.Position.z;

            s_JSON[tagstr][tc.ID]["Rotation"][0] = tc.Rotation.x;
            s_JSON[tagstr][tc.ID]["Rotation"][1] = tc.Rotation.y;
            s_JSON[tagstr][tc.ID]["Rotation"][2] = tc.Rotation.z;

            s_JSON[tagstr][tc.ID]["Size"][0] = tc.Size.x;
            s_JSON[tagstr][tc.ID]["Size"][1] = tc.Size.y;
            s_JSON[tagstr][tc.ID]["Size"][2] = tc.Size.z;
        }

        if (scene->Registry.try_get<QuadRendererComponent>(entityID))
        {
            QuadRendererComponent& qrc = scene->Registry.get<QuadRendererComponent>(entityID);

            s_JSON[tagstr][qrc.ID]["Scale"][0] = qrc.Scale.x;
            s_JSON[tagstr][qrc.ID]["Scale"][1] = qrc.Scale.y;
            s_JSON[tagstr][qrc.ID]["Scale"][2] = qrc.Scale.z;
           
            s_JSON[tagstr][qrc.ID]["Color"][0] = qrc.Color.r;
            s_JSON[tagstr][qrc.ID]["Color"][1] = qrc.Color.g;
            s_JSON[tagstr][qrc.ID]["Color"][2] = qrc.Color.b;
            s_JSON[tagstr][qrc.ID]["Color"][3] = qrc.Color.a;
        }

        if (scene->Registry.try_get<PerspectiveCameraComponent>(entityID))
        {
            PerspectiveCameraComponent& pcc = scene->Registry.get<PerspectiveCameraComponent>(entityID);

            s_JSON[tagstr][pcc.ID]["MainCamera"] = pcc.MainCamera;

            s_JSON[tagstr][pcc.ID]["FocalPoint"][0] = pcc.FocalPoint.x;
            s_JSON[tagstr][pcc.ID]["FocalPoint"][1] = pcc.FocalPoint.y;
            s_JSON[tagstr][pcc.ID]["FocalPoint"][2] = pcc.FocalPoint.z;

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
        
        Entity entity = scene->NewEntity(key);

        TagComponent& tag = entity.GetComponent<TagComponent>();
        tag.Tag = key;

        if (it.value().contains("TransformComponent"))
        {
            TransformComponent tc;

            tc.Position = { (float)it.value()[tc.ID]["Position"][0],
                            (float)it.value()[tc.ID]["Position"][1],
                            (float)it.value()[tc.ID]["Position"][2]};

            tc.Rotation = { (float)it.value()[tc.ID]["Rotation"][0],
                            (float)it.value()[tc.ID]["Rotation"][1],
                            (float)it.value()[tc.ID]["Rotation"][2]};

            tc.Size     = { (float)it.value()[tc.ID]["Size"][0],
                            (float)it.value()[tc.ID]["Size"][1],
                            (float)it.value()[tc.ID]["Size"][2]};
            
            entity.AddOrReplaceComponent<TransformComponent>(tc);
        }

        if (it.value().contains("QuadRendererComponent"))
        {
            QuadRendererComponent qrc;

            qrc.Scale = { (float)it.value()[qrc.ID]["Scale"][0],
                          (float)it.value()[qrc.ID]["Scale"][1],
                          (float)it.value()[qrc.ID]["Scale"][2]};

            qrc.Color = { (float)it.value()[qrc.ID]["Color"][0],
                          (float)it.value()[qrc.ID]["Color"][1],
                          (float)it.value()[qrc.ID]["Color"][2],
                          (float)it.value()[qrc.ID]["Color"][3]};

            entity.AddOrReplaceComponent<QuadRendererComponent>(qrc);
        }

        if (it.value().contains("PerspectiveCameraComponent"))
        {
            PerspectiveCameraComponent pcc;

            pcc.MainCamera = it.value()[pcc.ID]["MainCamera"];

            pcc.FocalPoint = {(float)it.value()[pcc.ID]["FocalPoint"][0],
                              (float)it.value()[pcc.ID]["FocalPoint"][1], 
                              (float)it.value()[pcc.ID]["FocalPoint"][2]};

            pcc.Distance = (float)it.value()[pcc.ID]["Distance"];
            pcc.Pitch = (float)it.value()[pcc.ID]["Pitch"];
            pcc.Yaw = (float)it.value()[pcc.ID]["Yaw"];
            pcc.FOV = (float)it.value()[pcc.ID]["FOV"];
            pcc.AspectRatio = (float)it.value()[pcc.ID]["AspectRatio"];
            pcc.FixedAspectRatio = (float)it.value()[pcc.ID]["FixedAspectRatio"];
            pcc.NearClip = (float)it.value()[pcc.ID]["NearClip"];
            pcc.FarClip = (float)it.value()[pcc.ID]["FarClip"];

            pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);

            pcc.Camera->SetPitch(pcc.Pitch);
            pcc.Camera->SetYaw(pcc.Yaw);

            entity.AddOrReplaceComponent<PerspectiveCameraComponent>(pcc);
        }
    }

    return scene;
}
