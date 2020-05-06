#include "stdio.h"
enum EnumType 
{ 
    enumOne, 
    enumTwo, 
    enumThree 
}; 
   
void showMessage(int type) 
{ 
    switch(type) 
    { 
    case enumOne: 
        printf("This is message one\n"); 
        break; 
    case enumTwo: 
        printf("This is message two\n"); 
        break; 
    case enumThree: 
        printf("This is message three\n"); 
        break; 
    default: 
        printf("This is wrong message\n"); 
        break; 
    } 
} 
   
int main() 
{ 
//常规switch 
    showMessage(enumOne); 
    showMessage(enumTwo); 
    showMessage(enumThree); 
   
    return 0; 
} 
