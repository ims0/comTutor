#include<iostream>  
#include<string>  
#include<vector>  
using namespace std;

class StrMatrix{
public:
	unsigned  row, col, size;	
	char*pchar;
	int*pflag;
	StrMatrix(vector<string>&vectstr);
	~StrMatrix();
	void setUsed(unsigned i,unsigned j);
	int notUsed(unsigned i,unsigned j);
	void clearFlag();
	void assignCharArr(vector<string>&);
	char getchar(unsigned i, unsigned j);
	void print();
};

vector<string> split(const string &str, string ch );

int findSingleStr(StrMatrix &charMat, string& curstr);

int findStr(string argvone,string argvtwo);

