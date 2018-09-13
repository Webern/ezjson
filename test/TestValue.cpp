#include "catch.h"
#include "Value.h"
#include "Doc.h"
#include "compareWithoutWhitespace.h"

#include <sstream>

static constexpr const char* const someJson = "{\"a\":\"hello\",\"b\":true,\"c\":0,\"d\":{},\"x\":[{\"subsub\":0},{\"bones\":\"bishop\"}],\"z\":[{\"silver\":0},{\"bison\":\"elk\"}]}";

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
    CHECK( areEqualWithoutWhiteSpace( expected, actual ) );
}
