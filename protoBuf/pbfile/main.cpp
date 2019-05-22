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
const int phoneNumArrSize=3;
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
	string email;
	PhoneNumber phoneNum[phoneNumArrSize];

}CPerson;

int fillPB(CPerson cPerson[], AddressBook&book, int numOfPerson)
{
	for( int cnt=0 ; cnt<numOfPerson ; cnt++ )
	{
		tutorial::Person* person =book.add_people();
		person->set_id(cPerson[cnt].id);
		person->set_name(cPerson[cnt].name);
		person->set_email(cPerson[cnt].email);

		for( int it=0 ; it<phoneNumArrSize ; it++ )
		{
			Person::PhoneNumber* phone_number = person->add_phonenum();
			phone_number->set_number(cPerson[cnt].phoneNum[it].number);
			phone_number->set_type(Person::MOBILE);
		}
	}
	return 0;
}

int parsePB(string &transStr)
{
	AddressBook addBook;
	addBook.ParseFromString(transStr);

	cout<<"Num of person in AddressBook:"<<addBook.people_size()<<endl;
	for( int index=0; index<addBook.people_size() ; index++ )
	{
		const Person& peopleInfo = addBook.people(index);	

		cout<<"people ID:"<<peopleInfo.id()<<endl;
		cout<<"people name:"<<peopleInfo.name()<<endl;
		cout<<"Size of phone_number:"<<peopleInfo.phonenum_size()<<endl;

		for( int i=0 ; i<peopleInfo.phonenum_size() ; i++ )
		{
			const Person::PhoneNumber&numberItem = peopleInfo.phonenum(i);
			cout<<"PhoneNumber:"<<numberItem.number()<<endl;
		}
	}
}

int main()
{
	int numOfPerson=3;
	CPerson cPerson[numOfPerson];	
	for( int cnt=0 ; cnt<numOfPerson ; cnt++ )
	{
		cPerson[cnt].id = cnt;
		cPerson[cnt].name = "ims";
		cPerson[cnt].email = "optimizes";
		for( int i=0 ; i<phoneNumArrSize ; i++ )
		{
			cPerson[cnt].phoneNum[i].number = "123456";
			cPerson[cnt].phoneNum[i].type= HOME;
		}
	}
	AddressBook addBook;
	int ret = fillPB(cPerson, addBook, numOfPerson);
	string transStr;
	addBook.SerializeToString(&transStr);
	ret = parsePB(transStr);

	book testBook;
	testBook.set_norm(64);

	cout<<testBook.norm()<<endl;
	google::protobuf::UInt64Value val64obj = testBook.num();
	val64obj.set_value(testBook.norm());
	cout<<val64obj.value()<<endl;
	//testBook.num().set_value(testBook.norm());
	//test.tval.value();

	return 0;
}
