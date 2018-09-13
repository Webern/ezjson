#include "SharedStrings.h"

namespace ezjsonprivate
{

    SharedStrings::SharedStrings()
    : myMutex{}
    , myStringVec{}
    , myStringMap{}
    {

    }


    SharedStrings::SharedStrings( const SharedStrings& other )
    : SharedStrings{}
    {
        other.copySelf( this->myStringVec, this->myStringMap );
    }


    SharedStrings::SharedStrings( SharedStrings&& other )
    : myMutex{}
    , myStringVec{ std::move( other.myStringVec ) }
    , myStringMap{ std::move( other.myStringMap ) }
    {

    }


    SharedStrings&
    SharedStrings::operator=( const SharedStrings& other )
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        this->myStringVec = StringVec{};
        this->myStringMap = StringMap{};
        other.copySelf( this->myStringVec, this->myStringMap );
        return *this;
    }


    SharedStrings&
    SharedStrings::operator=( SharedStrings&& other )
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        this->myStringVec = std::move( other.myStringVec );
        this->myStringMap = std::move( other.myStringMap );
        return *this;
    }


    const std::string&
    SharedStrings::get( int inIndex ) const
    {
        std::shared_lock<std::shared_mutex> lock( myMutex );

        if( inIndex > static_cast<int>( myStringVec.size() - 1 ) )
        {
            throw std::runtime_error{ "index too large " };
        }
        else if( inIndex < 0 )
        {
            throw std::runtime_error{ "index cannot be negative" };
        }

        const auto& item = myStringVec.at( static_cast<size_t>( inIndex ) );

        if( !item )
        {
            throw std::runtime_error{ "the string does not exist" };
        }

        return *item;
    }



    int
    SharedStrings::get( const std::string& inString ) const
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        UniqueString shStr{ inString };
        const auto findIter = myStringMap.find( shStr );

        if( findIter == std::cend( myStringMap ) )
        {
            return -1;
        }

        return findIter->second;
    }


    std::pair<bool, int>
    SharedStrings::add( const std::string& inString )
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        UniqueString shStr{ inString };
        const auto findIter = myStringMap.find( shStr );

        if( findIter == std::cend( myStringMap ) )
        {
            myStringVec.push_back( shStr.get().get() );
            myStringMap.emplace( std::move( shStr ), static_cast<int>( myStringVec.size() - 1 ) );
            return std::make_pair<bool, int>( true, static_cast<int>( myStringVec.size() - 1 ) );
        }

        return std::make_pair<bool, int>( false, findIter->second );
    }


    bool
    SharedStrings::destroy( int inIndex )
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );

        if( inIndex > static_cast<int>( myStringVec.size() - 1 ) )
        {
            return false;
        }
        else if( inIndex < 0 )
        {
            return false;
        }

        const auto& item = myStringVec.at( static_cast<size_t>( inIndex ) );

        if( !item )
        {
            return false;
        }

        UniqueString shStr{ *item };
        const auto findIter = myStringMap.find( shStr );

        if( findIter == std::cend( myStringMap ) )
        {
            throw std::runtime_error{ "myStringMap is in a bad state" };
        }

        myStringVec.at( static_cast<size_t>( inIndex ) ) = nullptr;
        myStringMap.erase( findIter );
        return true;
    }


    std::pair<bool, int>
    SharedStrings::destroy( const std::string& inString )
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        UniqueString shStr{ inString };
        const auto findIter = myStringMap.find( shStr );

        if( findIter == std::cend( myStringMap ) )
        {
            return std::make_pair<bool, int>( false, -1 );
        }

        auto index = findIter->second;
        myStringVec.at( static_cast<size_t>( index ) ) = nullptr;
        myStringMap.erase( findIter );
        return std::make_pair<bool, int>( true, index );
    }


    void
    SharedStrings::reset()
    {
        std::unique_lock<std::shared_mutex> lock( myMutex );
        this->operator=( SharedStrings{} );
    }


    void
    SharedStrings::copySelf( StringVec& ioStringVec, StringMap& ioStringMap ) const
    {
        std::shared_lock<std::shared_mutex> lock( myMutex );

        for( const auto& shStr : myStringMap )
        {
            if( shStr.second < static_cast<int>( ioStringVec.size() ) )
            {
                ioStringVec.resize( static_cast<size_t>( shStr.second ), nullptr );
            }

            ioStringMap.emplace( shStr );
            const auto theNewPair = ioStringMap.find( shStr.first );
            ioStringVec.at( static_cast<size_t>( shStr.second ) ) = theNewPair->first.get().get();
        }
    }
}