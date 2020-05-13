#include<iostream>
using namespace std;


template <typename T> class MakeFinal
{
public:
      friend T;
private:
      MakeFinal() {}
      ~MakeFinal() {}
};
 
class FinalClass : virtual public MakeFinal<FinalClass>
{
public:
      FinalClass() {}
      ~FinalClass() {}
};// 具备不能被继承的属性， final
class Try : public FinalClass
{
public:
    Try(){};
    ~Try(){};
};
///
class Base 
{
    Base(){};
    ~Base(){};
};
class Drive :public Base
{
    Drive(){};
    ~Drive(){};
};
int main()
{
    FinalClass obj;
    return 0;
}
