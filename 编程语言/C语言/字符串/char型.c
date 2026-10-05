#include <stdio.h>

int main()
{
    system("chcp 65001 > nul");
    char a = 'a';
    char b[10] = "he\0llo";//字符串结束符为'\0',数组长度为2

    printf("字符型变量a:%c,变量a的地址:%p\n",a,&a);
    printf("字符型数组b:%s,数组b的地址:%p,数组长度:%d\n",b,&b,strlen(b));
    for(int i=0;i<strlen(b);i++)
    {
        printf("b[%d]=%c,&b[%d]=%p\n",i,b[i],i,&b[i]);
    }
    return 0;
}