add_executable(App)

set(APP_SOURCES
    "main.cpp"
    "worldDraw/drawCommand.h"
    "worldDraw/levelRenderer.cpp"
    "worldDraw/levelRenderer.h"
    "worldDraw/levelVisuals.h"
    "world/level.cpp"
    "world/level.h"
    "world/player.h"
    "game/gameLayer.cpp"
    "game/gameLayer.h"

)

target_sources(App PRIVATE ${APP_SOURCES} "game/gameLayer.cpp" "world/gameObject.h" "world/tiles.h"  )

target_include_directories(App
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${APP_SOURCES}
)

add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Assets"  
        "$<TARGET_FILE_DIR:App>/Assets" 
    COMMENT "Syncing assets from ROOT to BUILD directory..."
)

target_link_libraries(App PRIVATE Core)
