#include "../include/findstr.h"

using namespace std;
StrMatrix::StrMatrix(vector<string>&vectstr)   
{
	row = vectstr.size();
	if(row>0)
	{
		col = vectstr[0].length();
	}
	else 
		return;
	size = row*col;
	pchar = new char[size];
	pflag= new int[size];
	int trow=0;
	for (string str :vectstr)
	{
		for(int j=0; j<col ; j++)
		pchar[j+trow*col] = str[j];
		trow++;
	}
}
void StrMatrix::clearFlag()
{
	for (unsigned j = 0; j<size; j++) 
	{
		pflag[j] = 1;
	}
}
void StrMatrix::print()
{
	for (unsigned j = 0; j<size; j++) 
	{
		cout<<pchar[j]<<" ";
		if((j+1)%col==0)
			cout<<endl;
	}
}

char StrMatrix::getchar(unsigned i, unsigned j)
{
	if(i<row && j<col)
		return pchar[i*col+j];
	else
		return ' ';
}
StrMatrix::~StrMatrix()
{
	if (pchar != NULL)
	{
		delete[]pchar;
		pchar= NULL;
	}
	if (pflag!= NULL)
	{
		delete[]pflag;
		pflag= NULL;
	}
}
void StrMatrix::setUsed(unsigned i,unsigned j)
{
	if(i<row && j<col)
		pflag[i*col+j]=0;
}
int StrMatrix::notUsed(unsigned i,unsigned j)
{
	if(i<row && j<col)
		return	pflag[i*col+j] ;
	else
		return 0;
}
vector<string> split(const string &str, string ch = " ")
{
	vector<string>ret;
	int pos = 0;
	int start = 0;
	while ((pos = str.find(ch, start) )!= string::npos )
	{
		//cout << "pos:" << pos << endl;
		if (pos>start)
		ret.push_back(str.substr(start, pos-start));
		start = pos+ch.size();
	}
	if (str.size()>start)
	ret.push_back(str.substr(start));
	return ret;
}

int checkPos(int i,int j, int*pos, char ch, int &validnum, StrMatrix &charMat)
{
	if(charMat.notUsed(i,j) && charMat.getchar(i,j) == ch)
	{
		pos[validnum] = i;
		pos[validnum+1] = j;
		validnum+=2;
	}
}

int recursion(int i,int j,StrMatrix  &charMat, string curstr)
{
	int ret=1;
	int pos[6];
    int validnum=0;
	checkPos(i,j+1,pos,curstr[0],validnum,charMat);
	checkPos(i,j-1,pos,curstr[0],validnum,charMat);
	checkPos(i-1,j,pos,curstr[0],validnum,charMat);
	checkPos(i+1,j,pos,curstr[0],validnum,charMat);

	if(validnum>0 && curstr.length()==1)
	{
		//cout<<"has find!"<<endl<<endl;
		return 0;
	}
	for(int cnt=0; cnt<validnum; cnt+=2)
	{
		//cout<<"ch: "<<charMat.getchar(pos[cnt],pos[cnt+1])<<endl;
		charMat.setUsed(pos[cnt],pos[cnt+1]);
		ret = recursion(pos[cnt],pos[cnt+1], charMat, curstr.substr(1));
		if(ret == 0)
			return 0;
	}
	return ret;
}
int findSingleStr(StrMatrix &charMat, string& curstr)
{
	int ret=1;
	for(int i=0; i<charMat.row; i++)
	{
		for(int j=0; j<charMat.col ; j++)
		{
			if(charMat.getchar(i,j) == curstr[0] )
			{
				if(curstr.length()==1)
					return 0;
				cout<<"start char:"<<charMat.getchar(i,j)<<endl;
				charMat.clearFlag();
				charMat.setUsed(i,j);
				ret = recursion(i,j,charMat,curstr.substr(1));	
				if (ret == 0)
				{
				    printf("\033[32;5m");
					cout<<"find str:"<<curstr<<endl;
					printf("\033[0m");
					return 0;
				}
			}
		}
	}
	return ret;
}
int findStr(string argvone,string argvtwo)
{
	vector<string>onemat=split(argvone);
	StrMatrix charMat(onemat);
	printf("\033[33;5m");
	charMat.print();
	printf("\033[0m");
	vector<string>strMatrix=split(argvtwo);

	int ret=0;
	for (string curstr :strMatrix)
	{
		//cout<<"curstr:"<<curstr<<endl;
        ret |= findSingleStr(charMat, curstr );   
	}
	return ret;

}
