set(SOURCES
	main.cpp
)

set(HEADERS
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

add_executable(CleaveEditor ${SOURCES} ${HEADERS})
target_link_libraries(CleaveEditor PRIVATE CleaveRuntime GLEW::GLEW glfw glm::glm imgui::imgui)
target_include_directories(CleaveEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
