add_library(gengr_client STATIC
    src/client.cpp
)

target_include_directories(gengr_client PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(gengr_client PUBLIC gengr_net)