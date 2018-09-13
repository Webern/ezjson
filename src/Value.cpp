#include "Value.h"

#include <sstream>

namespace ezjson
{
    static constexpr const char* const NOVAL = "this is a nully string";
    Value::Value( JDocPtr inJDoc )
    : myDoc{ std::move( inJDoc ) }
    , myType{ JValueType::null }
    , myName{ NOVAL }
    , myText{}
    , myNumber{ 0.0 }
    , myBool{ false }
    , myChildren{}
    {
        
    }

///////////////////////////////////////////////////////////////////
// public /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    
    JValueUPtr
    Value::clone() const
    {
        throw "not implemented";
        // std::make_unique<Value>( *this );
    }
    
    
    JValueType
    Value::getType() const
    {
        return myType;
    }
    
    
    bool
    Value::getIsNamed() const
    {
        return myName != NOVAL;
    }
    
    
    std::string
    Value::getName() const
    {
        if( !getIsNamed() )
        {
            throw "";
        }
        
        return myName;
    }
    
    
    void
    Value::setName( const std::string& inName )
    {
        
        if( false ) //getParent() )
        {
            const auto parentType = getParent()->getType();
            
            if( parentType == JValueType::array )
            {
                throw "array items cannot be named";
            }
            else if( parentType == JValueType::object )
            {
                const auto propWithSameName = getParent()->getObjectProperty( inName );
                
                if( propWithSameName != nullptr )
                {
                    throw "a property of the same name already exists on the parent object";
                }
            }
        }
        
        myName = inName;
    }
    
    
    
    std::string
    Value::getValueText() const
    {
        if( getType() != JValueType::text )
        {
            return "";
        }
        
        return myText;
    }
    
    
    double
    Value::getValueNumber() const
    {
        if( getType() != JValueType::number )
        {
            return 0.0;
        }
        
        return myNumber;
    }
    
    
    bool
    Value::getValueBool() const
    {
        if( getType() != JValueType::boolean )
        {
            return false;
        }
        
        return myBool;
    }
    
    
    JValueVec
    Value::getObjectProperties() const
    {
        if( getType() != JValueType::object )
        {
            return JValueVec{};
        }
        
        JValueVec vec;
        
        for( const auto& prop : myChildren )
        {
            vec.push_back( prop.get() );
        }
        
        return vec;
    }
    
    
    JValueCPtr
    Value::getObjectProperty( const std::string& inPropertyName ) const
    {
        if( getType() != JValueType::object )
        {
            return nullptr;
        }
        
        const auto finder = [&]( const JValueUPtr& inItem )
        {
            return inItem->getName() == inPropertyName;
        };
        
        const auto iter = std::find_if( std::cbegin( myChildren ), std::cend( myChildren ), finder );
        
        if( iter == std::cend( myChildren ) )
        {
            return nullptr;
        }
        
        return iter->get();
    }
    
    
    void
    Value::removeObjectProperty( const std::string& inPropertyName )
    {
        if( getType() != JValueType::object )
        {
            // throw?
            return;
        }
        
        const auto finder = [&]( const JValueUPtr& inItem )
        {
            return inItem->getName() == inPropertyName;
        };
        
        const auto iter = std::find_if( std::cbegin( myChildren ), std::cend( myChildren ), finder );
        
        if( iter == std::cend( myChildren ) )
        {
            // throw?
            return;
        }
        
        myChildren.erase( iter );
    }
    
    
    JValueVec
    Value::getArrayItems() const
    {
        if( getType() != JValueType::array )
        {
            return JValueVec{};
        }
        
        JValueVec vec;
        
        for( const auto& item : myChildren )
        {
            vec.push_back( item.get() );
        }
        
        return vec;
    }
    
    
    void
    Value::setValueText( const std::string& inText )
    {
        clear();
        myType = JValueType::text;
        myText = inText;
    }
    
    
    void
    Value::setValueNumber( double inNumber )
    {
        clear();
        myType = JValueType::number;
        myText = inNumber;
    }
    
    
    void
    Value::setValueBool( bool inBool )
    {
        clear();
        myType = JValueType::boolean;
        myBool = inBool;
    }
    
    
    void
    Value::setIsObject()
    {
        if( myType == JValueType::object )
        {
            return;
        }
        
        clear();
        myType = JValueType::object;
    }
    
    
    void
    Value::setIsArray()
    {
        if( myType == JValueType::array )
        {
            return;
        }
        
        clear();
        myType = JValueType::array;
    }
    
    
    JDocCPtr
    Value::getDoc() const
    {
        return myDoc;
    }
    
    
    JValueCPtr
    Value::getParent() const
    {
        throw "not implemented";
    }
    
    
    void
    Value::appendObjectProperty( JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::object )
        {
            throw "object properties can only be added to objects";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( !inJValue->getIsNamed() )
        {
            throw "object properties must have names";
        }
        
        myChildren.push_back( std::move( inJValue ) );
    }
    
    
    void
    Value::prependObjectProperty( JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::object )
        {
            throw "object properties can only be added to objects";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( !inJValue->getIsNamed() )
        {
            throw "object properties must have names";
        }
        
        if( myChildren.empty() )
        {
            myChildren.push_back( std::move( inJValue ) );
        }
        else
        {
            myChildren.insert( std::cbegin( myChildren ),  std::move( inJValue ) );
        }
    }
    
    
    void
    Value::insertObjectProperty( int inPosition, JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::object )
        {
            throw "object properties can only be added to objects";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( !inJValue->getIsNamed() )
        {
            throw "object properties must have names";
        }
        
        if( inPosition < 0 )
        {
            throw "position is out of bounds (negative)";
        }
        
        const size_t posSizeT = static_cast<size_t>( inPosition );
        
        if( posSizeT > myChildren.size() - 1 )
        {
            throw "position is out of bounds (too large)";
        }
        
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.insert( iter, std::move( inJValue ) );
    }
    
    
    void
    Value::deleteObjectProperty( int inPosition )
    {
        if( getType() != JValueType::object )
        {
            throw "object properties can only be deleted from objects";
        }
        
        if( inPosition < 0 )
        {
            throw "position is out of bounds (negative)";
        }
        
        const size_t posSizeT = static_cast<size_t>( inPosition );
        
        if( posSizeT > myChildren.size() - 1 )
        {
            throw "position is out of bounds (too large)";
        }
        
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.erase( iter );
    }
    
    
    void
    Value::appendArrayItem( JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::array )
        {
            throw "array items can only be added to arrays";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( inJValue->getIsNamed() )
        {
            throw "array items cannot be named";
        }
        
        myChildren.push_back( std::move( inJValue ) );
    }
    
    
    void
    Value::prependArrayItem( JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::array )
        {
            throw "array items can only be added to arrays";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( inJValue->getIsNamed() )
        {
            throw "array items cannot be named";
        }
        
        if( myChildren.empty() )
        {
            myChildren.push_back( std::move( inJValue ) );
        }
        else
        {
            myChildren.insert( std::cbegin( myChildren ),  std::move( inJValue ) );
        }
    }
    
    
    void
    Value::insertArrayItem( int inPosition, JValueUPtr&& inJValue )
    {
        if( getType() != JValueType::array )
        {
            throw "array items can only be added to arrays";
        }
        
        if( !isSameDoc( inJValue->getDoc() ) )
        {
            throw "incoming jvalue must be created with the same jdoc";
        }
        
        if( inJValue->getIsNamed() )
        {
            throw "array items cannot be named";
        }
        
        if( inPosition < 0 )
        {
            throw "position is out of bounds (negative)";
        }
        
        const size_t posSizeT = static_cast<size_t>( inPosition );
        
        if( posSizeT > myChildren.size() - 1 )
        {
            throw "position is out of bounds (too large)";
        }
        
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.insert( iter, std::move( inJValue ) );
    }
    
    
    void
    Value::deleteArrayItem( int inPosition )
    {
        if( getType() != JValueType::array )
        {
            throw "array items can only be deleted from arrays";
        }
        
        if( inPosition < 0 )
        {
            throw "position is out of bounds (negative)";
        }
        
        const size_t posSizeT = static_cast<size_t>( inPosition );
        
        if( posSizeT > myChildren.size() - 1 )
        {
            throw "position is out of bounds (too large)";
        }
        
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.erase( iter );
    }
    
    
    void
    Value::clear()
    {
        myType = JValueType::null;
        myText = NOVAL;
        myNumber = 0.0;
        myBool = false;
        myChildren.clear();
    }
    
    
    void
    Value::toStream( std::ostream& os, int inNestingLevel, int inPrettyIndent ) const
    {
        std::stringstream spacesSs;
        
        if( inPrettyIndent >= 0 )
        {
            const auto numSpaces = inPrettyIndent * inNestingLevel;
            
            for( int i = 0; i < numSpaces; ++i )
            {
                spacesSs << " ";
            }
        }
        
        const std::string spaces = spacesSs.str();
        
        if( getIsNamed() )
        {
            os << spaces << "\"" << getName() << "\": ";
        }
        
        switch ( getType() )
        {
            case JValueType::object:
            {
                os << spaces << "{" << std::endl;
                
                size_t current = 0;
                
                for( const auto& prop : myChildren )
                {
                    prop->toStream( os, inNestingLevel + 1, inPrettyIndent );
                    
                    if( current < myChildren.size() - 1 )
                    {
                        os << ",";
                    }
                    
                    os << std::endl;
                    ++current;
                }
                
                os << spaces << "}";
            }
                break;
                
            case JValueType::array:
            {
                os << spaces << "[" << std::endl;
                
                size_t current = 0;
                
                for( const auto& prop : myChildren )
                {
                    prop->toStream( os, inNestingLevel + 1, inPrettyIndent );
                    
                    if( current < myChildren.size() - 1 )
                    {
                        os << ",";
                    }
                    
                    os << std::endl;
                    ++current;
                }
                
                os << spaces << "]";
            }
                break;
                
            case JValueType::text:
            {
                os << "\"" << getValueText() << "\"";
            }
                break;
                
            case JValueType::number:
            {
                os << getValueNumber();
            }
                break;
                
            case JValueType::boolean:
            {
                if( getValueBool() )
                {
                    os << "true";
                }
                else
                {
                    os << "false";
                }
            }
                break;
                
            case JValueType::null:
            {
                os << "null";
            }
                break;
                
            default:
                throw "should never be here";
                break;
        }
    }


///////////////////////////////////////////////////////////////////
// private ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

    bool
    Value::isSameDoc( const JDocCPtr& inJDoc ) const
    {
        return myDoc.get() == inJDoc.get();
    }

}
