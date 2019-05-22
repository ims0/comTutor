#include <map> 
   
enum EnumType 
{ 
    enumOne, 
    enumTwo, 
    enumThree 
}; 
   
class Base 
{ 
public: 
    Base(){} 
    virtual ~Base(){} 
    virtual void showMessage(){} 
}; 
   
class MessageOne:public Base 
{ 
public: 
    MessageOne(){} 
cout<<"construct"<<endl;
    ~MessageOne(){} 
    void showMessage() 
    { 
        printf("This is message one\n"); 
    } 
}; 
   
class MessageTwo:public Base 
{ 
public: 
    MessageTwo(){} 
    ~MessageTwo(){} 
    void showMessage() 
    { 
        printf("This is message two\n"); 
    } 
}; 
   
class MessageThree:public Base 
{ 
public: 
    MessageThree(){} 
    ~MessageThree(){} 
    void showMessage() 
    { 
        printf("This is message three\n"); 
    } 
}; 
   
int main() 
{ 
//多态+std::map取代switch 
    std::map<int,Base*> baseMap; 
    baseMap.insert(std::make_pair(enumOne,new MessageOne)); 
    baseMap.insert(std::make_pair(enumTwo,new MessageTwo)); 
    baseMap.insert(std::make_pair(enumThree,new MessageThree)); 
    baseMap[enumOne]->showMessage(); 
    baseMap[enumTwo]->showMessage(); 
    baseMap[enumThree]->showMessage(); 
   
    return 0; 
} 
