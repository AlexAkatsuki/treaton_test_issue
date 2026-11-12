cmake_minimum_required(VERSION 3.10.0)
project(treaton_test_issue VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(treaton_test_issue
    main.cpp
)

option(TASK "Build task 1 (OFF builds task 2)" ON)

if(TASK)
    target_compile_definitions(treaton_test_issue PRIVATE TASK_1)
    message(STATUS "BUILDING TASK 1")
else()
    target_compile_definitions(treaton_test_issue PRIVATE TASK_2)
    message(STATUS "BUILDING TASK 2")
endif()

target_compile_options(treaton_test_issue PRIVATE -O0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#Simple_NeauralNet
