#include "catch.h"
#include "Value.h"
#include "Doc.h"
#include "compareWithoutWhitespace.h"

#include <sstream>

constexpr const char* const someJson = R"json({"a":"hello","b":true,"c":1.23,"d":{},"x":[{"subsub":3.45},{"bones":"bishop"}],"z":[{"silver":3.45},{"bison":"elk"}]})json";

TEST_CASE( "Value - toStream" )
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
    
    prop = std::make_unique<Value>( doc );
    prop->setName( "d" );
    prop->setIsObject();
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    
    prop = std::make_unique<Value>( doc );
    prop->setName( "x" );
    prop->setIsArray();
    auto subval = std::make_unique<Value>( doc );
    subval->setIsObject();
    auto subsubval = std::make_unique<Value>( doc );
    subsubval->setName( "subsub" );
    subsubval->setValueNumber( 3.45 );
    subval->appendObjectProperty( std::move( subsubval ) );
    prop->appendArrayItem( std::move( subval ) );
    subval = std::make_unique<Value>( doc );
    subval->setIsObject();
    subsubval = std::make_unique<Value>( doc );
    subsubval->setName( "bones" );
    subsubval->setValueText( "bishop" );
    subval->appendObjectProperty( std::move( subsubval ) );
    prop->appendArrayItem( std::move( subval ) );
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    
    prop = std::make_unique<Value>( doc );
    prop->setName( "z" );
    prop->setIsArray();
    subval = std::make_unique<Value>( doc );
    subval->setIsObject();
    subsubval = std::make_unique<Value>( doc );
    subsubval->setName( "silver" );
    subsubval->setValueNumber( 3.45 );
    subval->appendObjectProperty( std::move( subsubval ) );
    prop->appendArrayItem( std::move( subval ) );
    subval = std::make_unique<Value>( doc );
    subval->setIsObject();
    subsubval = std::make_unique<Value>( doc );
    subsubval->setName( "bison" );
    subsubval->setValueText( "elk" );
    subval->appendObjectProperty( std::move( subsubval ) );
    prop->appendArrayItem( std::move( subval ) );
    doc->getRoot()->appendObjectProperty( std::move( prop ) );
    
    std::stringstream ss;
    doc->getRoot()->toStream( ss, 0 );
    
    const std::string expected = someJson;
    const std::string actual = ss.str();
    CHECK( areEqualWithoutWhiteSpace( expected, actual ) == "" );
}

TEST_CASE( "Value - doc parse" )
{
    using namespace ezjson;
    auto doc = std::make_shared<Doc>();
    const std::string expected = someJson;
    
    std::istringstream is{ expected };
    doc->loadStream( is );
    
    std::stringstream os;
    doc->getRoot()->toStream( os, 0 );
    
    const std::string actual = os.str();
    CHECK( areEqualWithoutWhiteSpace( expected, actual ) == "" );
}
