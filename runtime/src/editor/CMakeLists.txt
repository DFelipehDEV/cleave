find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)

set(EDITOR_SOURCES
	EditorContext.cpp
	FileExplorer.cpp
	GameView.cpp
	Hierarchy.cpp
	MainMenuBar.cpp
	Properties.cpp
)

set(EDITOR_HEADERS
	EditorContext.hpp
	FileExplorer.hpp
	GameView.hpp
	Hierarchy.hpp
	MainMenuBar.hpp
	Properties.hpp
)

add_library(CleaveEditor ${EDITOR_SOURCES} ${EDITOR_HEADERS})
target_link_libraries(CleaveEditor PRIVATE CleaveRuntime GLEW::GLEW glfw imgui::imgui nlohmann_json::nlohmann_json)
target_include_directories(CleaveEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
