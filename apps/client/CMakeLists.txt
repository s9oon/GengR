add_executable(GengRClient
    main.cpp
)

target_link_libraries(GengRClient PRIVATE gengr_client raylib)

add_custom_command(TARGET GengRClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:GengRClient> $<TARGET_FILE_DIR:GengRClient>
    COMMAND_EXPAND_LISTS)