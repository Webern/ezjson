#include "Doc.h"

#include <sstream>

namespace ezjson
{
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
        throw "not implemented";
    }
    
    
    void
    Doc::saveStream( std::ostream& os ) const
    {
        throw "not implemented";
    }
    
    
    void
    Doc::loadFile( const std::string& filename )
    {
        throw "not implemented";
    }
    
    
    void
    Doc::saveFile( const std::string& filename ) const
    {
        throw "not implemented";
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
