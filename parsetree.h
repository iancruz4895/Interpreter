/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "parse.h"
#include "value.h"
#include "tokens.h"

using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };

// a "forward declaration" for a class to hold values
class Value;
static map<string, Value> vars;
void RuntimeError(int line, string);


class ParseTree {
    int            linenum;
public:
    ParseTree    *left;
    ParseTree    *right;
    
public:
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
    : linenum(linenum), left(l), right(r) {}
    
    ParseTree* getLeft() const { return left; }
    ParseTree* getRight() const { return right; }
    
    virtual ~ParseTree() {
        delete left;
        delete right;
    }
    
    int GetLinenum() const { return linenum; }
    
    virtual NodeType GetType() const { return ERRTYPE; }
    
    int LeafCount() const {
        int lc = 0;
        if( left ) lc += left->LeafCount();
        if( right ) lc += right->LeafCount();
        if( left == 0 && right == 0 )
            lc++;
        return lc;
    }
    
    virtual bool IsIdent() const { return false; }
    virtual bool IsString() const { return false; }
    
    virtual string GetId() const { return ""; }
    
    int IdentCount() const {
        int cnt = 0;
        if( left ) cnt += left->IdentCount();
        if( right ) cnt += right->IdentCount();
        if( IsIdent() )
            cnt++;
        return cnt;
    }
    
    int StringCount() const {
        int cnt = 0;
        if( left ) cnt += left->StringCount();
        if( right ) cnt += right->StringCount();
        if( IsString() )
            cnt++;
        return cnt;
    }
    
    void GetVars(map<string,int>& var) {
        if( left ) left->GetVars(var);
        if( right ) right->GetVars(var);
        if( IsIdent() )
            var[ this->GetId() ]++;
    }
    
    virtual Value Eval() {
        return Value();
    }
};

class StmtList : public ParseTree {
    
public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    virtual Value Eval()
    {
        left->Eval();
        if (right)
        {
            right->Eval();
        }
        
        return Value();
    }
};

class IfStatement : public ParseTree {
public:
    IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    virtual Value Eval()
    {
        Value lVal = left->Eval();
        if (lVal.isBoolType())
        {
            if (lVal.getBoolean())
            {
                return right->Eval();
            }
        }
        else
            RuntimeError(this->GetLinenum(), "Invalid IF and THEN expression");
        return Value();
    }
};

class Assignment : public ParseTree {
public:
    Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    virtual Value Eval()
    {
        if (left->IsIdent())
        {
            Value rVal = right->Eval();
            string lVal = left->GetId();
            vars[lVal] = rVal;
            return rVal;
        }
        else
            RuntimeError(this->GetLinenum(), "Invalid variable assignment");
        return Value();
    }
    
};

class PrintStatement : public ParseTree {
public:
    PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    Value Eval() override {
        Value lVal = left->Eval();
        if (lVal.isIntType())
            cout << lVal.getInteger() << endl;
        else if (lVal.isStringType())
            cout << lVal.getString() << endl;
        else if (lVal.isBoolType())
            cout << lVal << endl;
        return Value();
    }
    
    
};

class PlusExpr : public ParseTree {
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() + getRight()->Eval();
    }
};

class MinusExpr : public ParseTree {
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() - getRight()->Eval();
    }
};

class TimesExpr : public ParseTree {
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() * getRight()->Eval();
    }
};

class DivideExpr : public ParseTree {
public:
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() / getRight()->Eval();
    }
};

class LogicAndExpr : public ParseTree {
public:
    LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        Value lVal = getLeft()->Eval();
        Value rVal = getRight()->Eval();
        if (lVal.isBoolType() && rVal.isBoolType())
            return lVal.getBoolean() && rVal.getBoolean();
        else
            RuntimeError(this->GetLinenum()-1, "Cannot do AND Logic");
        return Value();
    }
};

class LogicOrExpr : public ParseTree {
public:
    LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        Value lVal = getLeft()->Eval();
        Value rVal = getRight()->Eval();
        if (lVal.isBoolType() || rVal.isBoolType())
            return lVal.getBoolean() || rVal.getBoolean();
        else
            RuntimeError(this->GetLinenum()-1, "Cannot do OR Logic");
        return Value();
    }
};

class EqExpr : public ParseTree {
public:
    EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() == getRight()->Eval();
    }
};

class NEqExpr : public ParseTree {
public:
    NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() != getRight()->Eval();
    }
};

class LtExpr : public ParseTree {
public:
    LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() < getRight()->Eval();
    }
};

class LEqExpr : public ParseTree {
public:
    LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() <= getRight()->Eval();
    }
};

class GtExpr : public ParseTree {
public:
    GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() > getRight()->Eval();
    }
};

class GEqExpr : public ParseTree {
public:
    GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() override {
        return getLeft()->Eval() >= getRight()->Eval();
    }
};


class IConst : public ParseTree {
    int val;
    
public:
    IConst(int l, int i) : ParseTree(l), val(i) {}
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }
    
    NodeType GetType() const override { return INTTYPE; }
    Value Eval() override {
        return Value(val);
    }
    
};

class BoolConst : public ParseTree {
    bool val;
    
public:
    BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}
    
    NodeType GetType() const override { return BOOLTYPE; }
    Value Eval() override {
        return Value(val);
    }
};

class SConst : public ParseTree {
    string val;
    
public:
    SConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = t.GetLexeme();
    }
    
    NodeType GetType() const override { return STRTYPE; }
    bool IsString() const override { return true; }
    Value Eval() override {
        return Value(val);
    }
};

class Ident : public ParseTree {
    string id;
    
public:
    Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    
    bool IsIdent() const override { return true; }
    string GetId() const override { return id; }
    
    Value Eval() override {
        if (not vars.empty() && vars.count(id))
            return vars[id];
        RuntimeError(this->GetLinenum(), "Unnable to access undefined variable");
        return Value();
    }
    
};

#endif /* PARSETREE_H_ */

