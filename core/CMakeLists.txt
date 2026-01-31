add_library(Core STATIC)



target_sources(Core PRIVATE ${CORE_SOURCES} 
"application/application.h" 
"application/application.cpp"   
"application/event.h" 
"application/inputEvents.h" 
"application/layer.cpp" 
"application/layer.h" 
"application/window.cpp" 
"application/window.h" 
"application/windowEvents.h"   
"camera/camera.h"
"camera/cameraController.h"
"camera/cameraController.cpp"
"gl/mesh.h"
"gl/model.cpp"
"gl/model.h"
"gl/shader.cpp"
"gl/shader.h"
"gl/texturedModel.cpp"
"gl/texturedModel.h"
"gl/texture2d.h"

)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${CORE_SOURCES}
)

target_link_libraries(Core
    PUBLIC 
        glad
        glfw
        glm
        imgui
        stb
        assimp
)