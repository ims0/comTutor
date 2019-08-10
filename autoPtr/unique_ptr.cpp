
#include<iostream>
#include <memory>
#include <cstring>
using namespace std;
#define LOG(str) printf("[%s][%s](%d)[%s][%s]:%s\n",__FILE__, __func__, __LINE__,__DATE__, __TIME__, str)

template <typename T, size_t  N>
void printArr(T (&arr)[N])
{
    printf("arr len:%lu\n", N);
    for(unsigned i=0; i<N; ++i)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

auto allocMem(int N)
{
    std::unique_ptr<int []>ret(new int[N]);
    return ret;
}
int main()
{
    int arr[] = { 1,2,3,4,5 };
    printArr(arr);

    auto un_ptr = allocMem(10);
    int *ptr = un_ptr.get();
    cout<<"ptr:"<<ptr<<endl;
    memcpy(ptr,arr,5*sizeof(int));

    for( int i=0 ; i<5 ; ++i )
    {
        cout<<un_ptr[i]<<" ";
    }
    cout<<endl;
    return 0;
}
