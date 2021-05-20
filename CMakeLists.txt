cmake_minimum_required(VERSION 3.5)

project(watchman)

file(GLOB source_files "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/log/*.cpp" "${PROJECT_SOURCE_DIR}/src/utils/*.cpp")
include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/vendor" "${PROJECT_SOURCE_DIR}/vendor/rapidjson/include")

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -DDEBUGMODE -std=c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++20")
endif()

add_executable(watchman ${source_files})