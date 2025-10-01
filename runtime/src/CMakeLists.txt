if(WIN32)
	set(PLATFORM win32)
endif()

set(AUDIO_BACKEND SoLoud)

set(SOURCES
	services/InputManager.cpp
	services/Services.cpp
	Window.cpp
	audio/${AUDIO_BACKEND}Backend.cpp
	services/AudioManager.cpp
	math/Matrix4.cpp
	math/Transform.cpp
	scene/JsonSceneSerializer.cpp
	scene/Scene.cpp
	entities/AnimatedSprite.cpp
	entities/Camera.cpp
	entities/Entity.cpp
	entities/SoundPlayer.cpp
	entities/Sprite.cpp
	entities/WorldLabel.cpp
	platform/${PLATFORM}/FileDialog.cpp
	platform/${PLATFORM}/MessageBox.cpp
	rendering/OpenGLRenderer.cpp
	rendering/RenderTarget.cpp
	resources/Resource.cpp
	resources/Font.cpp
	services/ResourceManager.cpp	
	resources/Shader.cpp
	resources/Sound.cpp
	resources/Texture.cpp
)

set(HEADERS
	services/InputManager.hpp
	Log.hpp
	UUID.hpp
	services/Service.hpp
	services/Services.hpp
	Window.hpp
	audio/${AUDIO_BACKEND}Backend.hpp
	services/AudioManager.hpp
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
	entities/WorldLabel.hpp
	platform/FileDialog.hpp
	platform/MessageBox.hpp
	rendering/Color.hpp
	rendering/FontHandle.hpp
	rendering/OpenGLRenderer.hpp
	rendering/Renderer.hpp
	rendering/RenderTarget.hpp
	rendering/RenderTargetHandle.hpp
	rendering/ShaderHandle.hpp
	rendering/TextureFormat.hpp
	rendering/TextureHandle.hpp
	resources/Resource.hpp
	resources/Font.hpp
	services/ResourceManager.hpp
	resources/Shader.hpp
	resources/Sound.hpp
	resources/Texture.hpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

set(SOLOUD_BACKEND_WINMM ON CACHE BOOL "" FORCE)
set(SOLOUD_BACKEND_SDL2 OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/soloud/contrib ${CMAKE_BINARY_DIR}/soloud_build)

add_library(CleaveRuntime STATIC ${SOURCES} ${HEADERS})
target_link_libraries(CleaveRuntime PRIVATE GLEW::GLEW glfw soloud Freetype::Freetype)
target_include_directories(CleaveRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CleaveRuntimeExe main.cpp)
target_link_libraries(CleaveRuntimeExe PRIVATE CleaveRuntime GLEW::GLEW glfw)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/res)
add_custom_command(
    TARGET CleaveRuntimeExe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCES_DIR}
            $<TARGET_FILE_DIR:CleaveRuntimeExe>/res
)

option(BUILD_EDITOR "Build the Editor module" ON)
if(BUILD_EDITOR)
    add_subdirectory(editor)
    target_link_libraries(CleaveRuntimeExe PRIVATE CleaveEditor)
    target_compile_definitions(CleaveRuntimeExe PRIVATE CLEAVE_EDITOR_ENABLED)
endif()