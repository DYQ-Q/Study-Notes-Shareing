#include <stdio.h>

int main()
{
    system("chcp 65001 > nul");
    printf("\033[32m普通指针|common pointer\033[0m\n");
    int a =10;
    int *p = &a;
    printf("整型变量a:%d,变量a的地址:%p\n",a,&a);
    printf("指针内容(指向变量的地址):%p\n指针指向内容:%d\n指针地址:%p\n",p,*p,&p); //10
    return 0;
}