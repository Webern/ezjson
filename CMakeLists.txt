cmake_minimum_required(VERSION 3.12)
project(ezjson)

set(SOURCE "./src")
set(INCLUDE "./include")
set(TEST "./test")
set(RAPIDJSON_HEADER_DIR "./external/rapidjson/include/rapidjson")
set(BUILD_TESTS "true")

file(GLOB_RECURSE SRC_PRIVATE ${SOURCE}/*.cpp ${SOURCE}/*.h)
file(GLOB_RECURSE PUBLIC_HEADERS ${SOURCE}/*.h)
file(GLOB_RECURSE RAPIDJSON ${RAPIDJSON_HEADER_DIR})
file(GLOB_RECURSE TEST_SRC ${TEST}/*.h ${TEST}/*.cpp)

# ezjson static library
add_library(ezjson STATIC ${SRC_PRIVATE} ${PUBLIC_HEADERS} ${RAPIDJSON})
source_group(ezjson "public" ${PUBLIC_HEADERS})
source_group(ezjson "private" ${SRC_PRIVATE})
source_group(ezjson "rapid-json" ${RAPID_JSON})
include_directories(ezjson PUBLIC ${INCLUDE})
include_directories(ezjson PRIVATE ${SOURCE})
include_directories(ezjson PRIVATE ${RAPIDJSON})
set_property(TARGET ezjson PROPERTY CXX_STANDARD 17)

# ezjson tests
add_executable(ezjsontest ${TEST_SRC})
target_link_libraries(ezjsontest ezjson)
source_group( "test-code" ${TEST_SRC})
include_directories(ezjsontest PRIVATE ${TEST_SRC})
include_directories(ezjsontest PUBLIC ${INCLUDE})
include_directories(ezjsontest PRIVATE ${SRC_PRIVATE})

set_property(TARGET ezjsontest PROPERTY CXX_STANDARD 17)
