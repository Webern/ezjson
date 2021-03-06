// Copyright (c) Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace ezjson
{
    enum class JValueType
    {
        null,
        object,
        array,
        number,
        boolean,
        text,
    };

    class JValue;
    using JValuePtr = JValue*;
    using JValueCPtr = const JValue*;
    using JValueUPtr = std::unique_ptr<JValue>;
    using JValueVec = std::vector<JValueCPtr>;
    
    class JDoc;
    using JDocPtr = std::shared_ptr<JDoc>;
    using JDocCPtr = std::shared_ptr<const JDoc>;
    
    class JValue
    {
    public:
        virtual ~JValue() = default;
        virtual JValueUPtr clone() const = 0;

        virtual JValueType getType() const = 0;
        virtual bool getIsNamed() const = 0;
        virtual std::string getName() const = 0;

        // throws if this jvalue's parent is an array or
        // if this jvalue is the root of the document
        virtual void setName( const std::string& inName ) = 0;

        virtual std::string getValueText() const = 0;
        virtual long double getValueNumber() const = 0;
        virtual bool getValueBool() const = 0;
        virtual JValueVec getObjectProperties() const = 0;
        virtual JValueCPtr getObjectProperty( const std::string& inPropertyName ) const = 0;
        virtual void removeObjectProperty( const std::string& inPropertyName ) = 0;
        virtual JValueVec getArrayItems() const = 0;

        virtual void setValueText( const std::string& inText ) = 0;
        virtual void setValueNumber( long double inNumber ) = 0;
        virtual void setValueBool( bool inBool ) = 0;
        virtual void setIsObject() = 0;
        virtual void setIsArray() = 0;
        virtual void setIsNull() = 0;
        

        // get the JDoc from which this
        // entire json tree descends
        virtual JDocCPtr getDoc() const = 0;

        // get the jvalue that contains this jvalue
        // returns a nullptr if this jvalue is
        // the root of the entire document
        virtual JValueCPtr getParent() const = 0;
        
        // add a jvalue as the last child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void appendObjectProperty( JValueUPtr&& JValueUPtr ) = 0;

        // add a jvalue as the first child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void prependObjectProperty( JValueUPtr&& JValueUPtr ) = 0;

        // add a jvalue as at the given position
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertObjectProperty( int inPosition, JValueUPtr&& inJValue ) = 0;

        // delete a jvalue from this object.
        // throws if this jvalue's type == object
        // throws if the position is out of range
        virtual void deleteObjectProperty( int inPosition ) = 0;

        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void appendArrayItem( JValueUPtr&& inJValue ) = 0;

        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void prependArrayItem( JValueUPtr&& inJValue ) = 0;

        // add a jvalue as at the given position
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertArrayItem( int inPosition, JValueUPtr&& inJValue ) = 0;

        // delete a jvalue from this object.
        // throws if this jvalue's type == array
        // throws if the position is out of range
        virtual void deleteArrayItem( int inPosition ) = 0;

        // if object - removes all properties
        // if array - removes all items
        // if anything else - changes to null type
        virtual void clear() = 0;
        
        virtual void toStream( std::ostream& os, int inNestingLevel, int inPrettyIndent = 2 ) const = 0;
    };
}
