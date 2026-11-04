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

#SDL (library)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(external/vendored/SDL EXCLUDE_FROM_ALL)

#modules (libraries), lowest level first
add_subdirectory(modules/Net)
add_subdirectory(modules/Server)
add_subdirectory(modules/Client)

#apps (executables)
add_subdirectory(apps/server)
add_subdirectory(apps/client)