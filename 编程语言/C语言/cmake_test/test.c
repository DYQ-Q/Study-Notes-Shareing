#include <stdio.h>

int main()
{
    
    char a=256+65;//char也是整型，本质是数字，范围0~255，使用%c可以显示对应的ASCII字符
    printf("%c,%c,%c\n",a,321,65);
    return 0;
}