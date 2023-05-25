
#include "SceneSerializer.h"

#include "json.hpp"
#include <iostream>

#include "Entity.h"
#include "Components.h"

#include <fstream>



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

        if (scene->Registry.has<TransformComponent>(entityID))
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

        if (scene->Registry.has<QuadRendererComponent>(entityID))
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

    Scene* scene = new Scene(path.filename().string());

    // Iterate through the first-level keys
    for (json::iterator it = s_JSON.begin(); it != s_JSON.end(); ++it)
    {
        std::string key = it.key();
        std::cout << "Key: " << key << std::endl;

        Entity entity = scene->NewEntity(key);

        TagComponent& tag = entity.GetComponent<TagComponent>();
        tag.Tag = key;

        if (it.value().contains("TransformComponent"))
        {
            TransformComponent tc;

            tc.Position = { (float)it.value()["TransformComponent"]["Position"][0],
                            (float)it.value()["TransformComponent"]["Position"][1],
                            (float)it.value()["TransformComponent"]["Position"][2]};

            tc.Rotation = { (float)it.value()["TransformComponent"]["Rotation"][0],
                            (float)it.value()["TransformComponent"]["Rotation"][1],
                            (float)it.value()["TransformComponent"]["Rotation"][2]};

            tc.Size     = { (float)it.value()["TransformComponent"]["Size"][0],
                            (float)it.value()["TransformComponent"]["Size"][1],
                            (float)it.value()["TransformComponent"]["Size"][2]};
            
            entity.AddOrReplaceComponent<TransformComponent>(tc);
        }

        if (it.value().contains("QuadRendererComponent"))
        {
            QuadRendererComponent qrc;

            qrc.Scale = { (float)it.value()["QuadRendererComponent"]["Scale"][0],
                          (float)it.value()["QuadRendererComponent"]["Scale"][1],
                          (float)it.value()["QuadRendererComponent"]["Scale"][2] };

            qrc.Color = { (float)it.value()["QuadRendererComponent"]["Color"][0],
                          (float)it.value()["QuadRendererComponent"]["Color"][1],
                          (float)it.value()["QuadRendererComponent"]["Color"][2],
                          (float)it.value()["QuadRendererComponent"]["Color"][3] };

            entity.AddOrReplaceComponent<QuadRendererComponent>(qrc);
        }
    }

    return scene;
}
