#include "catch.h"
#include "SharedStrings.h"

TEST_CASE( "SharedStrings - add" )
{
    using namespace ezjsonprivate;

    SharedStrings strings;
    const auto resultA = strings.add( "A" );
    CHECK( true == resultA.first );
    CHECK( 0 == resultA.second );
    CHECK( "A" == strings.get( 0 ) );
    CHECK( 0 == strings.get( "A" ) );

    const auto resultA2 = strings.add( "A" );
    CHECK( false == resultA2.first );
    CHECK( 0 == resultA2.second );

    const auto resultB = strings.add( "B" );
    CHECK( true == resultB.first );
    CHECK( 1 == resultB.second );
    CHECK( "B" == strings.get( 1 ) );
    CHECK( 1 == strings.get( "B" ) );

    const auto resultB2 = strings.add( "B" );
    CHECK( false == resultB2.first );
    CHECK( 1 == resultB2.second );
}
