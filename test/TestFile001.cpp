#include "catch.h"
#include "ezjson/ezjson.h"
#include "compareWithoutWhitespace.h"

#include <sstream>
#include <fstream>

// TODO - dynamic file paths
constexpr const char* const filePath = "/Users/mjb/repos/ezjson/test/data/test-file-001.json";
constexpr const char* const outPath = "/Users/mjb/Desktop/test-file-001-out.json";

TEST_CASE( "TestFile001" )
{
    auto jdoc = ezjson::JFactory::makeJDoc();
    std::ifstream infile{ filePath };
    const std::string expected( ( std::istreambuf_iterator<char>( infile ) ), std::istreambuf_iterator<char>() );
    std::istringstream iss{ expected };
    jdoc->loadStream( iss );
    jdoc->saveFile( outPath );
    std::ifstream reloadFile{ outPath };
    const std::string actual( ( std::istreambuf_iterator<char>( reloadFile ) ), std::istreambuf_iterator<char>() );
    CHECK( ezjson::areEqualWithoutWhiteSpace( expected, actual ) == "" );
}
