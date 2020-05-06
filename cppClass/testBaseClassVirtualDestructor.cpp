#include <iostream>


using namespace std;

class Base {
    public:
        ~Base() {
            cout << "~Base()" << endl;
        }
};
class VBase {
    public:
        virtual ~VBase() {
            cout << "~Base()" << endl;
        }
};


class Derived1 : public Base {
    public:
        Derived1():name_(new string("NULL")) {}
        Derived1(const string& n):name_(new string(n)) {}

        ~Derived1() {
            delete name_;
            cout << "~Derived1(): name_ has been deleted." << endl;
        }

    private:
        string* name_;
};

class VDerived1 : public VBase {
    public:
        VDerived1():name_(new string("NULL")) {}
        VDerived1(const string& n):name_(new string(n)) {}

        ~VDerived1() {
            delete name_;
            cout << "~Derived1(): name_ has been deleted." << endl;
        }

    private:
        string* name_;
};


int main()
{
    // derived class pointer can call itself class destructor
    Derived1 *d1= new Derived1("d1 obj");
    delete d1;

    // base class pointer can not call derive class destructor when base not virtual destructor
    Base *pBase = new Derived1("d1 obj");
    delete pBase;

    // base class pointer can call derive class destructor when base is virtual destructor
    VBase *pVBase = new VDerived1("d1 obj");
    delete pVBase;
}
