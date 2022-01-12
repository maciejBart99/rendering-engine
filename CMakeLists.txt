# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (gk4)

include_directories(include)

# The main executable
add_executable(main
        src/glad.c
        src/main.cpp
        src/engine/core/Mesh.h src/engine/core/Mesh.cpp
        src/engine/core/SimpleShader.h src/engine/core/SimpleShader.cpp
        src/engine/data_model/Component.cpp src/engine/data_model/Component.h
        src/engine/data_model/data_model.h
        src/engine/core/Camera.h
        src/engine/core/Model.h src/engine/core/Model.cpp
        src/engine/application/Application.cpp src/engine/application/Application.h
        src/engine/core/Engine.cpp src/engine/core/Engine.h src/engine/EngineLogger.cpp src/engine/EngineLogger.h src/engine/core/Camera.cpp src/engine/core/Skybox.cpp src/engine/core/Skybox.h src/demo/demo_components.h src/engine/application/KeyListener.h src/engine/application/SceneBuilder.h)
set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_compile_options(main PRIVATE -Wall)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)

# assimp
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE assimp)

set (source "${CMAKE_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}"
)
