#include <stdio.h>
#include <string.h>

int main()
{
    system("chcp 65001 > nul");
    printf("\033[32m指针所占内存大小|pointer size\033[0m\n");
    int b = 20;int *q = &b;
    float a = 10.0f;float *p = &a;
    double c = 3.14;double *r = &c;
    char d = 'a';char *s = &d;
    char str[] = "hello"; char *t = &str;
    printf("整型指针q的大小:%d\n",sizeof(q));
    printf("浮点型指针p的大小:%d\n",sizeof(p));
    printf("双精度指针r的大小:%d\n",sizeof(r));
    printf("字符型指针s的大小:%d\n",sizeof(s));
    printf("字符串指针t的大小:%d\n",sizeof(t));
    printf("\033[33m指针所占内存的大小与指针指向的数据类型无关,与编译器相关\033[0m\n");
    return 0;
}