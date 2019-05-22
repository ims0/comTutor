/*************************************************************************
    > File Name: main.cpp
    > Author: ims
    > Created Time: 2019年01月09日 星期三 14时45分16秒
 ************************************************************************/
//https://www.jianshu.com/p/d2bed3614259
//https://www.cnblogs.com/Anker/p/3209876.html
//
#include<iostream>
#include"person.pb.h"
using namespace std;
using namespace tutorial;
const int phoneNumArrSize=2;
typedef enum CPhoneType
{
    MOBILE,
    HOME,
    WORK,
}CPhoneType;

typedef struct PhoneNumber 
{
    string number;
    CPhoneType type;
    
}PhoneNumber;

typedef struct CPerson
{
    int id;
    string name;
    const char* p_email;
    char ip[4];
    PhoneNumber phoneNum[phoneNumArrSize];

}CPerson;

int fillPB(CPerson cPerson[], PersonBook&pbBook, int numOfPerson)
{

    for( int cnt=0 ; cnt<numOfPerson ; cnt++ )
    {
        tutorial::PersonList* person =pbBook.add_person_list();
        person->set_id(cPerson[cnt].id);
        person->set_name(cPerson[cnt].name);
        person->set_email(cPerson[cnt].p_email);
        person->set_ip(cPerson[cnt].ip, 4);

        for( int it=0 ; it<phoneNumArrSize ; it++ )
        {
            PersonList::PhoneNumber &phone_number = *person->add_phonenum();
            phone_number.set_number(cPerson[cnt].phoneNum[it].number);
            //C 语言类型为： PhoneNumType_PhoneType
            phone_number.set_num_type((PhoneNumType_PhoneType)cPerson[cnt].phoneNum[it].type);//enum type
        }
    }
    pbBook.PrintDebugString(); 
    return 0;
}

int parsePbAndPrint(string &transStr)
{
    PersonBook addBook;
    bool flag = addBook.ParseFromString(transStr);
    if(!flag)
    {
        puts("\033[32m [ParseFromString] failed! \033[0m");
        return 1;
    }

    cout<<"Num of person in person_list:"<<addBook.person_list_size()<<endl;
    for( int index=0; index<addBook.person_list_size() ; index++ )
    {
        const PersonList& peopleInfo = addBook.person_list(index);    

        cout<<"people ID:"<<peopleInfo.id()<<endl;
        cout<<"people name:"<<peopleInfo.name()<<endl;
        cout<<"people email(bytes):"<<peopleInfo.email()<<",len:"<<peopleInfo.email().length()<<endl;
        cout<<"people ip(bytes):"<<peopleInfo.ip().c_str()<<",ip_len:"<<peopleInfo.ip().length()<<endl;
        cout<<"Size of phone_number:"<<peopleInfo.phonenum_size()<<endl;

        for( int i=0 ; i<peopleInfo.phonenum_size() ; i++ )
        {
            const PersonList::PhoneNumber&numberItem = peopleInfo.phonenum(i);
            cout<<"numberItem.number:"<<numberItem.number()<<endl;
            cout<<"numberItem.num_type:"<<numberItem.num_type()<<endl;
        }
    }
}

int main()
{
    int numOfPerson=2;
    CPerson cPerson[numOfPerson];    
    for( int cnt=0 ; cnt<numOfPerson ; cnt++ )
    {
        cPerson[cnt].id = cnt;
        cPerson[cnt].name = "ims";
        cPerson[cnt].p_email = "optimizes";
        cPerson[cnt].ip[0]= 0x03;
        cPerson[cnt].ip[1]= 0x02;
        cPerson[cnt].ip[2]= 0x00;
        cPerson[cnt].ip[3]= 0x00;
        for( int i=0 ; i<phoneNumArrSize ; i++ )
        {
            cPerson[cnt].phoneNum[i].number = "123456";
            cPerson[cnt].phoneNum[i].type= HOME;
        }
    }
    PersonBook pbPersonBook;
    int ret = fillPB(cPerson, pbPersonBook, numOfPerson);
    string transStr;
    bool flag = pbPersonBook.SerializeToString(&transStr);
    if(!flag)
    {
        puts("\033[32m [SerializeToString] failed! \033[0m");
        return 1;
    }
    ret = parsePbAndPrint(transStr);

    Book testBook;
    // int64 --> google.protobuf.UInt64Value
    testBook.set_int64num(64);
    //这里google.protobuf.UInt64Value 不是基本类型,不能用静态引用直接赋值。
    //testBook.uint64num().set_value(testBook.int64num());
    //应该调用mutable 函数。
    google::protobuf::UInt64Value &int64obj = *testBook.mutable_uint64num();
    int64obj.set_value(9);
    cout<<"test google.protobuf.UInt64Value type:"<<int64obj.value()<<endl;
    cout<<"test google.protobuf.UInt64Value type:"<<testBook.uint64num().value()<<endl;

    return 0;
}
