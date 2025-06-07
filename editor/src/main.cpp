#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

#include <algorithm>

#include "Window.hpp"
#include "Services.hpp"
#include "rendering/Renderer.hpp"
#include "resources/Texture.hpp"
#include "resources/ResourceManager.hpp"
#include "entities/Sprite.hpp"

#include "Editor.hpp"
#include "GameView.hpp"
#include "scene/Scene.hpp"
#include "Properties.hpp"
#include "scene/EntityRegistry.hpp"

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
    resourceManager->AddShaderFromString("main", vertexShaderSource, fragmentShaderSource);
    resourceManager->AddShaderFromString("sprite", spriteVertexShaderSource, spriteFragmentShaderSource);
    Services::Provide(resourceManager);

    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    const TypeRegistry registries[] = {
        {"cleave::Sprite", []() { return new Sprite(); }},
    };
    for (const TypeRegistry& registry : registries) {
        Registry::RegisterType(registry);
    }

    Editor editor = Editor(window);
    Scene* scene = editor.GetGameView()->GetScene();

    Sprite* cat = new Sprite(Transform({ 16, 32 }), resourceManager->textures["cat.png"]);
    cat->SetName("Carlos Gato");

    Sprite* dog = new Sprite(Transform({ 128, 128 }), resourceManager->textures["dog.png"]);
    dog->SetName("Roberto Cao");
    cat->AddChild(dog);

    scene->GetRoot()->AddChild(cat);
    
    editor.Run((Renderer*)renderer);
    return 0;
}