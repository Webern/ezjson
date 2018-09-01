cmake_minimum_required(VERSION 3.12)
project(ezjson)

set(SOURCE "./src")
set(INCLUDE "./include")
set(TEST "./test")
set(BUILD_TEST "true")

file(GLOB_RECURSE SRC_PRIVATE ${SOURCE}/*.cpp ${SOURCE}/*.h)
file(GLOB_RECURSE PUBLIC_HEADERS ${SOURCE}/*.h)
file(GLOB_RECURSE TEST_SRC ${TEST}/*.h ${TEST}/*.cpp)

include_directories(ezjson ${INCLUDE})

# ezjson static library
add_library(ezjson STATIC ${SRC_PRIVATE} ${PUBLIC_HEADERS})
source_group(ezjson "public" ${PUBLIC_HEADERS})
source_group(ezjson "private" ${SRC_PRIVATE} )

# ezjson tests
add_executable(ezjsontest ${TEST_SRC})
target_link_libraries(ezjsontest ezjson)
source_group( "test-code" ${TEST_SRC})

set_property(TARGET ezjson PROPERTY CXX_STANDARD 17)
set_property(TARGET ezjsontest PROPERTY CXX_STANDARD 17)
