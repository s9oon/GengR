add_executable(GengRClient
    main.cpp
)

target_link_libraries(GengRClient PRIVATE gengr_client SDL3::SDL3)