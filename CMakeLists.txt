cmake_minimum_required(VERSION 3.28)
project("cubexx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

add_subdirectory(libs/bw-engine)
add_subdirectory(libs/bw-pecs)
add_subdirectory(extern/stb)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/core/utils.cpp
        src/game.cpp
        src/game/objects/chunk_generator_object.cpp
        src/game/objects/chunk_mesh_generator_object.cpp
        src/game/objects/chunk_unloader_object.cpp
        src/game/objects/grid_object.cpp
        src/game/objects/player_object.cpp
        src/game/objects/world_object.cpp
        src/game/objects/world_renderer_object.cpp
        src/game/chunk_mesh_generator.cpp
        src/game/world_generator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        bw::engine
        bw::pecs
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
