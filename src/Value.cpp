#include "Value.h"
#include "Throw.h"
#include <sstream>

namespace ezjson
{
    static constexpr const char* const NOVAL = "this is a nully string";
    
    Value::Value( JDocPtr inJDoc )
    : Value{}
    {
        EZJ_ASSERT( inJDoc != nullptr );
        myDoc = inJDoc;
    }
    
    
    // private constructor
    Value::Value()
    : myDoc{ nullptr }
    , myType{ JValueType::null }
    , myName{ NOVAL }
    , myText{}
    , myNumber{ 0.0 }
    , myBool{ false }
    , myChildren{}
    {
        
    }
    
    
    Value::Value( const Value& inValue )
    : Value{}
    {
        copyOther( inValue );
    }
    
    
    Value&
    Value::operator=( const Value& inOther )
    {
        copyOther( inOther );
        return *this;
    }

///////////////////////////////////////////////////////////////////
// public /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
    
    JValueUPtr
    Value::clone() const
    {
        return std::make_unique<Value>( *this );
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
        EZJ_ASSERT( getIsNamed() );
        return myName;
    }
    
    
    void
    Value::setName( const std::string& inName )
    {
        
        if( false ) //getParent() )
        {
            const auto parentType = getParent()->getType();
            EZJ_ASSERT( parentType == JValueType::array );
            
            if( parentType == JValueType::object )
            {
                const auto propWithSameName = getParent()->getObjectProperty( inName );
                EZJ_ASSERT( propWithSameName == nullptr );
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
        EZJ_ASSERT( getType() == JValueType::object );
        
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
        EZJ_ASSERT( getType() == JValueType::object );
        
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
        EZJ_ASSERT( getType() == JValueType::object );
        
        const auto finder = [&]( const JValueUPtr& inItem )
        {
            return inItem->getName() == inPropertyName;
        };
        
        const auto iter = std::find_if( std::cbegin( myChildren ), std::cend( myChildren ), finder );
        
        if( iter == std::cend( myChildren ) )
        {
            return;
        }
        
        myChildren.erase( iter );
    }
    
    
    JValueVec
    Value::getArrayItems() const
    {
        EZJ_ASSERT( getType() == JValueType::array );
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
        EZJ_NOT_IMPLEMENTED;
    }
    
    
    void
    Value::appendObjectProperty( JValueUPtr&& inJValue )
    {
        EZJ_ASSERT( getType() == JValueType::object );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( inJValue->getIsNamed() );
        const auto iter = findProperty( inJValue->getName() );
        EZJ_ASSERT( iter == std::cend( myChildren ) );
        myChildren.push_back( std::move( inJValue ) );
    }
    
    
    void
    Value::prependObjectProperty( JValueUPtr&& inJValue )
    {
        EZJ_ASSERT( getType() == JValueType::object );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( inJValue->getIsNamed() );
        const auto iter = findProperty( inJValue->getName() );
        EZJ_ASSERT( iter == std::cend( myChildren ) );
        
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
        EZJ_ASSERT( getType() == JValueType::object );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( inJValue->getIsNamed() );
        EZJ_THROW_IF_BAD_VALUE( inPosition, 0, static_cast<int>( myChildren.size() - 1 ) );
        const size_t posSizeT = static_cast<size_t>( inPosition );
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.insert( iter, std::move( inJValue ) );
    }
    
    
    void
    Value::deleteObjectProperty( int inPosition )
    {
        EZJ_ASSERT( getType() == JValueType::object );
        EZJ_THROW_IF_BAD_VALUE( inPosition, 0, static_cast<int>( myChildren.size() - 1 ) );
        const size_t posSizeT = static_cast<size_t>( inPosition );
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.erase( iter );
    }
    
    
    void
    Value::appendArrayItem( JValueUPtr&& inJValue )
    {
        EZJ_ASSERT( getType() == JValueType::array );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( !inJValue->getIsNamed() );
        myChildren.push_back( std::move( inJValue ) );
    }
    
    
    void
    Value::prependArrayItem( JValueUPtr&& inJValue )
    {
        EZJ_ASSERT( getType() == JValueType::array );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( !inJValue->getIsNamed() );
        
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
        EZJ_ASSERT( getType() == JValueType::array );
        EZJ_ASSERT( isSameDoc( inJValue->getDoc() ) );
        EZJ_ASSERT( !inJValue->getIsNamed() );
        EZJ_THROW_IF_BAD_VALUE( inPosition, 0, static_cast<int>( myChildren.size() - 1 ) );
        const size_t posSizeT = static_cast<size_t>( inPosition );
        const auto iter = std::cbegin( myChildren ) + posSizeT;
        myChildren.insert( iter, std::move( inJValue ) );
    }
    
    
    void
    Value::deleteArrayItem( int inPosition )
    {
        EZJ_ASSERT( getType() == JValueType::array );
        EZJ_THROW_IF_BAD_VALUE( inPosition, 0, static_cast<int>( myChildren.size() - 1 ) );
        const size_t posSizeT = static_cast<size_t>( inPosition );
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
                os << ( getIsNamed() ? "" : spaces ) << "{";
                
                if( !myChildren.empty() )
                {
                    os << std::endl;
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
                    
                    os << spaces;
                }

                os << "}";
            }
                break;
                
            case JValueType::array:
            {
                os << ( getIsNamed() ? "" : spaces ) << "[";;
                
                if( !myChildren.empty() )
                {
                    os << std::endl;
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
                    
                    os << spaces;
                }

                os << "]";
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
                EZJ_BUG;
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

    
    std::vector<JValueUPtr>::const_iterator
    Value::findProperty( const std::string& inPropertyName )
    {
        const auto finder = [&]( const JValueUPtr& inItem )
        {
            return inItem->getName() == inPropertyName;
        };
        
        const auto iter = std::find_if( std::cbegin( myChildren ), std::cend( myChildren ), finder );
        
        return iter;
    }
    
    
    bool
    Value::getPropertyExists( const std::string& inPropertyName )
    {
        const auto iter = findProperty( inPropertyName );
        return iter != std::cend( myChildren );
    }
    
    
    void
    Value::copyOther( const Value& inValue )
    {
        clear();
        myDoc = inValue.myDoc;
        myType = inValue.myType;
        myName = inValue.myName;
        myText = inValue.myText;
        myNumber = inValue.myNumber;
        myBool = inValue.myBool;
        
        for( const auto& child : inValue.myChildren )
        {
            myChildren.emplace_back( child->clone() );
        }
    }
}
