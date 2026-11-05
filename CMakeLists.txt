cmake_minimum_required(VERSION 3.23)

option(GENGR_ALLOW_UNTESTED_CMAKE
    "Allow configuration with untested CMake versions"
    OFF
)

if(CMAKE_VERSION VERSION_LESS "4.4.4" AND NOT GENGR_ALLOW_UNTESTED_CMAKE)
    message(FATAL_ERROR
        "Geng was developed on CMake 4.4.4 or newer.\n"
        "Your installed version is ${CMAKE_VERSION}.\n"
        "Older versions have not been tested.\n"
        "Set GENGR_ALLOW_UNTESTED_CMAKE=ON to bypass this check."
    )
endif()

project(GengR VERSION 0.1.0)

#import raylib
add_library(raylib SHARED IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_IMPLIB               "${CMAKE_SOURCE_DIR}/external/raylib/raylibdll.lib"
    IMPORTED_LOCATION             "${CMAKE_SOURCE_DIR}/external/raylib/raylib.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/raylib"
)

#modules (libraries), lowest level first
add_subdirectory(modules/Net)
add_subdirectory(modules/Engine)
add_subdirectory(modules/Server)
add_subdirectory(modules/Client)

#apps (executables)
add_subdirectory(apps/server)
add_subdirectory(apps/client)