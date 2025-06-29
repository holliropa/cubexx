cmake_minimum_required(VERSION 3.28)
project("cubexx")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

add_subdirectory(libs/bw-engine)
add_subdirectory(extern/stb)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/chunk_mesh_generator.cpp
        src/cube.cpp
        src/game.cpp
        src/gizmo.cpp
        src/grid.cpp
        src/player.cpp
        src/utils.cpp
        src/world.cpp
        src/world_generator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        bw::engine
        stb)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

# Create a custom target for copying assets
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMENT "Copying assets directory to build folder"
)

# Make the executable depend on the custom target
add_dependencies(${PROJECT_NAME} copy_assets)
