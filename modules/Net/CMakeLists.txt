add_library(gengr_net STATIC
    src/enet_impl.cpp
    src/host.cpp
)

target_include_directories(gengr_net PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/external/enet"
)

#ENet needs the Windows socket and multimedia libraries
if(WIN32)
    target_link_libraries(gengr_net PUBLIC ws2_32 winmm)
endif()

target_compile_features(gengr_net PUBLIC cxx_std_23)
