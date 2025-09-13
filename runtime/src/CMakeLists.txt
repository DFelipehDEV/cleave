if(WIN32)
	set(PLATFORM win32)
endif()

set(AUDIO_BACKEND SoLoud)

set(SOURCES
	Input.cpp
	Services.cpp
	Window.cpp
	audio/${AUDIO_BACKEND}Backend.cpp
	audio/AudioManager.cpp
	math/Matrix4.cpp
	math/Transform.cpp
	scene/JsonSceneSerializer.cpp
	scene/Scene.cpp
	entities/AnimatedSprite.cpp
	entities/Camera.cpp
	entities/Entity.cpp
	entities/SoundPlayer.cpp
	entities/Sprite.cpp
	platform/${PLATFORM}/FileDialog.cpp
	platform/${PLATFORM}/MessageBox.cpp
	rendering/OpenGLRenderer.cpp
	resources/Resource.cpp
	resources/ResourceManager.cpp	
	resources/Shader.cpp
	resources/Sound.cpp
	resources/Texture.cpp
)

set(HEADERS
	Input.hpp
	Services.hpp
	Window.hpp
	audio/${AUDIO_BACKEND}Backend.hpp
	audio/AudioManager.hpp
	scene/JsonSceneSerializer.hpp
	scene/EntityRegistry.hpp
	scene/Scene.hpp
	math/Matrix4.hpp
	math/Rect4.hpp
	math/Transform.hpp
	math/Vec2.hpp
	entities/AnimatedSprite.hpp
	entities/Camera.hpp
	entities/Entity.hpp
	entities/SoundPlayer.hpp
	entities/Sprite.hpp
	platform/FileDialog.hpp
	platform/MessageBox.hpp
	rendering/OpenGLRenderer.hpp
	rendering/Renderer.hpp
	resources/Resource.hpp
	resources/ResourceManager.hpp
	resources/Shader.hpp
	resources/Sound.hpp
	resources/Texture.hpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(SOLOUD_BACKEND_WINMM ON CACHE BOOL "" FORCE)
set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/soloud/contrib ${CMAKE_BINARY_DIR}/soloud_build)

add_library(CleaveRuntime STATIC ${SOURCES} ${HEADERS})
target_link_libraries(CleaveRuntime PRIVATE GLEW::GLEW glfw soloud)
target_include_directories(CleaveRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CleaveRuntimeExe main.cpp)
target_link_libraries(CleaveRuntimeExe PRIVATE CleaveRuntime GLEW::GLEW glfw)

option(BUILD_EDITOR "Build the Editor module" ON)
if(BUILD_EDITOR)
    add_subdirectory(editor)
    target_link_libraries(CleaveRuntimeExe PRIVATE CleaveEditor)
    target_compile_definitions(CleaveRuntimeExe PRIVATE CLEAVE_EDITOR_ENABLED)
endif()