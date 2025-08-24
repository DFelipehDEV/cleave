#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "Services.hpp"
#include "Window.hpp"
#include "entities/AnimatedSprite.hpp"
#include "entities/Camera.hpp"
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
#include "editor/EditorContext.hpp"
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
    resourceManager->RegisterLoader(std::make_unique<SceneLoader>());

    resourceManager->ScanResources();

    Services::Provide<ResourceManager>("ResMgr", resourceManager);

    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    Registry::RegisterType<Entity>();
    Registry::RegisterType<AnimatedSprite>();
    Registry::RegisterType<Camera>();
    Registry::RegisterType<Sprite>();

    Services::Provide<Registry>("registry", std::make_shared<Registry>());

    std::unique_ptr<Sprite> cat = std::make_unique<Sprite>(
        Transform({16, 32}), resourceManager->Get<Texture>("res/textures/cat.png"));
    cat->SetName("Carlos Gato");

    std::unique_ptr<Entity> dog = std::make_unique<Entity>(Transform({48, 64}));
    dog->SetName("Roberto Cao");
    {
        std::unique_ptr<AnimatedSprite> animatedDogIdle =
            std::make_unique<AnimatedSprite>(Transform({116, 128}), resourceManager->Get<Texture>("res/textures/dog-sheet-idle.png"), 116, 167);
        std::unique_ptr<AnimatedSprite> animatedDogWalk =
            std::make_unique<AnimatedSprite>(Transform({232, 128}), resourceManager->Get<Texture>("res/textures/dog-sheet-walk.png"), 116, 167);
        animatedDogIdle->SetName("AnimatedDogIdle");
        animatedDogWalk->SetName("AnimatedDogWalk");

        animatedDogIdle->Play(5.0f, true);
        animatedDogWalk->Play(10.0f, true);
        dog->AddChild(std::move(animatedDogIdle));
        dog->AddChild(std::move(animatedDogWalk));
    }

    cat->AddChild(std::move(dog));
#ifdef CLEAVE_EDITOR_ENABLED
    Cleave::Editor::EditorContext editor =
        Cleave::Editor::EditorContext(window);
    Scene* scene = editor.GetGameView()->GetScene();

    scene->GetRoot()->AddChild(std::move(cat));

    editor.Run((Renderer*)renderer);
#else
    while (!window->shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        cat->OnTick(1.0f / 60.0f);
        cat->OnRender((Renderer*)renderer);

        window->swapBuffers();
        window->pollEvents();
    }
#endif
    return 0;
}