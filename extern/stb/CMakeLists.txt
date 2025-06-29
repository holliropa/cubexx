include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(stb STATIC
        src/stb.cpp
)

target_include_directories(stb
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   # Public headers
        $<INSTALL_INTERFACE:include>                            # Installed location
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>      # Explicitly require internal prefix
)