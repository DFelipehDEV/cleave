#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "Services.hpp"
#include "Window.hpp"
#include "entities/AnimatedSprite.hpp"
#include "entities/Camera.hpp"
#include "entities/Sprite.hpp"
#include "rendering/OpenGLRenderer.hpp"

#include "resources/ResourceManager.hpp"
#include "resources/Shader.hpp"
#include "resources/Texture.hpp"
#include "resources/Sound.hpp"

#include "audio/AudioManager.hpp"
#include "audio/SoLoudBackend.hpp"

#include "scene/EntityRegistry.hpp"
#include "scene/Scene.hpp"
#include "thirdparty/stb_image.h"
#include "Input.hpp"

using namespace Cleave;

#ifdef CLEAVE_EDITOR_ENABLED
#include "editor/EditorContext.hpp"
#include "editor/GameView.hpp"
#include "editor/Properties.hpp"
using namespace Cleave::Editor;
#endif

int main() {
    Window* window = new Window(512, 288, "CleaveRT!");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(*window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->SetRenderer((Renderer*)renderer);

    std::shared_ptr<AudioManager> audioManager = std::make_shared<AudioManager>(resourceManager, std::make_unique<SoLoudBackend>());

    Input* input = new Input();
    Services::Provide<Input>("Input", input);

    resourceManager->RegisterLoader(std::make_unique<TextureLoader>());
    resourceManager->RegisterLoader(std::make_unique<ShaderLoader>());
    resourceManager->RegisterLoader(std::make_unique<SceneLoader>());
    resourceManager->RegisterLoader(std::make_unique<SoundLoader>());

    resourceManager->ScanResources();

    Services::Provide<ResourceManager>("ResMgr", resourceManager);
    Services::Provide<AudioManager>("AudMgr", audioManager);

    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    Registry::RegisterType<Entity>();
    Registry::RegisterType<AnimatedSprite>();
    Registry::RegisterType<Camera>();
    Registry::RegisterType<Sprite>();

    Services::Provide<Registry>("registry", std::make_shared<Registry>());
    input->AddAction("right", GLFW_KEY_D);
    input->AddAction("right", GLFW_KEY_RIGHT);

#ifndef CLEAVE_EDITOR_ENABLED
    Cleave::Editor::EditorContext editor =
        Cleave::Editor::EditorContext(window);
    Scene* scene = editor.GetCurrentGameView()->GetScene();

    editor.Run((Renderer*)renderer);
#else
    audioManager->PlayMusic(resourceManager->Get<Sound>("res/GMate.ogg"));
    Scene* scene = resourceManager->Get<Scene>("res/scenes/TestScene.jscn").get();
    while (!window->shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        renderer->BeginFrame();
        input->Update();
        if (input->IsActionJustPressed("right")) {
            audioManager->PlaySound(resourceManager->Get<Sound>("res/Jump.wav"));
            std::cout << "right pressed!" << std::endl;
        }
        scene->Tick();
        scene->Render((Renderer*)renderer);
        renderer->EndFrame();
        window->swapBuffers();
        window->pollEvents();
    }
#endif
    return 0;
}