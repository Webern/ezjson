#include "catch.h"
#include "ezjson/ezjson.h"
#include "compareWithoutWhitespace.h"

#include <sstream>

// TODO - dynamic file paths
constexpr const char* const filePath = "/Users/mjb/repos/ezjson/test/data/test-file-001.json";

TEST_CASE( "TestFile001" )
{
    auto jdoc = ezjson::JFactory::makeJDoc();
}
