if(WIN32)
	set(PLATFORM win32)
endif()

set(SOURCES
	scene/JsonSceneSerializer.cpp
	math/Matrix4.cpp
	scene/Scene.cpp
	Services.cpp
	math/Transform.cpp
	Window.cpp
	entities/AnimatedSprite.cpp
	entities/Camera.cpp
	entities/Entity.cpp
	entities/Sprite.cpp
	platform/${PLATFORM}/FileDialog.cpp
	platform/${PLATFORM}/MessageBox.cpp
	rendering/Renderer.cpp
	resources/Resource.cpp
	resources/ResourceManager.cpp	
	resources/Shader.cpp
	resources/Texture.cpp
)

set(HEADERS
	scene/JsonSceneSerializer.hpp
	math/Matrix4.hpp
	math/Rect4.hpp
	scene/EntityRegistry.hpp
	scene/Scene.hpp
	Services.hpp
	math/Transform.hpp
	math/Vec2.hpp
	Window.hpp
	entities/AnimatedSprite.hpp
	entities/Camera.hpp
	entities/Entity.hpp
	entities/Sprite.hpp
	platform/FileDialog.hpp
	platform/MessageBox.hpp
	rendering/Renderer.hpp
	resources/Resource.hpp
	resources/ResourceManager.hpp
	resources/Shader.hpp
	resources/Texture.hpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(CleaveRuntime STATIC ${SOURCES} ${HEADERS})
target_link_libraries(CleaveRuntime PRIVATE GLEW::GLEW glfw glm::glm)
target_include_directories(CleaveRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CleaveRuntimeExe main.cpp)
target_link_libraries(CleaveRuntimeExe PRIVATE CleaveRuntime GLEW::GLEW glfw glm::glm)

option(BUILD_EDITOR "Build the Editor module" ON)
if(BUILD_EDITOR)
    add_subdirectory(editor)
    target_link_libraries(CleaveRuntimeExe PRIVATE CleaveEditor)
    target_compile_definitions(CleaveRuntimeExe PRIVATE CLEAVE_EDITOR_ENABLED)
endif()