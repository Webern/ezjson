#include "Doc.h"
#include "Throw.h"

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
