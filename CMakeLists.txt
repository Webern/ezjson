cmake_minimum_required(VERSION 3.12)
project(ezjson)

set(EZJ_SOURCE_DIR "./src")
set(EZJ_PUBLIC_INCLUDE_DIR "./include")
set(EZJ_TEST_DIR "./test")
set(RAPIDJSON_HEADER_DIR "./external/rapidjson/include")
set(OPTION_BUILD_TESTS "true")

file(GLOB_RECURSE EZJ_PRIVATE_SOURCE_FILE_LIST ${EZJ_SOURCE_DIR}/*.cpp ${EZJ_SOURCE_DIR}/*.h)
file(GLOB_RECURSE EZJ_PUBLIC_HEADER_FILE_LIST ${EZJ_PUBLIC_INCLUDE_DIR}/ezjson/*.h)
file(GLOB_RECURSE RAPIDJSON_HEADER_FILE_LIST ${RAPIDJSON_HEADER_DIR}/rapidjson/*.h*)
file(GLOB_RECURSE EZJ_TEST_SOURCE_FILE_LIST ${EZJ_TEST_DIR}/*.h ${EZJ_TEST_DIR}/*.cpp)

# ezjson static library
add_library(ezjson STATIC ${EZJ_PRIVATE_SOURCE_FILE_LIST} ${EZJ_PUBLIC_HEADER_FILE_LIST} ${RAPIDJSON_HEADER_FILE_LIST})
set_property(TARGET ezjson PROPERTY CXX_STANDARD 17)

source_group("public" FILES ${EZJ_PUBLIC_HEADER_FILE_LIST})
source_group("private" FILES ${EZJ_PRIVATE_SOURCE_FILE_LIST})
source_group("rapidjson" FILES ${RAPIDJSON_HEADER_FILE_LIST})

include_directories(ezjson PUBLIC ${EZJ_PUBLIC_INCLUDE_DIR})
include_directories(ezjson PRIVATE ${EZJ_SOURCE_DIR})
include_directories(ezjson PRIVATE ${RAPIDJSON_HEADER_DIR})


# ezjson tests
add_executable(ezjsontest ${EZJ_TEST_SOURCE_FILE_LIST})
target_link_libraries(ezjsontest ezjson)

source_group("test-code" FILES ${EZJ_TEST_SOURCE_FILE_LIST})

include_directories(ezjsontest PRIVATE ${EZJ_TEST_DIR})
include_directories(ezjsontest PUBLIC ${EZJ_PUBLIC_INCLUDE_DIR})
include_directories(ezjson PRIVATE ${EZJ_SOURCE_DIR})

set_property(TARGET ezjsontest PROPERTY CXX_STANDARD 17)
