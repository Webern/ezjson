#include "UniqueString.h"

namespace ezjsonprivate
{
    UniqueString::UniqueString()
            : myString{ nullptr }
    {

    }


    UniqueString::UniqueString( const std::string& inString )
            : myString{ std::make_unique<const std::string>( inString ) }
    {

    }


    UniqueString::UniqueString( StringUPtr&& inString )
            : myString{ std::move( inString ) }
    {

    }


    UniqueString::UniqueString( const UniqueString& other )
            : myString{ std::make_unique<const std::string>( *other.get() ) }
    {

    }


    UniqueString::UniqueString( UniqueString&& other ) noexcept
            : myString{ std::move( other.myString ) }
    {

    }


    UniqueString& UniqueString::operator=( const UniqueString& other )
    {
        this->myString = std::make_unique<const std::string>( *other.get() );
        return *this;
    }


    UniqueString& UniqueString::operator=( UniqueString&& other ) noexcept
    {
        this->myString = std::move( other.myString );
        return *this;
    }


    const StringUPtr& UniqueString::get() const
    {
        return myString;
    }


    bool UniqueString::getIsNull() const
    {
        return myString == nullptr;
    }


    void UniqueString::set( StringUPtr&& inString )
    {
        myString = std::move( inString );
    }


    void UniqueString::setNull()
    {
        myString = StringUPtr{ nullptr };
    }


    bool operator<( const UniqueString& left, const UniqueString& right )
    {
        if( !left.getIsNull() && !right.getIsNull() )
        {
            return ( *left.get() ) < ( *right.get() );
        }
        else if( left.getIsNull() && right.getIsNull() )
        {
            return true;
        }

        return false;
    }


    bool operator>( const UniqueString& left, const UniqueString& right )
    {
        return right < left;
    }


    bool operator<=( const UniqueString& left, const UniqueString& right )
    {
        return ( left < right ) || ( left == right );
    }


    bool operator>=( const UniqueString& left, const UniqueString& right )
    {
        return ( left > right ) || ( left == right );
    }


    bool operator==( const UniqueString& left, const UniqueString& right )
    {
        return ( ! ( left < right ) ) || ( ! ( right < left ) );
    }


    bool operator!=( const UniqueString& left, const UniqueString& right )
    {
        return  ( left < right ) || ( right < left );
    }
}
