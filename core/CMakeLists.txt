add_library(Core STATIC)

file(GLOB_RECURSE CORE_SOURCES
    "*.cpp"
    "*.h"
    "*.hpp"
)

target_sources(Core PRIVATE ${CORE_SOURCES})

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
        imgui
        stb
)