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

namespace Config {
    constexpr int WINDOW_WIDTH = 512;
    constexpr int WINDOW_HEIGHT = 288;
    constexpr const char* WINDOW_TITLE = "CleaveRT!";
    constexpr const char* START_SCENE_PATH = "res/scenes/TestScene.jscn";
    using AudioBackendType = SoLoudBackend;

    constexpr bool USE_EDITOR = true;
}

int main() {
    Window* window = new Window(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE);

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(*window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->SetRenderer((Renderer*)renderer);

    resourceManager->RegisterLoader(std::make_unique<TextureLoader>());
    resourceManager->RegisterLoader(std::make_unique<ShaderLoader>());
    resourceManager->RegisterLoader(std::make_unique<SceneLoader>());
    resourceManager->RegisterLoader(std::make_unique<SoundLoader>());

    resourceManager->ScanResources();

    Input* input = new Input();
    AudioManager* audioManager = new AudioManager(resourceManager, std::make_unique<Config::AudioBackendType>());
    Services::Provide<Input>("Input", input);
    Services::Provide<ResourceManager>("ResMgr", resourceManager);
    Services::Provide<AudioManager>("AudMgr", audioManager);

    Registry::RegisterType<Entity>();
    Registry::RegisterType<AnimatedSprite>();
    Registry::RegisterType<Camera>();
    Registry::RegisterType<Sprite>();

    // input->AddAction("right", GLFW_KEY_D);
    // input->AddAction("right", GLFW_KEY_RIGHT);

#ifdef CLEAVE_EDITOR_ENABLED
    if (Config::USE_EDITOR) {
        Cleave::Editor::EditorContext editor = Cleave::Editor::EditorContext(window);
        editor.Run((Renderer*)renderer);
    }
#else
    audioManager->PlayMusic(resourceManager->Get<Sound>("res/GMate.ogg"));
    Scene* scene = resourceManager->Get<Scene>(Config::START_SCENE_PATH).get();
    auto lastPrintTime = std::chrono::high_resolution_clock::now();
    while (!window->shouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();
        renderer->ClearColor(100, 149, 237, 255);
        renderer->BeginFrame();
        input->Update();
        // if (input->IsActionJustPressed("right")) {
        //     audioManager->PlaySound(resourceManager->Get<Sound>("res/Jump.wav"));
        //     std::cout << "right pressed!" << std::endl;
        // }

        scene->Tick();
        scene->Render((Renderer*)renderer);

        renderer->EndFrame();
        window->swapBuffers();
        window->pollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        float frameTimeMs = std::chrono::duration<float, std::milli>(end - now).count();
        float elapsed = std::chrono::duration<float>(end - lastPrintTime).count();
        if (elapsed >= 1.0f) {
            std::cout << "Frame Time: " << frameTimeMs << " FPS:" << (frameTimeMs > 0.0f ? 1000.0f / frameTimeMs : 0.0f) << std::endl;
            lastPrintTime = end;
        }
    }
#endif
    return 0;
}