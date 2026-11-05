add_library(gengr_server STATIC
    src/server.cpp
)

target_include_directories(gengr_server PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(gengr_server PUBLIC gengr_net)