#ifndef VAL_H
#define VAL_H

#include <algorithm>
#include <string>
using namespace std;

class Val
{
    int i;
    string s;
    enum ValType
    {
        ISINT,
        ISSTR,
        ISERR
    } vt;

public:
    Val() : i(0), vt(ISERR) {}
    Val(int i) : i(i), vt(ISINT) {}
    Val(string s) : i(0), s(s), vt(ISSTR) {}
    Val(ValType vt, string errmsg) : i(0), s(errmsg), vt(ISERR) {}

    ValType getVt() const { return vt; }

    bool isErr() const { return vt == ISERR; }
    bool isInt() const { return vt == ISINT; }
    bool isStr() const { return vt == ISSTR; }

    int ValInt() const
    {
        if (isInt())
            return i;
        throw "This Val is not an Int";
    }
    string ValString() const
    {
        if (isStr())
            return s;
        throw "This Val is not a Str";
    }

    friend ostream &operator<<(ostream &out, const Val &v)
    {
        if (v.vt == ValType::ISINT)
            out << v.i;
        if (v.vt == ValType::ISSTR)
            out << v.s;
        return out;
    }

    string GetErrMsg() const
    {
        if (isErr())
            return s;
        throw "This Val is not an Error";
    }

    Val operator+(const Val &op) const
    {
        if (isInt() && op.isInt())
            return ValInt() + op.ValInt();
        if (isStr() && op.isStr())
            return ValString() + op.ValString();
        return Val(ISERR, "Type mismatch on operands of +");
    }

    Val operator-(const Val &op) const
    {
        if (isInt() && op.isInt())
        {
            return ValInt() - op.ValInt();
        }

        return Val(ISERR, "Type mismatch on operands of -");
    }

    Val operator*(const Val &op) const
    {
        if (isInt() && op.isInt())
        {
            return Val(ValInt() * op.ValInt());
        } 
         else if (isStr() && op.isInt())
        {
            if (op.ValInt() > 0)
            {
                string str = "";
                for (int i = 0; i < op.ValInt(); i++)
                {
                    str = str + ValString();
                }
                return str;
            }
            else if (op.ValInt() == 0)
            {
                string str = "";
                return str;
            }
            else
            {
                //Add runtime error;
                return Val(ISERR, "Cannot multiply string by negative int");
            }
        }
        else if (isInt() && op.isStr())
        {
            if (ValInt() >= 0)
            {

                string str = "";
                for (int i = 0; i < ValInt(); i++)
                {
                    str = str + op.ValString();
                }
                return str;
            }
            else if (op.ValInt() == 0)
            {
                string str = "";
                return str;
            }
            else
            {
                return Val(ISERR, " Expression is not an integer");
            }
        }
        
        return Val(ISERR, "Type mismatch on operands of *");
    }

    Val operator/(const Val &op) const
    {
        if (isInt() && op.isInt())
        {
            if(op.ValInt() !=0){
                return ValInt() / op.ValInt();
            }
            else{
              return Val(ISERR, "Divide by zero error");
            }  
        }
        return Val(ISERR, "Type mismatch on operands of /");
          
    }

    Val operator!() const
    {
        if (isInt())
        {
           int num =ValInt();
           int reverse =0 ;
           int remainder;
           while(num !=0){
               remainder = num %10;
               reverse = reverse *10 + remainder;
               num= num/10;
           }
           return reverse;
            
        }
        if (isStr())
        {
            int len = s.size() - 1;
            string temp = s;
            
            for (int i = len; i >= 0; i--)
            {
                temp[len - i] = s[i];
            }
            return temp;
        }
        return Val(ISERR, "Type mismatch on operands of !");
    }
};
#endif