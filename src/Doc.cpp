#include "Doc.h"
#include "Throw.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>

namespace ezjson
{
    constexpr const auto RJFLAGS =
    rapidjson::kParseDefaultFlags |
    rapidjson::kParseCommentsFlag |
    rapidjson::kParseTrailingCommasFlag |
    rapidjson::kParseNanAndInfFlag;
    
    Doc::Doc()
    : myRoot{}
    {
        
    }

///////////////////////////////////////////////////////////////////
// hidden /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

    static void buildValueRecursively( JValuePtr inParentNode, const rapidjson::Value& inChildValue, const std::string* const inName );
    static void buildObjectRecursively( JValuePtr inParentNode, const rapidjson::Value::ConstObject& inChildObj, const std::string* const inName );
    static void buildArrayRecursively( JValuePtr inParentNode, const rapidjson::Value::ConstArray& inChildArr, const std::string* const inName );
    
    static void buildValueRecursively( JValuePtr inParentNode, const rapidjson::Value& inChildValue, const std::string* const inName )
    {
        auto value = inParentNode->getDoc()->makeValue();
        
        if( inChildValue.IsObject() )
        {
            value->setIsObject();
            buildObjectRecursively( value.get(), inChildValue.GetObject(), nullptr );
        }
        else if( inChildValue.IsArray() )
        {
            value->setIsArray();
            buildArrayRecursively( value.get(), inChildValue.GetArray(), nullptr );
        }
        else if( inChildValue.IsNull() )
        {
            value->setIsNull();
        }
        else if( inChildValue.IsNumber() )
        {
            value->setValueNumber( inChildValue.GetDouble() );
        }
        else if( inChildValue.IsBool() )
        {
            value->setValueBool( inChildValue.GetBool() );
        }
        else if( inChildValue.IsString() )
        {
            value->setValueText( inChildValue.GetString() );
        }
        else
        {
            EZJ_BUG;
        }
        
        if( inName )
        {
            value->setName( *inName );
        }
        
        if( inParentNode->getType() == JValueType::array )
        {
            inParentNode->appendArrayItem( std::move( value ) );
        }
        else if( inParentNode->getType() == JValueType::object )
        {
            inParentNode->appendObjectProperty( std::move( value ) );
        }
        else
        {
            EZJ_BUG
        }
    }
    
    
    static void buildObjectRecursively( JValuePtr inParentNode, const rapidjson::Value::ConstObject& inChildObj, const std::string* const inName )
    {
        EZJ_ASSERT( inParentNode->getType() == JValueType::object );
        
        for( auto it = inChildObj.MemberBegin(); it != inChildObj.MemberEnd(); ++it )
        {
            const std::string name = it->name.GetString();
            buildValueRecursively( inParentNode, it->value, &( name ) );
        }
    }
    
    
    static void buildArrayRecursively( JValuePtr inParentNode, const rapidjson::Value::ConstArray& inChildArr, const std::string* const inName )
    {
        EZJ_ASSERT( inParentNode->getType() == JValueType::array );
        
        for( auto it = inChildArr.Begin(); it != inChildArr.End(); ++it )
        {
            buildValueRecursively( inParentNode, *it, inName );
        }
    }
    
///////////////////////////////////////////////////////////////////
// public /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    
    
    JValueUPtr
    Doc::makeValue() const
    {
        return std::make_unique<Value>( const_cast<Doc*>( this )->shared_from_this() );
    }
    
    
    void
    Doc::loadStream( std::istream& is )
    {
        rapidjson::Document rjdoc;
        rapidjson::BasicIStreamWrapper rjis{ is };
        rjdoc.ParseStream<RJFLAGS>( rjis );
        const auto rjerror = rjdoc.GetParseError();
        EZJ_ASSERT( rapidjson::ParseErrorCode::kParseErrorNone == rjerror );
        
        if( rjdoc.IsObject() )
        {
            getRoot()->clear();
            getRoot()->setIsObject();
            const rapidjson::Value::ConstObject obj = const_cast<const rapidjson::Document*>( &rjdoc )->GetObject();
            buildObjectRecursively( getRoot(), obj, nullptr );
        }
        else if( rjdoc.IsArray() )
        {
            getRoot()->clear();
            getRoot()->setIsArray();
            const rapidjson::Value::ConstArray arr = const_cast<const rapidjson::Document*>( &rjdoc )->GetArray();
            buildArrayRecursively( getRoot(), arr, nullptr );
        }
        else
        {
            EZJ_THROW( "invalid json" );
        }
    }
    
    
    void
    Doc::saveStream( std::ostream& os ) const
    {
        getRoot()->toStream( os, 0 );
    }
    
    
    void
    Doc::loadFile( const std::string& filename )
    {
        std::ifstream infile{ filename };
        EZJ_ASSERT( infile.is_open() );
        loadStream( infile );
    }
    
    
    void
    Doc::saveFile( const std::string& filename ) const
    {
        std::ofstream ofile{ filename };
        EZJ_ASSERT( ofile.is_open() );
        saveStream( ofile );
    }
    
    
    JValuePtr
    Doc::getRoot() const
    {
        if( !myRoot )
        {
            myRoot = std::make_unique<Value>( const_cast<Doc*>( this )->shared_from_this() );
            myRoot->setIsObject();
        }
        
        // TODO - this is bad
        return const_cast<Value*>( myRoot.get() );
    }
    
    
///////////////////////////////////////////////////////////////////
// private ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    
//    bool
//    Value::isSameDoc( const JDocCPtr& inJDoc ) const
//    {
//        return myDoc.get() == inJDoc.get();
//    }
    
}
