// Copyright (c) Matthew James Briggs

#pragma once

#include "ezjson/JValue.h"

#include <iostream>
#include <memory>

namespace ezjson
{
    class JDoc;
    using JDocPtr = std::shared_ptr<JDoc>;
    using JDocCPtr = std::shared_ptr<const JDoc>;
    
    class JDoc : public std::enable_shared_from_this<JDoc>
    {
    public:
        virtual ~JDoc() {}

        // these can throw std::runtime_error
        virtual void loadStream( std::istream& is ) = 0;
        virtual void saveStream( std::ostream& os ) const = 0;
        
        // these can throw std::runtime_error
        virtual void loadFile( const std::string& filename ) = 0;
        virtual void saveFile( const std::string& filename ) const = 0;
        
        virtual JValuePtr getRoot() const = 0;
    };
}
