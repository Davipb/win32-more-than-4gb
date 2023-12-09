cmake_minimum_required(VERSION 3.16.3)
project(memorytest
    LANGUAGES C
)

add_executable(memorytest
    main.c
)
set_property(TARGET memorytest PROPERTY CXX_STANDARD 20)
set_property(TARGET memorytest PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_options(memorytest PRIVATE /LARGEADDRESSAWARE)
