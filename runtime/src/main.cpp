#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "Services.hpp"
#include "Window.hpp"
#include "entities/Sprite.hpp"
#include "rendering/Renderer.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/Texture.hpp"
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

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    FragColor = texture(tex, TexCoord);
}
)";

const char* spriteVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* spriteFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    FragColor = texture(tex, TexCoord);
}
)";

int main() {
    Window* window = new Window(512, 288, "CleaveRT!");

    OpenGLRenderer* renderer = new OpenGLRenderer();
    renderer->Initialize(*window);

    ResourceManager* resourceManager = new ResourceManager();
    resourceManager->AddTexture("cat.png");
    resourceManager->AddTexture("dog.png");
    resourceManager->AddShaderFromString("main", vertexShaderSource,
                                         fragmentShaderSource);
    resourceManager->AddShaderFromString("sprite", spriteVertexShaderSource,
                                         spriteFragmentShaderSource);
    Services::Provide<ResourceManager>("ResMgr", resourceManager);

    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    Registry::RegisterType<Entity>();
    Registry::RegisterType<Sprite>();

    Services::Provide<Registry>("registry", std::make_shared<Registry>());

    Sprite* cat =
        new Sprite(Transform({16, 32}), resourceManager->textures["cat.png"]);
    cat->SetName("Carlos Gato");

    Sprite* dog =
        new Sprite(Transform({128, 128}), resourceManager->textures["dog.png"]);
    dog->SetName("Roberto Cao");
    cat->AddChild(dog);
#ifdef CLEAVE_EDITOR_ENABLED
    Cleave::Editor::Editor editor = Cleave::Editor::Editor(window);
    Scene* scene = editor.GetGameView()->GetScene();

    scene->GetRoot()->AddChild(cat);

    editor.Run((Renderer*)renderer);
#else
    while (!window->shouldClose()) {
        renderer->ClearColor(100, 149, 237, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        resourceManager->shaders["main"]->Use();
        resourceManager->shaders["main"]->SetUniformInt("tex", 0);
        resourceManager->shaders["main"]->SetUniformMatrix4(
            "projection", glm::value_ptr(renderer->GetProjection()));
        cat->OnRender((Renderer*)renderer);

        window->swapBuffers();
        window->pollEvents();
    }
#endif
    return 0;
}