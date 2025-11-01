#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "Window.hpp"
#include "audio/SoLoudBackend.hpp"
#include "entities/AnimatedSprite.hpp"
#include "entities/Camera.hpp"
#include "entities/SoundPlayer.hpp"
#include "entities/Sprite.hpp"
#include "entities/Tilemap.hpp"
#include "entities/WorldLabel.hpp"
#include "rendering/OpenGLRenderer.hpp"
#include "resources/Font.hpp"
#include "resources/Shader.hpp"
#include "resources/Sound.hpp"
#include "resources/Texture.hpp"
#include "scene/EntityRegistry.hpp"
#include "scene/Scene.hpp"
#include "services/AudioManager.hpp"
#include "services/InputManager.hpp"
#include "services/ResourceManager.hpp"
#include "services/Services.hpp"
#include "thirdparty/stb_image.h"

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
}  // namespace Config

int main() {
    Window* window = new Window(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE);

    Renderer* renderer = static_cast<Renderer*>(new OpenGLRenderer());
    renderer->Initialize(*window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->SetRenderer(renderer);

    resourceManager->RegisterLoader(std::make_unique<TextureLoader>());
    resourceManager->RegisterLoader(std::make_unique<ShaderLoader>());
    resourceManager->RegisterLoader(std::make_unique<SceneLoader>());
    resourceManager->RegisterLoader(std::make_unique<SoundLoader>());
    resourceManager->RegisterLoader(std::make_unique<FontLoader>());

    resourceManager->ScanResources();

    InputManager* input = new InputManager();
    AudioManager* audioManager = new AudioManager(resourceManager, std::make_unique<Config::AudioBackendType>());
    Services::Provide<InputManager>(input);
    Services::Provide<ResourceManager>(resourceManager);
    Services::Provide<AudioManager>(audioManager);

    Registry::RegisterType<Entity>();
    Registry::RegisterType<AnimatedSprite>();
    Registry::RegisterType<Camera>();
    Registry::RegisterType<Sprite>();
    Registry::RegisterType<SoundPlayer>();
    Registry::RegisterType<Tilemap>();
    Registry::RegisterType<WorldLabel>();

#ifdef CLEAVE_EDITOR_ENABLED
    if (Config::USE_EDITOR) {
        Cleave::Editor::EditorContext editor = Cleave::Editor::EditorContext(window);
        editor.Run(renderer);
    } else
#endif
    {
        audioManager->PlayMusic(resourceManager->Get<Sound>("res/GMate.ogg"));
        Scene* scene = resourceManager->Get<Scene>(Config::START_SCENE_PATH).get();
        auto lastPrintTime = std::chrono::high_resolution_clock::now();
        while (!window->shouldClose()) {
            auto now = std::chrono::high_resolution_clock::now();
            renderer->ClearColor(Color(100, 149, 237, 255));
            renderer->BeginFrame();
            input->Update();

            scene->Tick();
            scene->Render(renderer);

            renderer->EndFrame();
            window->swapBuffers();
            window->pollEvents();
            auto end = std::chrono::high_resolution_clock::now();
            float frameTimeMs = std::chrono::duration<float, std::milli>(end - now).count();
            float elapsed = std::chrono::duration<float>(end - lastPrintTime).count();
            if (elapsed >= 1.0f) {
                LOG_INFO("Frame Time: " << frameTimeMs
                                        << " FPS:" << (frameTimeMs > 0.0f ? 1000.0f / frameTimeMs : 0.0f)
                                        << " DrawCalls:" << renderer->GetDrawCalls()
                                        << " TextureSwaps:" << renderer->GetTextureSwaps());
                lastPrintTime = end;
            }
        }
    }
    return 0;
}