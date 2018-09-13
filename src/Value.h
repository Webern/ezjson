#pragma once

#include "ezjson/JValue.h"

namespace ezjson
{
    class Value : public JValue
    {
    public:
        Value( JDocPtr inJDoc );
        ~Value() = default;
        Value( const Value& inValue ) = default;
        Value( Value&& inValue ) noexcept = default;
        Value& operator=( const Value& inValue ) = default;
        Value& operator=( Value&& inValue ) noexcept = default;
    
    public:
        virtual JValueUPtr clone() const override;
        
        virtual JValueType getType() const override;
        virtual bool getIsNamed() const override;
        virtual std::string getName() const override;
        
        // throws if this jvalue's parent is an array or
        // if this jvalue is the root of the document
        virtual void setName( const std::string& inName ) override;
        
        virtual std::string getValueText() const override;
        virtual double getValueNumber() const override;
        virtual bool getValueBool() const override;
        virtual JValueVec getObjectProperties() const override;
        virtual JValueCPtr getObjectProperty( const std::string& inPropertyName ) const override;
        virtual void removeObjectProperty( const std::string& inPropertyName ) override;
        virtual JValueVec getArrayItems() const override;
        
        virtual void setValueText( const std::string& inText ) override;
        virtual void setValueNumber( double inNumber ) override;
        virtual void setValueBool( bool inBool ) override;
        
        // get the JDoc from which this
        // entire json tree descends
        virtual JDocCPtr getDoc() const override;
        
        // get the jvalue that contains this jvalue
        // returns a nullptr if this jvalue is
        // the root of the entire document
        virtual JValueCPtr getParent() const override;
        
        // add a jvalue as the last child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void appendObjectProperty( JValueUPtr&& inJValue ) override;
        
        // add a jvalue as the first child of this object
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        virtual void prependObjectProperty( JValueUPtr&& inJValue ) override;
        
        // add a jvalue as at the given position
        // throws if this jvalue's type == object
        // throws if the JValue is not named
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertObjectProperty( int inPosition, JValueUPtr&& inJValue ) override;
        
        // delete a jvalue from this object.
        // throws if this jvalue's type == object
        // throws if the position is out of range
        virtual void deleteObjectProperty( int inPosition ) override;
        
        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void appendArrayItem( JValueUPtr&& inJValue ) override;
        
        // add a jvalue as the last item of this array.
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        virtual void prependArrayItem( JValueUPtr&& inJValue ) override;
        
        // add a jvalue as at the given position
        // throws if this jvalue's type == array
        // throws if the JValue does not have the same JDoc parent
        // throws if the position is out of range
        virtual void insertArrayItem( int inPosition, JValueUPtr&& inJValue ) override;
        
        // delete a jvalue from this object.
        // throws if this jvalue's type == array
        // throws if the position is out of range
        virtual void deleteArrayItem( int inPosition ) override;
        
        // if object - removes all properties
        // if array - removes all items
        // if anything else - changes to null type
        virtual void clear() override;
        
        virtual void toStream( std::ostream& os, int inNestingLevel, int inPrettyIndent = 2 ) const override;
    
    private:
        JDocPtr myDoc;
        JValueType myType;
        std::string myName;
        std::string myText;
        double myNumber;
        bool myBool;
        std::vector<JValueUPtr> myChildren;
    
    private:
        bool isSameDoc( const JDocCPtr& inJDoc ) const;
        
    };
}
