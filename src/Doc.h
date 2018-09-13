#pragma once

#include "ezjson/JDoc.h"
#include "Value.h"
#include <memory>

namespace ezjson
{
    class Doc : public JDoc
    {
    public:
        Doc();
        ~Doc() = default;
        Doc( const Doc& inValue ) = default;
        Doc( Doc&& inValue ) noexcept = default;
        Doc& operator=( const Doc& inValue ) = default;
        Doc& operator=( Doc&& inValue ) noexcept = default;
        
    public:
        virtual void loadStream( std::istream& is ) override;
        virtual void saveStream( std::ostream& os ) const override;
        virtual void loadFile( const std::string& filename ) override;
        virtual void saveFile( const std::string& filename ) const override;
        virtual JValuePtr getRoot() const override;

    private:
        // TODO - this is bad, fix
        mutable std::unique_ptr<Value> myRoot;
        
    private:
//        bool isSameDoc( const JDocCPtr& inJDoc ) const;
        
    };
}
