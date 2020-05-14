
#include<iostream>
using namespace std;


class Base {
public:
    virtual void f() { cout << "Base::f()" << endl; }
    virtual void g() { cout << "Base::g()" << endl; }
    virtual void h() { cout << "Base::h()" << endl; }
    int var;
};

typedef void(*Fun)(void);


class Base1 {
public:
    virtual void f() { cout << "Base1::f" << endl; }
    virtual void g() { cout << "Base1::g" << endl; }
    virtual void h() { cout << "Base1::h" << endl; }
};

class Base2 {
public:
    virtual void f() { cout << "Base2::f" << endl; }
    virtual void g() { cout << "Base2::g" << endl; }
    virtual void h() { cout << "Base2::h" << endl; }
};

class Base3 {
public:
    virtual void f() { cout << "Base3::f" << endl; }
    virtual void g() { cout << "Base3::g" << endl; }
    virtual void h() { cout << "Base3::h" << endl; }
};

class Derive : public Base1, public Base2, public Base3 {
public:
    virtual void f()override { cout << "Derive::f" << endl; }
    virtual void g1() { cout << "Derive::g1" << endl; }
};

int main()
{
    // no inherit
    Base base;
    if((void*)&base != (void*)&base.var)
    {
        cout<<"vptr int the head of class"<<endl;
    }
    void ***vptrAddr = (void***)&base;
    void **vtblAddr = *vptrAddr;
    void *firstFuncAddr = *vtblAddr;
    printf("addr of vptr of object:%p\n", vptrAddr);
    printf("value of vptr of object:%p\n", vtblAddr);
    printf("addr of first virtual func in class :%p\n", firstFuncAddr);

    Fun f = (Fun) firstFuncAddr;
    Fun g = (Fun) vtblAddr[1];
    Fun h = (Fun) vtblAddr[2];

    f();
    g();
    h();
    // has inherit
    cout << "-- has inherit --"<< endl;
    Derive derive;
    void ***pVTablePtr = (void***)&derive;
    //Base1's vtable
    Fun pFun = (Fun)pVTablePtr[0][0];
    pFun();
    pFun = (Fun)pVTablePtr[0][1];
    pFun();
    pFun = (Fun)pVTablePtr[0][2];
    pFun();
    //Derive's vtable
    pFun = (Fun)pVTablePtr[0][3];
    pFun();
    //The tail of the vtable
    pFun = (Fun)pVTablePtr[0][4];
    cout << "The tail of the vtable:"<< pFun << endl;
    //Base2's vtable
    pFun = (Fun)pVTablePtr[1][0];
    pFun();
    pFun = (Fun)pVTablePtr[1][1];
    pFun();
    pFun = (Fun)pVTablePtr[1][2];
    pFun();
    //The tail of the vtable
    pFun = (Fun)pVTablePtr[1][3];
    cout << "The tail of the vtable:"<< pFun << endl;

    //Base3's vtable
    pFun = (Fun)pVTablePtr[2][0];
    pFun();
    pFun = (Fun)pVTablePtr[2][1];
    pFun();
    pFun = (Fun)pVTablePtr[2][2];
    pFun();
    //The tail of the vtable
    pFun = (Fun)pVTablePtr[2][3];
    cout << "The tail of the vtable:"<< pFun << endl;
    return 0;
}
