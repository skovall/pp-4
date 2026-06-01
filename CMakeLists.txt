cmake_minimum_required(VERSION 3.10)
project(cpp_cicd_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Приложение
add_executable(app src/main.cpp)

# Юнит-тесты (только если есть папка tests и файл test_processor.cpp)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_processor.cpp")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(tests tests/test_processor.cpp)
    target_link_libraries(tests gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif()
