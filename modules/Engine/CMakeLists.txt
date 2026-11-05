add_library(gengr_engine STATIC
    src/window.cpp
    src/input.cpp
)

target_include_directories(gengr_engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(gengr_engine
    PUBLIC  gengr_net
    PRIVATE raylib
)