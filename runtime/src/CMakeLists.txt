set(SOURCES
	main.cpp
	Matrix4.cpp
	Scene.cpp
	Services.cpp
	Window.cpp
	entities/Entity.cpp
	entities/Sprite.cpp
	rendering/Renderer.cpp
	resources/Resource.cpp	
	resources/Shader.cpp
	resources/Texture.cpp
	resources/ResourceManager.cpp
)

set(HEADERS
	Matrix4.hpp
	Scene.hpp
	Services.hpp
	Vec2.hpp
	Window.hpp
	entities/Entity.hpp
	entities/Sprite.hpp
	rendering/Renderer.hpp
	resources/Resource.hpp
	resources/Shader.hpp
	resources/Texture.hpp
	resources/ResourceManager.hpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(CleaveRuntime ${SOURCES} ${HEADERS})
target_link_libraries(CleaveRuntime PRIVATE GLEW::GLEW glfw glm::glm)
target_include_directories(CleaveRuntime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})