/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <iostream>
#include <vector>
#include <map>
#include "val.h"

using std::map;
using std::vector;

using namespace std;
extern void runtime_err(int linenum, string message);
// NodeType represents all possible types
enum NodeType
{
	ERRTYPE,
	INTTYPE,
	STRTYPE
};



// a "forward declaration" for a class to hold values
class Value;

class ParseTree
{
public:

	int linenum;
	ParseTree *left;
	ParseTree *right;

	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree()
	{
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }
	// ParseTree *GetLeft()const{ return left;}
	// ParseTree *GetLeft()const{return right;}
	int Traverse(int input, int (ParseTree::*func)(void))
	{
		if (left)
			input += left->Traverse(input, func);
		if (right)
			input += right->Traverse(input, func);
		return Visit(input, func);
	}

	virtual int Visit(int input, int (ParseTree::*func)(void))
	{
		return input;
	}

	int MaxDepth() const
	{
		int lc = 0, rc = 0;
		if (left)
			lc = left->MaxDepth();
		if (right)
			rc += right->MaxDepth();
		if (lc > rc)
			return lc + 1;
		return rc + 1;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsVar() const { return false; }
	virtual string GetId() const { return ""; }
	virtual int IsBang() const { return 0; }
	virtual bool IsLet() const { return false; }

	int BangCount() const
	{
		int cnt = 0;
		if (left)
			cnt += left->BangCount();
		if (right)
			cnt += right->BangCount();
		cnt += IsBang();
		return cnt;
	}

	int CheckLetBeforeUse(map<string, bool> &var)
	{
		int cnt = 0;
		if (left)
			cnt += left->CheckLetBeforeUse(var);
		if (right)
			cnt += right->CheckLetBeforeUse(var);
		if (IsLet())
			var[this->GetId()] = true;
		if (IsIdent() && var.find(GetId()) == var.end())
		{
			cout << "UNDECLARED VARIABLE " << GetId()<< endl;
			cnt++;
            exit(0);
            
		}
		return cnt;
	}
	virtual Val Eval(map<string, Val> &symbols) const=0;
	
};

class StmtList : public ParseTree
{

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
	virtual Val Eval(map<string, Val> &symbols)const override
	{
		left->Eval(symbols);
		if (right != 0)
			right->Eval(symbols);
		return Val();
	}
};

class Let : public ParseTree
{
	string id;

public:
	Let(Lex &t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

	string GetId() const { return id; }
	bool IsLet() const { return true; }
	virtual Val Eval(map<string, Val> &symbols) const override
	{
		
        symbols[id] = left->Eval(symbols);
		return Val();
	}
};

class Print : public ParseTree
{
public:
	Print(int line, ParseTree *e) : ParseTree(line, e) {}

	virtual Val Eval(map<string, Val> &symbols) const override
	{
		Val results = left->Eval(symbols);
        cout<<results;
		return Val();
	}
};

class Loop : public ParseTree
{

public:
	Loop(int line, ParseTree *e, ParseTree *s) : ParseTree(line, e, s) {}
    virtual Val Eval(map<string, Val>&symbols)const override{
      
       if(left->Eval(symbols).isInt()){
           while(left->Eval(symbols).ValInt() !=0){
                right ->Eval(symbols);
           }
           
       }
       
       
       return Val();
    }
};

class If : public ParseTree
{

public:
	If(int line, ParseTree *e, ParseTree *s) : ParseTree(line, e, s) {}

	virtual Val Eval(map<string, Val> &symbols) const
	{
		Val val = left->Eval(symbols);
        if(!val.isInt()){
            runtime_err(linenum, "Expression is not an integer");
        }
            if (val.ValInt() !=0)
            {
                val = right->Eval(symbols);
            }
        
		return Val();
	}
};

class PlusExpr : public ParseTree
{
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		Val L= left->Eval(symbols);
		if( L.isErr() )
		    runtime_err(linenum, L.GetErrMsg());

		Val R = right->Eval(symbols);
		if( R.isErr() )
		    runtime_err(linenum, R.GetErrMsg());
		Val answer = L + R;
		if( answer.isErr() )
		    runtime_err(linenum, answer.GetErrMsg());
        
		return answer;
	}
};

class MinusExpr : public ParseTree
{
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		Val L = left->Eval(symbols);
		if( L.isErr() )

		    runtime_err(linenum, L.GetErrMsg());

		Val R = right->Eval(symbols);
		if( R.isErr() )

		    runtime_err(linenum, R.GetErrMsg());

		Val answer = L - R;
		if( answer.isErr() )

		    runtime_err(linenum, answer.GetErrMsg());
        
		return answer;
	}
};

class TimesExpr : public ParseTree
{
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		Val L= left->Eval(symbols);
		if( L.isErr() )
		    runtime_err(linenum, L.GetErrMsg());

		Val R = right->Eval(symbols);
		if( R.isErr() )
		    runtime_err(linenum, R.GetErrMsg());
		Val answer = L * R;
		if( answer.isErr() )
		    runtime_err(linenum, answer.GetErrMsg());
        
		return answer;
	}
};

class DivideExpr : public ParseTree
{
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		Val L= left->Eval(symbols);
		if( L.isErr() )
		    runtime_err(linenum, L.GetErrMsg());

		Val R = right->Eval(symbols);
		if( R.isErr() )
		    runtime_err(linenum, R.GetErrMsg());
		Val answer = L /R;
		if( answer.isErr() )
		    runtime_err(linenum, answer.GetErrMsg());
        
		return answer;     
	}
};

class BangExpr : public ParseTree
{
public:
	BangExpr(int line, ParseTree *l) : ParseTree(line, l) {}
	virtual int IsBang() const { return 1; }
	virtual Val Eval(map<string, Val> &symbols)const
	{
		Val bangOp = !left->Eval(symbols);
		return bangOp;
	}
};

class IConst : public ParseTree
{
	int val;

public:
	IConst(Lex &t) : ParseTree(t.GetLinenum())
	{
		val = stoi(t.GetLexeme());
	}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		return Val(val);
	}
};

class SConst : public ParseTree
{
	string val;

public:
	SConst(Lex &t) : ParseTree(t.GetLinenum())
	{
		val = t.GetLexeme();
	}

	virtual Val Eval(map<string, Val> &symbols)const
	{
		return Val(val);
	}
};

class Ident : public ParseTree
{
	string id;

public:
	Ident(Lex &t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }
	virtual Val Eval(map<string, Val> &symbols)const
	{
		if (!symbols.empty() )
		{
			return symbols[id];
		}
        
		return Val();
	}
};

#endif /* PARSETREE_H_ */
