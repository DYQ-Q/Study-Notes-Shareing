#include <stdio.h>

int main()
{
    system("chcp 65001 > nul");
    printf("\033[32m指向数组的指针|pointer to array\033[0m\n");
    double arr[5] = {1,2,3,4,5};
    double *p1 = arr;
    printf("数组指针p1的大小:%d,数组大小:%d,数组长度:%d\n",sizeof(p1),sizeof(arr),sizeof(arr)/sizeof(arr[0]));
    printf("数组指针p1:%p\n",p1);
    printf("数组指针p1解引用的值:%lf\n",*p1);
    for(int i=0;i<5;i++)
    {
        printf("a[%d]=%lf,&a[%d]=%p\n",i,*(p1+i),i,p1+i);
    }
    return 0;
}