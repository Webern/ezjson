#include "ezjson/JFactory.h"
#include "Value.h"
#include "Doc.h"

namespace ezjson
{
    JDocPtr
    JFactory::makeJDoc()
    {
        return std::make_unique<Doc>();
    }
    
    
    JValueUPtr
    JFactory::makeJValue( JDocPtr& inJDoc )
    {
        return std::make_unique<Value>( inJDoc );
    }
}
