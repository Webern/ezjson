#include "Doc.h"
#include "Throw.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <iostream>
#include <string>
#include <sstream>
#include <istream>

namespace ezjson
{
    constexpr const auto RJFLAGS =
    rapidjson::kParseDefaultFlags |
//    rapidjson::kParseInsituFlag |
    rapidjson::kParseCommentsFlag |
    rapidjson::kParseTrailingCommasFlag |
    rapidjson::kParseNanAndInfFlag;
    
    Doc::Doc()
    : myRoot{}
    {
        
    }
    
///////////////////////////////////////////////////////////////////
// public /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    
    void
    Doc::loadStream( std::istream& is )
    {
        rapidjson::Document rjdoc;
        rapidjson::BasicIStreamWrapper rjis{ is };
        rjdoc.ParseStream<RJFLAGS>( rjis );
        EZJ_NOT_IMPLEMENTED;
    }
    
    
    void
    Doc::saveStream( std::ostream& os ) const
    {
        EZJ_NOT_IMPLEMENTED;
    }
    
    
    void
    Doc::loadFile( const std::string& filename )
    {
        EZJ_NOT_IMPLEMENTED;
    }
    
    
    void
    Doc::saveFile( const std::string& filename ) const
    {
        EZJ_NOT_IMPLEMENTED;
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
