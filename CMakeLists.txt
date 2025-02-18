cmake_minimum_required(VERSION 3.28)
project("cubexx")

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(extern/stb)
add_subdirectory(extern/tinyobjloader)
add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        extern/glad/include
        extern/glad-wrap/include
        extern/glfw-wrap/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glad
        glfw
        glm::glm
        tinyobjloader
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
