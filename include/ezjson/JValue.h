// Copyright (c) Matthew James Briggs

#pragma once

#include <memory>
#include <iostream>

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

    class JValue
    {
    public:
        virtual ~JValue() = default;
        virtual JValuePtr clone() const = 0;

        virtual JValueType getType() const = 0;
        virtual bool getIsNamed() const = 0;
        virtual void setIsNamedFalse() = 0;
        virtual std::string getName() const = 0;
        virtual void setName( const std::string& inName ) = 0;

        virtual std::string getValueText() const = 0;
        virtual double getValueNumber() const = 0;
        virtual bool getValueBool() const = 0;
        virtual JValueVec getObjectProperties() const = 0;
        virtual const JValue& getObjectProperty( const std::string& inPropertyName ) const = 0;
        virtual void removeObjectProperty( const std::string& inPropertyName );
        virtual JValueVec getArrayItems() const = 0;

        virtual void setValueText( const std::string& inText ) = 0;
        virtual void setValueNumber( double inNumber ) = 0;
        virtual void setValueBool( bool inBool ) = 0;

        // get the JDoc from which this
        // entire json tree descends
        virtual const JDoc& getDoc() const = 0;

        // get the jvalue that contains this jvalue
        // returns a nullptr if this jvalue is
        // the root of the entire document
        virtual const JValue& getParent() const = 0;
        
        // add a jvalue as the last child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void appendObjectProperty( JValuePtr&& inJValue ) = 0;

        // add a jvalue as the first child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void prependObjectProperty( JValuePtr&& inJValue ) = 0;

        // add a jvalue as at the given position
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertObjectProperty( int inPosition, JValuePtr&& inJValue ) = 0;

        // delete a jvalue from this object.
        // throws if this jvalue's type == object
        // throws if the position is out of range
        virtual void deleteObjectProperty( int inPosition ) = 0;

        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void appendArrayItem( JValuePtr&& inJValue ) = 0;

        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void prependArrayItem( JValuePtr&& inJValue ) = 0;

        // add a jvalue as at the given position
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertArrayItem( int inPosition, JValuePtr&& inJValue ) = 0;

        // delete a jvalue from this object.
        // throws if this jvalue's type == array
        // throws if the position is out of range
        virtual void deleteArrayItem( int inPosition ) = 0;

        // if object - removes all properties
        // if array - removes all items
        // if anything else - changes to null type
        void clear() = 0;
        
        virtual void toStream( std::ostream& os, int inPrettyIndent = 2 ) const = 0;
    };
}
