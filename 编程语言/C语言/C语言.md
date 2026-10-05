## 关键字|keyword
C 语言的关键字是 C 语言中保留的标识符，它们有特殊的含义，不能用作变量名、函数名等。C 语言的关键字共有 32 个，如下所示：

```c
auto  break  case  char  const  continue  default  do  double  else  enum  extern  float  for  goto  if  int  long  register  return  short  signed  sizeof  static  struct  switch  typedef  union  unsigned  void  volatile  while
```
关键字有几种类型：
1. **控制流关键字**：用于控制程序执行流程，如 `if`、`else`、`else if`、`for`、`while` 等。
2. **类型定义关键字**：用于定义新的数据类型，如 `typedef`、`enum`、`struct`、`union` 等。
3. **存储类关键字**：用于指定变量的存储位置，如 `static`、`extern` 等。
4. **跳转关键字**：用于跳转到其他位置，如 `goto`、`return` 等。
5. **其他关键字**：如 `sizeof`、`volatile` 等。
### 控制流关键字|control flow keyword
控制流关键字是用于控制程序执行流程的关键字。

```c
if  else  else if  for  while
```
### 类型定义关键字|type definition keyword
类型定义关键字是用于定义新的数据类型的关键字。

```c
typedef  enum  struct  union
```
### 存储类关键字|storage class keyword
存储类关键字是用于指定变量的存储位置的关键字。

```c
auto  register  static  extern
```
#### static
`static` 关键字用于声明静态变量或函数。静态变量在函数或代码块中定义，但它们的值在函数或代码块结束后仍然存在，可以在后续的函数或代码块中访问。静态函数只能在定义它的文件中使用。
在编程中经常用来实现全局变量或函数的单例模式。
#### extern
`extern` 关键字用于声明一个全局变量或函数，该变量或函数可以在其他文件中使用。`extern` 关键字告诉编译器该变量或函数在其他地方定义。

#### register
`register` 关键字用于声明一个寄存器变量。寄存器变量在函数调用时会自动保存和恢复，因此在函数内部使用。寄存器变量的存储位置在寄存器中，而不是在栈中。
### 跳转关键字|jump keyword
跳转关键字是用于跳转到其他位置的关键字。

```c
goto  return
```
### 其他关键字|other keyword
其他关键字是用于其他目的的关键字。

```c
sizeof  volatile
```


## 字符串|string
C 语言中的字符串是一个非常基础但也容易出错的概念。与其他高级语言（如 Python 或 Java）不同，C 语言**没有原生的字符串类型**。

在 C 语言中，字符串实际上是**以空字符 `\0`（ASCII 码为 0）结尾的字符数组**。

以下是关于 C 语言字符串的详细讲解和代码示例。

---

### 1. 核心定义：以 `\0` 结尾

这是 C 语言字符串最重要的规则。
*   字符串 `"Hello"` 在内存中实际上占用了 **6** 个字节，而不是 5 个。
*   内存布局：`'H'`, `'e'`, `'l'`, `'l'`, `'o'`, `'\0'`。
*   `\0` 是字符串结束的标志。当打印字符串时，遇到 `\0` 就会停止。

### 2. 两种定义方式

#### 方式一：字符数组
这种方式分配在**栈**（如果是局部变量）或**全局数据区**。内容是**可修改**的。

```c
char str1[] = "Hello";
// 等价于
char str1[] = {'H', 'e', 'l', 'l', 'o', '\0'}; 

str1[0] = 'h'; // 合法：修改数组的第一个元素
```

#### 方式二：字符指针
这种方式通常将字符串字面量存储在**只读存储区**（常量区）。指针 `str2` 只是指向了那个地址。**内容通常不可修改**（尝试修改会导致程序崩溃）。

```c
char *str2 = "World";

// str2[0] = 'w'; // 非法！可能导致段错误，因为 "World" 可能存放在只读内存区
str2 = "New";     // 合法：让指针指向一个新的字符串常量
```

---

### 3. 常用操作函数（`<string.h>`）

C 语言标准库提供了丰富的字符串处理函数，使用时需要包含 `<string.h>`。

| 函数 | 功能 | 注意事项 |
| :--- | :--- | :--- |
| `strlen(s)` | 计算长度（**不**包含 `\0`） | |
| `strcpy(dest, src)` | 将 `src` 复制到 `dest` | **必须确保 `dest` 空间足够大**，否则会溢出 |
| `strncpy(dest, src, n)` | 复制前 n 个字符 | 比 `strcpy` 安全，但注意如果没空间放 `\0`，需手动添加 |
| `strcat(dest, src)` | 将 `src` 拼接到 `dest` 后面 | **必须确保 `dest` 剩余空间足够** |
| `strcmp(s1, s2)` | 比较两个字符串 | 返回 0 表示相等，<0 表示 s1 小于 s2，>0 表示 s1 大于 s2 |
| `strchr(s, c)` | 查找字符 c 在 s 中第一次出现的位置 | 返回指向该字符的指针，找不到返回 NULL |

---

### 4. 代码示例演示

下面的代码涵盖了定义、计算长度、拷贝、比较以及常见的坑。

```c
#include <stdio.h>
#include <string.h> // 必须包含字符串头文件

int main()
{
    // --- 1. 定义与初始化 ---
    char arr[] = "Hello"; // 栈上数组，可修改
    char *ptr = "World";  // 指向常量区，尽量只读

    printf("arr: %s, ptr: %s\n", arr, ptr);

    // --- 2. 获取长度 ---
    // "Hello" 有 5 个字符，不包含 \0
    printf("Length of arr: %zu\n", strlen(arr)); 

    // --- 3. 拷贝 ---
    char dest[20]; // 定义一个足够大的缓冲区
    
    // 安全拷贝：限制拷贝的字节数，防止溢出
    strncpy(dest, arr, sizeof(dest) - 1); 
    dest[sizeof(dest) - 1] = '\0'; // 手动确保结尾是 \0 (好习惯)
    
    printf("Copied: %s\n", dest);

    // --- 4. 拼接 ---
    // 将 ptr 拼接到 dest 后面
    // 注意：必须确保 dest 剩下的空间够放 ptr
    strncat(dest, " ", 1);       // 拼接空格
    strncat(dest, ptr, 19);      // 拼接 ptr
    printf("Concatenated: %s\n", dest);

    // --- 5. 比较 ---
    if (strcmp("apple", "banana") < 0) {
        printf("'apple' is less than 'banana'\n");
    }

    // --- 6. 常见错误演示：缓冲区溢出 ---
    char small_buf[5];
    // strcpy(small_buf, "This is too long"); // 危险！会覆盖相邻内存，可能导致崩溃
    // 正确做法：
    strncpy(small_buf, "This is too long", 4); // 只拷贝4个字符
    small_buf[4] = '\0'; // 封口
    printf("Safe copy result: %s\n", small_buf);

    return 0;
}
```

---

### 5. 常见陷阱与注意事项

1.  **忘记预留 `\0` 的空间**：
    *   如果你要存一个 10 个字符的字符串，数组大小至少要是 **11**。
    *   `char buf[10]; strcpy(buf, "0123456789");` // **错误！** 没空间放 `\0` 了。

2.  **未初始化的字符数组**：
    *   `char buf[10]; strcpy(buf, "hello");` // **危险！** `buf` 内容是随机的（垃圾值），`strcpy` 依赖 `\0` 来判断结束，如果 `buf` 中间碰巧有个 0，拷贝会提前终止；如果没有 0，拷贝会一直向后找，直到越界崩溃。

3.  **字符串常量的修改**：
    *   `char *p = "abc"; p[0] = 'A';` // **崩溃！** 尽量使用 `char p[] = "abc";` 如果需要修改。

4.  **`scanf` 读取字符串**：
    *   `scanf("%s", buf);` 遇到空格或换行符就会停止。
    *   更安全的做法是用 `fgets`，它可以读取空格，并且限制读取长度：
        ```c
        fgets(buf, sizeof(buf), stdin);
        ```
        
## 指针|pointer

### 普通指针|common pointer
```c
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
```
### 指针所占内存大小|pointer size
```c
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
```
### 指针与数组|pointer and array
```c
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
```
## 内存管理|memory management

## 其他|other

### 编码格式切换和终端打印颜色|encoding format switch and terminal print color
```c
#include <stdio.h>

int main()
{
    system("chcp 65001 > null"); // 设置控制台输出编码为UTF-8
    printf("切换至UTF-8编码,避免中文乱码\nHello World!你好\n");
    printf("\033[31m这是红色文字\033[0m\n");
    printf("\033[32m这是绿色文字\033[0m\n");
    printf("\033[33m这是黄色文字\033[0m\n");
    printf("\033[34m这是蓝色文字\033[0m\n");
    printf("\033[35m这是紫色文字\033[0m\n");
    printf("\033[36m这是青色文字\033[0m\n");
    // 组合使用：绿色背景(42)，白色文字(37)
    printf("\033[42;37m 绿底白字 \033[0m\n");
    // 高亮/加粗 (代码 1)
    printf("\033[1;31m这是加粗的红色文字\033[0m\n");
    // 下划线 (代码 4)
    printf("\033[4;32m这是带有下划线的绿色文字\033[0m\n");
    // 反显 (代码 7)
    printf("\033[7;33m这是反显的黄色文字\033[0m\n");
    return 0;
}
```