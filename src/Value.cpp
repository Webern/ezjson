#include "Value.h"
#include "Throw.h"
#include <sstream>
#include <iomanip>
#include <cmath>

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
    , myNumber{ PreciseDecimal::MAX_STORAGE_TYPE_DIGITS, PreciseDecimal::MAX_STORAGE_TYPE_DIGITS }
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
    
    
    long double
    Value::getValueNumber() const
    {
        if( getType() != JValueType::number )
        {
            return 0.0;
        }
        
        return myNumber.getValue();
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
    Value::setValueNumber( long double inNumber )
    {
        clear();
        myType = JValueType::number;
        if( inNumber > myNumber.getMaxExpressibleNumber() || inNumber == INFINITY )
        {
            myNumber.setValue( myNumber.getMaxExpressibleNumber() );
        }
        else if( inNumber < myNumber.getMinExpressibleNumber() || inNumber == -1 * INFINITY )
        {
            myNumber.setValue( myNumber.getMinExpressibleNumber() );
        }
        else
        {
            myNumber.setValue( inNumber );
        }
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
    
    
    void
    Value::setIsNull()
    {
        clear();
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
    
    
    inline std::string escape( char c )
    {
        if( c == 8 )
        {
            return std::string{ "\\b" };
        }
        else if( c == 12 )
        {
            return std::string{ "\\f" };
        }
        else if( c == 10 )
        {
            return std::string{ "\\n" };
        }
        else if( c == 13 )
        {
            return std::string{ "\\r" };
        }
        else if( c == 9 )
        {
            return std::string{ "\\t" };
        }
        else if( c == 34 )
        {
            return std::string{ "\\\"" };
        }
        else if( c == 92 )
        {
            return std::string{ "\\\\" };
        }
        
        return std::string{ c };
        
//        Backspace is replaced with \b
//        Form feed is replaced with \f
//        Newline is replaced with \n
//        Carriage return is replaced with \r
//        Tab is replaced with \t
//        Double quote is replaced with \"
//        Backslash is replaced with \\
//
//        Dec  Char                           Dec  Char     Dec  Char     Dec  Char
//        ---------                           ---------     ---------     ----------
//        0  NUL (null)                      32  SPACE     64  @         96  `
//        1  SOH (start of heading)          33  !         65  A         97  a
//        2  STX (start of text)             34  "         66  B         98  b
//        3  ETX (end of text)               35  #         67  C         99  c
//        4  EOT (end of transmission)       36  $         68  D        100  d
//        5  ENQ (enquiry)                   37  %         69  E        101  e
//        6  ACK (acknowledge)               38  &         70  F        102  f
//        7  BEL (bell)                      39  '         71  G        103  g
//        8  BS  (backspace)                 40  (         72  H        104  h
//        9  TAB (horizontal tab)            41  )         73  I        105  i
//        10  LF  (NL line feed, new line)    42  *         74  J        106  j
//        11  VT  (vertical tab)              43  +         75  K        107  k
//        12  FF  (NP form feed, new page)    44  ,         76  L        108  l
//        13  CR  (carriage return)           45  -         77  M        109  m
//        14  SO  (shift out)                 46  .         78  N        110  n
//        15  SI  (shift in)                  47  /         79  O        111  o
//        16  DLE (data link escape)          48  0         80  P        112  p
//        17  DC1 (device control 1)          49  1         81  Q        113  q
//        18  DC2 (device control 2)          50  2         82  R        114  r
//        19  DC3 (device control 3)          51  3         83  S        115  s
//        20  DC4 (device control 4)          52  4         84  T        116  t
//        21  NAK (negative acknowledge)      53  5         85  U        117  u
//        22  SYN (synchronous idle)          54  6         86  V        118  v
//        23  ETB (end of trans. block)       55  7         87  W        119  w
//        24  CAN (cancel)                    56  8         88  X        120  x
//        25  EM  (end of medium)             57  9         89  Y        121  y
//        26  SUB (substitute)                58  :         90  Z        122  z
//        27  ESC (escape)                    59  ;         91  [        123  {
//        28  FS  (file separator)            60  <         92  \        124  |
//        29  GS  (group separator)           61  =         93  ]        125  }
//        30  RS  (record separator)          62  >         94  ^        126  ~
//        31  US  (unit separator)            63  ?         95  _        127  DEL
    }

    
    inline std::string escape( const std::string& inString )
    {
        std::stringstream ss;
        for( const auto& c : inString )
        {
            ss << escape( c );
        }
        
        return ss.str();
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
            os << spaces << "\"" << escape( getName() ) << "\": ";
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
                os << "\"" << escape( getValueText() ) << "\"";
            }
                break;
                
            case JValueType::number:
            {
                os << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << myNumber.toString();
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
