#include "../include/findstr.h"
int main(int argc, char* argv[])
{
	if(argc !=3)
	{
		cout<<"argc should is 3, now it is "<<argc<<endl;
		return 1;
	}
	findStr(argv[1],argv[2]);
	return 0;
}
