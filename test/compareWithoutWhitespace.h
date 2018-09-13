
#pragma once

#include <string>
#include <sstream>

namespace ezjson
{
    inline std::string areEqualWithoutWhiteSpace( const std::string& inLeft, const std::string& inRight )
    {
        int lpos = 0;
        auto l = std::cbegin( inLeft );
        const auto le = std::cend( inLeft );
        int rpos = 0;
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
        
        for( ; !isEnd(); ++l, ++lpos, ++r, ++rpos )
        {
            while( std::isspace( *l ) && !isEnd() )
            {
                ++l;
                ++lpos;
            }
            
            while( std::isspace( *r ) && !isEnd() )
            {
                ++r;
                ++rpos;
            }
            
            if( *l != *r )
            {
                std::stringstream message;
                message << "strings were not found to be equal ";
                message << "at left string position " << lpos << " and right string position " << rpos << ".";
                message << "  '" << *l << "' != '" << *r << "'.";
                message << std::endl << "left:" << std::endl;
                message << inLeft;
                message << std::endl << "right:" << std::endl;
                message << inRight;
                return message.str();
            }
        }

        while( std::isspace( *l ) && l != le )
        {
            ++l;
            ++lpos;
        }
        
        while( std::isspace( *r ) && r != re )
        {
            ++r;
            ++rpos;
        }
        
        if( l != le || r != re )
        {
            std::stringstream message;
            message << "strings were not found to be equal ";
            message << "at left string position " << lpos << " and right string position " << rpos << ".";
            message << " one of the strings is at the end while the other is not.";
            message << std::endl << "left:" << std::endl;
            message << inLeft;
            message << std::endl << "right:" << std::endl;
            message << inRight;
            return message.str();
        }
        
        return "";
    }
    
}
