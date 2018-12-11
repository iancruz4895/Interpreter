/*
 * value.h
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <iostream>
#include "parsetree.h"
using namespace std;
extern void RuntimeError(int, string);

// object holds boolean, integer, or string, and remembers which it holds
class Value {
    bool    bval;
    int        ival;
    string    sval;
    enum VT { isBool, isInt, isString, isTypeError, isIdent } type;
    
    
public:
    Value() : bval(false), ival(0), type(isTypeError) {}
    Value(bool bval) : bval(bval), ival(0), type(isBool) {}
    Value(int ival) : bval(false), ival(ival), type(isInt) {}
    Value(string sval) : bval(false), ival(0), sval(sval), type(isString) {}
    
    
    
    // in the case of an error, I use the value to hold the error message
    Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}
    
    bool isBoolType() const { return type == VT::isBool; }
    bool isIdentType() const { return type == VT::isIdent; }
    bool isIntType() const { return type == VT::isInt; }
    bool isStringType() const { return type == VT::isString; }
    bool isError() const { return type == VT::isTypeError; }
    bool hasMessage() const { return isError() && sval.size() > 0; }
    bool isSameType(Value that) const { return this->type == that.type; }
    
    bool isTrue() const { return isBoolType() && bval; }
    bool getBoolean() const {
        if( !isBoolType() )
            throw "Not boolean valued";
        return bval;
    }
    
    int getInteger() const {
        if( !isIntType() )
            throw "Not integer valued";
        return ival;
    }
    
    string getString() const {
        if( !isStringType() )
            throw "Not string valued";
        return sval;
    }
    
    string getMessage() const {
        if( !hasMessage() )
            throw "No message";
        return sval;
    }
    
    friend ostream& operator<<(ostream& out, const Value& v) {
        if( v.type == VT::isBool ) out << (v.bval ? "True" : "False");
        else if( v.type == VT::isInt ) out << v.ival;
        else if( v.type == VT::isString ) out << v.sval;
        else if( v.sval.size() > 0 ) out << "RUNTIME ERROR " << v.sval;
        else out << "TYPE ERROR";
        return out;
    }
    
    Value operator+(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt)
        {
            return Value(ival + that.ival);
        }
        
        if (this->type == VT::isString and that.type == VT::isString)
        {
            return Value(sval + that.sval);
        }
        else {
            RuntimeError(0, "Cannot add these two values");
        }
        return Value();
    }
    Value operator-(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival - that.ival);
        } else{
            RuntimeError(0, "Cannot subtract these two values");
        }
        return Value();
    }
    Value operator*(const Value& that) const {
        if (this->type == VT::isString and that.type == VT::isInt) {
            if (that.ival < 0)
                RuntimeError(0, "Cannot multiply negative value to string");
            string str = this->getString();
            int count = that.getInteger();
            string newStr;
            for (unsigned int i = 0; i < count; i++)
                newStr += str;
            return Value(newStr);
        } else if (this->type == VT::isInt and that.type == VT::isString) {
            return that * (*this);
        } else if (this->type == VT::isInt and that.type == VT::isInt) {
            return Value(ival * that.ival);
        } else if (this->type == VT::isInt and that.type == VT::isBool){
            if (this->ival < 0) {
                return Value(not that.bval);
            } else if (this->ival == 0) {
                return Value(false);
            } else {
                return Value(that.bval);
            }
        } else {
            RuntimeError(0, "Tried to multiply with invalid operands");
        }
        return Value();
    }
    Value operator/(const Value& that){
        if (this->type == VT::isInt and (that.type == VT::isInt && that.ival > 0)) {
            return Value(this->ival / that.ival);
        } else if( that.ival == 0){
            RuntimeError(0, "Cannot divide by 0");
            
        } else {
            RuntimeError(0, "Tried to divide with invalid operands");
        }
        return Value();
    }
    Value operator<(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival < that.ival);
        }else if (this->type == VT::isString and that.type == VT::isString){
            return Value(sval < that.sval);
        }else if (this->type == VT::isBool and that.type == VT::isBool){
            return Value(bval < that.bval);
        }else
            RuntimeError(0, "Type mismatch in <");
        return Value();
    }
    Value operator<=(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival <= that.ival);
        }else if (this->type == VT::isString and that.type == VT::isString){
            return Value(sval <= that.sval);
        }else if (this->type == VT::isBool and that.type == VT::isBool){
            return Value(bval <= that.bval);
        }else
            RuntimeError(0, "Type mismatch in <=");
        return Value();
    }
    Value operator>(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival > that.ival);
        }else if (this->type == VT::isString and that.type == VT::isString){
            return Value(sval > that.sval);
        }else if (this->type == VT::isBool and that.type == VT::isBool){
            return Value(bval > that.bval);
        }else
            RuntimeError(0, "Type mismatch in >");
        return Value();
    }
    Value operator>=(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival >= that.ival);
        }else if (this->type == VT::isString and that.type == VT::isString){
            return Value(sval >= that.sval);
        }else if (this->type == VT::isBool and that.type == VT::isBool){
            return Value(bval >= that.bval);
        }else
            RuntimeError(0, "Type mismatch in >=");
        return Value();
    }
    Value operator==(const Value& that){
        if (this->type == VT::isInt and that.type == VT::isInt){
            return Value(ival == that.ival);
        }else if (this->type == VT::isString and that.type == VT::isString){
            return Value(sval == that.sval);
        }else if (this->type == VT::isBool and that.type == VT::isBool){
            return Value(bval == that.bval);
        }else
            RuntimeError(0, "Type mismatch in ==");
        return Value();
    }
    Value operator!=(const Value& v) {
        Value ans = this->operator==(v);
        ans.bval = !ans.bval;
        return ans;
    }
};

#endif /* VALUE_H_ */
