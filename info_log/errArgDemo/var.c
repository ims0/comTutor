#include<stdio.h>
#include<stdarg.h>

void typeLength()
{
    printf("int:%lu, long:%lu, *p:%lu, double:%lu\n",  sizeof(int), sizeof(long), sizeof(int *), sizeof(double));
}
void print(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    size_t val1 = va_arg(ap, size_t);
    double val2 = va_arg(ap, double);
    size_t val3 = va_arg(ap, size_t);
    size_t val4 = va_arg(ap, size_t);

    printf("format:%s \n", format);
    printf("arg hex format v1:%lx, v2:%lf, v3:%lx,v4:%lx\n", val1, val2, val3, val4);
    printf(format, val1, val2, val3);
    printf(format, val1, val3);
    va_end(ap);
}
int main()
{ 
    typeLength();
    int inta =10; 
    double doval = 2.25;
    const char* cstr = "string!";
    size_t ptr_len = (size_t) cstr;
    if(ptr_len > 0xFFFFFFFF)
    {
        printf("ptr len > 32 bit: %lx \n", ptr_len);
    }
    print("val1:%d, val2:%s,  val3: %lf\n",inta ,doval, cstr);
    return 0;
}
