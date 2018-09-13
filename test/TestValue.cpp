#include "catch.h"
#include "Value.h"
#include "Doc.h"

#include <sstream>

TEST_CASE( "Value" )
{
    using namespace ezjson;
    auto doc = std::make_shared<Doc>();
    auto prop = std::make_unique<Value>( doc );
    prop->setName( "a" );
    prop->setValueText( "hello" );
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    prop = std::make_unique<Value>( doc );
    prop->setName( "b" );
    prop->setValueBool( true );
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    prop = std::make_unique<Value>( doc );
    prop->setName( "c" );
    prop->setValueNumber( 1.23 );
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    std::stringstream ss;
    doc->getRoot()->toStream( ss, 0 );
    
    std::cout << ss.str() << std::endl;
}
