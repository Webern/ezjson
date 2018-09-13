
#pragma once

#include <string>

namespace ezjson
{
    inline bool areEqualWithoutWhiteSpace( const std::string& inLeft, const std::string& inRight )
    {
        auto l = std::cbegin( inLeft );
        const auto le = std::cend( inLeft );
        auto r = std::cbegin( inRight );
        const auto re = std::cend( inRight );
        
        const auto isEnd = [&]()
        {
            if( l == le )
            {
                return true;
            }
            
            if( r == re )
            {
                return true;
            }
            
            return false;
        };
        
        for( ; !isEnd(); ++l, ++r )
        {
            while( std::isspace( *l ) && !isEnd() )
            {
                ++l;
            }
            
            while( std::isspace( *r ) && !isEnd() )
            {
                ++r;
            }
            
            if( *l != *r )
            {
                return false;
            }
        }

        while( std::isspace( *l ) && l != le )
        {
            ++l;
        }
        
        while( std::isspace( *r ) && r != re )
        {
            ++r;
        }
        
        if( l != le || r != re )
        {
            return false;
        }
        
        return true;
    }
    
}
