#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "Services.hpp"
#include "Window.hpp"
#include "entities/Sprite.hpp"
#include "rendering/Renderer.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "resources/ShaderLoader.hpp"
#include "resources/Texture.hpp"
#include "resources/TextureLoader.hpp"
#include "scene/EntityRegistry.hpp"
#include "scene/Scene.hpp"
#include "thirdparty/stb_image.h"


using namespace Cleave;

#ifdef CLEAVE_EDITOR_ENABLED
#include "editor/Editor.hpp"
#include "editor/GameView.hpp"
#include "editor/Properties.hpp"
using namespace Cleave::Editor;
#endif
#include <glm/gtc/type_ptr.hpp>

int main() {
    Window* window = new Window(512, 288, "CleaveRT!");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(*window);

    ResourceManager* resourceManager = new ResourceManager();

    resourceManager->RegisterLoader(std::make_unique<TextureLoader>());
    resourceManager->RegisterLoader(std::make_unique<ShaderLoader>());

    resourceManager->ScanResources();

    auto texture = resourceManager->Get<Texture>("cat");

    Services::Provide<ResourceManager>("ResMgr", resourceManager);

    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    Registry::RegisterType<Entity>();
    Registry::RegisterType<Sprite>();

    Services::Provide<Registry>("registry", std::make_shared<Registry>());

    std::unique_ptr<Sprite> cat = std::make_unique<Sprite>(
        Transform({16, 32}), resourceManager->Get<Texture>("cat"));
    cat->SetName("Carlos Gato");

    std::unique_ptr<Sprite> dog = std::make_unique<Sprite>(
        Transform({128, 128}), resourceManager->Get<Texture>("dog"));
    dog->SetName("Roberto Cao");
    cat->AddChild(std::move(dog));
#ifdef CLEAVE_EDITOR_ENABLED
    Cleave::Editor::Editor editor = Cleave::Editor::Editor(window);
    Scene* scene = editor.GetGameView()->GetScene();

    scene->GetRoot()->AddChild(std::move(cat));

    editor.Run((Renderer*)renderer);
#else
    while (!window->shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        resourceManager->Get<Shader>("main")->Use();
        resourceManager->Get<Shader>("main")->SetUniformInt("tex", 0);
        resourceManager->Get<Shader>("main")->SetUniformMatrix4(
            "projection", glm::value_ptr(renderer->GetProjection()));
        cat->OnRender((Renderer*)renderer);

        window->swapBuffers();
        window->pollEvents();
    }
#endif
    return 0;
}