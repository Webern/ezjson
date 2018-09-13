#include "catch.h"
#include "UniqueString.h"

TEST_CASE( "UniqueString" )
{
    using namespace ezjsonprivate;

    UniqueString uqStr{};
    CHECK( true == uqStr.getIsNull() );

    uqStr = UniqueString{ "test" };
    REQUIRE( false == uqStr.getIsNull() );
    REQUIRE( uqStr.get() );
    CHECK( "test" == *( uqStr.get().get() ) );

    UniqueString cpStr = uqStr;
    REQUIRE( false == cpStr.getIsNull() );
    REQUIRE( cpStr.get() );
    CHECK( cpStr.get().get() != uqStr.get().get() );
}
