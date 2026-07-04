# SymPy 学习笔记

---

# 1. SymPy 简介

SymPy 是 Python 中最常用的：

```text id="26y8g8"
符号数学库
```

主要用于：

* 代数计算
* 微积分
* 方程求解
* 矩阵运算
* 级数展开
* 极限计算
* 微分方程
* 符号推导

与 NumPy 不同：

```text id="v6yizg"
NumPy 主要做数值计算
SymPy 主要做符号计算
```

例如：

NumPy：

```python id="iwrjlwm"
2 + 3
```

得到：

```python id="h6gmrl"
5
```

而 SymPy：

```python id="pvjlwm"
x + x
```

得到：

```python id="hjlwmv"
2*x
```

因为：

```text id="jlwmrw"
x 是符号
不是数值
```

---

# 2. 安装

## 安装 SymPy

```bash id="8znx8w"
pip install sympy
```

---

## 测试安装

```python id="8kt8wk"
import sympy

print(sympy.__version__)
```

---

# 3. SymPy 的核心思想

SymPy 的核心：

```text id="jlwmg7"
数学表达式对象化
```

例如：

```python id="jlwmm1"
x + y
```

不是普通文本。

而是：

```text id="jlwmj4"
数学表达式对象
```

因此：

可以：

* 化简
* 求导
* 积分
* 求极限
* 求解方程

---

# 4. 定义符号

---

## symbols()

定义数学符号。

```python id="jlwm74"
from sympy import symbols

x = symbols('x')
```

---

## 定义多个符号

```python id="jlwmc5"
x, y, z = symbols('x y z')
```

---

# 5. 创建表达式

```python id="jlwm4u"
from sympy import *

x = symbols('x')

expr = x**2 + 2*x + 1

print(expr)
```

输出：

```python id="jlwm98"
x**2 + 2*x + 1
```

---

# 6. simplify()

化简表达式。

```python id="jlwm8m"
from sympy import *

x = symbols('x')

expr = (x**2 - 1)/(x - 1)

print(simplify(expr))
```

结果：

```python id="jlwmf7"
x + 1
```

---

# 7. expand()

展开表达式。

```python id="jlwmwv"
from sympy import *

x = symbols('x')

expr = (x + 1)**3

print(expand(expr))
```

结果：

```python id="jlwm7y"
x**3 + 3*x**2 + 3*x + 1
```

---

# 8. factor()

因式分解。

```python id="jlwmwo"
from sympy import *

x = symbols('x')

expr = x**2 + 2*x + 1

print(factor(expr))
```

结果：

```python id="jlwm7s"
(x + 1)**2
```

---

# 9. subs()

表达式代入。

---

## 单变量代入

```python id="jlwm8d"
from sympy import *

x = symbols('x')

expr = x**2 + 1

print(expr.subs(x, 2))
```

结果：

```python id="jlwml4"
5
```

---

## 多变量代入

```python id="jlwm3r"
x, y = symbols('x y')

expr = x + y

print(expr.subs({

    x:1,

    y:2
}))
```

结果：

```python id="jlwm95"
3
```

---

# 10. evalf()

数值计算。

```python id="jlwm63"
from sympy import *

x = pi

print(x.evalf())
```

结果：

```python id="jlwmp5"
3.141592653...
```

---

## 指定精度

```python id="jlwm9d"
print(pi.evalf(50))
```

---

# 11. solve()

求解方程。

---

## 一元方程

```python id="jlwmz3"
from sympy import *

x = symbols('x')

eq = x**2 - 4

print(solve(eq, x))
```

结果：

```python id="jlwml8"
[-2, 2]
```

---

## 多元方程组

```python id="jlwm4q"
x, y = symbols('x y')

eq1 = x + y - 3

eq2 = x - y - 1

print(solve(

    (eq1, eq2),

    (x, y)
))
```

结果：

```python id="jlwm7n"
{x:2, y:1}
```

---

# 12. 微分

---

## diff()

求导。

```python id="jlwm9y"
from sympy import *

x = symbols('x')

expr = x**3

print(diff(expr, x))
```

结果：

```python id="jlwm2r"
3*x**2
```

---

## 高阶导数

```python id="jlwm8r"
print(diff(expr, x, 2))
```

结果：

```python id="jlwm1v"
6*x
```

---

# 13. 积分

---

## integrate()

积分。

```python id="jlwm6x"
from sympy import *

x = symbols('x')

expr = x**2

print(integrate(expr, x))
```

结果：

```python id="jlwm6f"
x**3/3
```

---

## 定积分

```python id="jlwmie"
print(

    integrate(

        expr,

        (x,0,1)
    )
)
```

结果：

```python id="jlwmjk"
1/3
```

---

# 14. 极限

---

## limit()

求极限。

```python id="jlwm4k"
from sympy import *

x = symbols('x')

expr = sin(x)/x

print(limit(expr, x, 0))
```

结果：

```python id="jlwmvb"
1
```

---

# 15. 无穷级数

---

## series()

泰勒展开。

```python id="jlwm1c"
from sympy import *

x = symbols('x')

expr = sin(x)

print(series(expr, x, 0, 6))
```

结果：

```python id="jlwm9p"
x - x**3/6 + x**5/120 + O(x**6)
```

---

# 16. 矩阵

---

## Matrix()

创建矩阵。

```python id="jlwm4m"
from sympy import *

A = Matrix([

    [1,2],

    [3,4]
])

print(A)
```

---

## 矩阵乘法

```python id="jlwm8q"
A = Matrix([

    [1,2],

    [3,4]
])

B = Matrix([

    [5,6],

    [7,8]
])

print(A*B)
```

---

## 行列式

```python id="jlwm2m"
print(A.det())
```

---

## 逆矩阵

```python id="jlwmfd"
print(A.inv())
```

---

# 17. 特征值与特征向量

---

## eigenvals()

特征值。

```python id="jlwm0d"
print(A.eigenvals())
```

---

## eigenvects()

特征向量。

```python id="jlwmzt"
print(A.eigenvects())
```

---

# 18. 微分方程

---

## dsolve()

求解微分方程。

```python id="jlwmu4"
from sympy import *

x = symbols('x')

f = Function('f')

eq = Eq(

    f(x).diff(x),

    f(x)
)

print(dsolve(eq))
```

结果：

```python id="jlwmxy"
Eq(f(x), C1*exp(x))
```

---

# 19. Latex 输出

SymPy 可以直接生成 LaTeX。

---

## latex()

```python id="jlwmha"
from sympy import *

x = symbols('x')

expr = integrate(

    sin(x),

    x
)

print(latex(expr))
```

结果：

```python id="jlwmkp"
- \\cos{\\left(x \\right)}
```

---

# 20. pretty_print()

美化输出。

```python id="jlwm53"
from sympy import *

x = symbols('x')

expr = integrate(

    sin(x),

    x
)

pprint(expr)
```

---

# 21. 数学常量

---

## pi

```python id="jlwmq4"
print(pi)
```

---

## E

```python id="jlwm0n"
print(E)
```

---

## oo

无穷大。

```python id="jlwmz9"
print(oo)
```

---

# 22. 常见数学函数

---

## 三角函数

```python id="jlwm3b"
sin(x)

cos(x)

tan(x)
```

---

## 指数函数

```python id="jlwmk2"
exp(x)
```

---

## 对数函数

```python id="jlwm4r"
log(x)
```

---

## 开方

```python id="jlwm7v"
sqrt(x)
```

---

# 23. lambdify()

将 SymPy 表达式转为数值函数。

这是：

```text id="jlwmx8"
SymPy 与 NumPy 的重要桥梁
```

---

## 示例

```python id="jlwmbe"
from sympy import *

x = symbols('x')

expr = x**2 + 1

f = lambdify(

    x,

    expr
)

print(f(3))
```

结果：

```python id="jlwmvn"
10
```

---

# 24. SymPy 与 NumPy 的区别

| 功能   | SymPy | NumPy |
| ---- | ----- | ----- |
| 计算类型 | 符号计算  | 数值计算  |
| 精度   | 理论精确  | 浮点近似  |
| 求导   | 支持    | 不支持   |
| 积分   | 支持    | 不支持   |
| 方程求解 | 支持    | 有限支持  |
| 速度   | 较慢    | 很快    |

---

# 25. 常见应用

---

## 数学教学

* 高数
* 线代
* 微积分

---

## 科研计算

* 符号推导
* 理论验证

---

## 控制系统

* 传递函数
* 拉普拉斯变换

---

## 机器人学

* 运动学推导
* 雅可比矩阵

---

# 26. 常见问题

---

## 1. 忘记定义 symbols

错误：

```text id="jlwmf4"
NameError
```

---

## 2. 使用普通 math 库

错误：

```text id="jlwmj8"
TypeError
```

应使用：

```python id="jlwm8x"
sympy.sin()

而不是

math.sin()
```

---

## 3. 符号与数值混淆

SymPy 的：

```text id="jlwmm8"
x
```

不是普通变量。

而是：

```text id="jlwmt6"
数学符号对象
```

---

# 27. 推荐学习路线

建议顺序：

```text id="jlwm8p"
symbols
↓
表达式
↓
化简
↓
求解方程
↓
微积分
↓
矩阵
↓
微分方程
↓
Latex 输出
```

---

# 28. 推荐练习项目

## 初级

* 方程求解器
* 多项式化简工具

---

## 中级

* 微积分计算器
* 矩阵计算器

---

## 高级

* 自动公式推导
* 控制系统分析
* 机器人运动学推导
* 数学符号计算系统
