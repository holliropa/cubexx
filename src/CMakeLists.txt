set(SOURCES
        main.cpp
        application.hpp
        application.cpp
        attribute_layout.hpp
        mesh.hpp
        mesh.cpp
        shader.hpp
        shader.cpp
        not_defined.hpp
        files.hpp
        files.cpp
        game.hpp
        game.cpp
        key_code.hpp
        button_code.hpp
        shader_manager.hpp
        shader_manager.cpp
        mesh_manager.hpp
        mesh_manager.cpp
        camera.hpp
        utils.hpp
        utils.cpp
        input.hpp
        input.cpp
        key_action.hpp
        object.hpp
        camera_behaviour.hpp
        texture2D.hpp
        texture2D.cpp
        texture_manager.hpp
        texture_manager.cpp
        gizmo_object.hpp
        chunk_object.hpp
)

# Prepend "src/" to every source file
list(TRANSFORM SOURCES PREPEND "src/")

# Propagate the SOURCES variable to the parent scope
set(SOURCES ${SOURCES} PARENT_SCOPE)