# include <iostream>
# include <fstream>
#include "lex.h"
#include "parse.h"

using namespace std;

map<string, Val> symbols;
map<string, bool> testUnd;

 void runtime_err(int linenum, string message) {
    
   // cout<< "RUNTIME ERROR at " << linenum << ": " << message << endl;
     string err = "RUNTIME ERROR at " + std::to_string(linenum) + ": "+ message;
     throw std::string(err);

}

int main(int argc, char *argv[])
{
    
	ifstream file;
	istream *in;
	int linenum = 0;

	if( argc == 1 ) {
		in = &cin;
	}
    
	else if( argc == 2 ) {
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cout << "COULD NOT OPEN " << argv[1] << endl;
			return 1;
		}
		in = &file;
	}

	else {
		cout << "TOO MANY FILENAMES" << endl;
		return 1;
	}

	ParseTree *prog = Prog(*in, linenum);

	if( prog == 0 )
    {
		return 0;
    }
    try {
        prog->CheckLetBeforeUse(testUnd);
        prog->Eval(symbols);
    }
    catch(std:: string& e) {
        cout << e << endl;
        
    } 
    
    return 0;
}