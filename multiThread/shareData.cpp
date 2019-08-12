/*************************************************************************
    > File Name: shareData.cpp
    > Author: ims
    > Created Time: 2019年08月12日 星期一 22时54分33秒
 ************************************************************************/

#include<iostream>
#include<thread>
#include<vector>
using namespace std;
vector<int> shareData = {
    1, 2, 3
};
void print(int i)
{
    cout<<"thread start running!"<<endl;
    cout<<i<<endl;
    cout<<"thread running end!"<<endl;
}
void write(int i)
{
    shareData.push_back(i);
}
void read()
{
    for( auto iter = shareData.begin()  ; iter != shareData.end() ; ++iter )
    {
        cout<<" "<<*iter;
    }
    cout<<" end!"<<endl;
}
int main()
{
    vector<thread> myth;
    for( int i=0 ; i<5 ; ++i )
    {
        //myth.push_back(thread(write, i+10));
        myth.push_back(thread(read));
    }
    for( auto iter=myth.begin() ; iter != myth.end(); ++iter )
    {
        iter->join();
    }

    
    cout<<"main () end!"<<endl;
    return 0;
}
