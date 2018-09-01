#pragma once
#include "UniqueString.h"
#include <map>
#include <vector>
#include <shared_mutex>

namespace ezjsonprivate
{
    using StringPtr = const std::string*;
    using StringMap = std::map<UniqueString, std::vector<StringUPtr>::size_type>;
    using StringMapPair = StringMap::value_type;
    using StringVec = std::vector<StringPtr>;

    class SharedStrings
    {
    public:
        SharedStrings();
        SharedStrings( const SharedStrings& other );
        SharedStrings( SharedStrings&& other );
        SharedStrings& operator=( const SharedStrings& other );
        SharedStrings& operator=( SharedStrings&& other );

        // returns the string at the given index
        // throws if index is out-of-bounds
        const std::string& get( int inIndex ) const;

        // returns the index of the given string value
        // returns -1 if the string equivalent is not found
        int get( const std::string& inString ) const;

        // adds the given string or, if it already exists true
        // if the string was added or false if the string already
        // existed. either way, the index of the string is
        // returned as the second item of the returned pair.
        // of the newly added or existing string.
        std::pair<bool, int> add( const std::string& inString );

        // destroys the string at the given index. careful, anyone
        // holding a reference to the string will have a dangling
        // reference (causing a crash). returns true if the index
        // was found to hold a string, returns false if the index
        // did not hold a string.
        bool destroy( int inIndex );

        // destroys the given (matching) string. careful, anyone
        // holding a reference to the (matching) string will have a
        // dangling reference (causing a crash). returns true if
        // the string was found to exist or false if the string did
        // not exist.
        std::pair<bool, int> destroy( const std::string& inString );

        // destroys all strings and resets the index to 0
        void reset();

    private:
        void copySelf( StringVec& ioStringVec, StringMap& ioStringMap ) const;

    private:
        mutable std::shared_mutex myMutex;
        StringVec myStringVec;
        StringMap myStringMap;
    };
}