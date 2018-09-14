#pragma once
#include <string>
#include <map>
#include <vector>
#include <shared_mutex>

namespace ezjsonprivate
{
    using StringUPtr = std::unique_ptr<const std::string>;

    class UniqueString
    {
    public:
        UniqueString();
        UniqueString( const std::string& inString );
        UniqueString( StringUPtr&& inString );
        UniqueString( const UniqueString& other );
        UniqueString( UniqueString&& other ) noexcept;
        UniqueString& operator=( const UniqueString& other );
        UniqueString& operator=( UniqueString&& other ) noexcept;

    public:
        const StringUPtr& get() const;
        bool getIsNull() const;
        void set( StringUPtr&& inString );
        void setNull();

    private:
        StringUPtr myString;
    };

    bool operator<( const UniqueString& left, const UniqueString& right );
    bool operator>( const UniqueString& left, const UniqueString& right );
    bool operator<=( const UniqueString& left, const UniqueString& right );
    bool operator>=( const UniqueString& left, const UniqueString& right );
    bool operator==( const UniqueString& left, const UniqueString& right );
    bool operator!=( const UniqueString& left, const UniqueString& right );
}