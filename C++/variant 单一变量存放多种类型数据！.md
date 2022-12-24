# `std::variant` | 单一变量存放多种类型数据！

在 **`C++17`** 被正式引入，被包含在 `<variant>`。

它实际上不是一个真正的特性，而是一个 `C++17` 引入的一个新的类！

LNK：`C++17` 给我们带来的三个类：  [`std::optional` ](optional 处理OPTIONAL数据.md)、`std::variant` 、[`std::`any](any 存储任意类型的数据.md)



## 为什么 `std::variant` ？

可以用来处理 [^1]**可能存在、也可能不存在的数据** ，抑或是[^2]**单纯储存多种数据**。

`std::variant` 的使用可以让我们**不用担心处理的确切数据类型**。

e.g. 你得到一个命令行指令，有一些参数你并不确定它会是整型或是字符或是布尔，你只想先接收它，再来处理。

`std::variant` 允许你列出所有的可能类型，然后接收一些不确定类型的数据。



## `std::variant` 的使用

### 定义、赋值、输出

```cpp
std::variant<Args,.../*列举所有可能的类型*/> data; //定义
std::cout << std::get<knownType/*已知类型*/>(data) << std::endl; //如果你想cout一个variant的话
```

------

这里有一个例子：

```cpp
//variant
#include<iostream>
#include<variant>
#include<string>

int main(){
    std::variant<std::string, int> data;
    data = "DvdBr3o";
    std::cout << std::get<std::string>(data) << std::endl;
    data = 2;
    std::cout << std::get<int>(data) << std::endl;
}
```

输出：

```
DvdBr3o
2
```

非常 amazing 啊~

------



### 类型检测&类型安全

in last case，假设我们有时候把类型对应搞砸了，

比如我们把 `data` 赋给了一个整型值，却把他当作 `std::string` 用 `std::get` 输出会怎么样？

```cpp
//What I mean...
data = 2;
std::cout << std::get<std::string>(data) << std::endl;
```

**报错 `std::bad_variant_access`** 。

你当然可以用 `try` & `catch` 来解决这个问题，`std::variant` 有他自己配套的办法。



##### + `.index() ` 获取类型序号

in this case，

```cpp
std::variant<std::string, int> data;
data = "DvdBr3o";
std::cout << data.index() << std::endl;
data = 2;
std::cout << data.index() << std::endl;
```

输出：

```
0
1
```

事实上，`.index()` 就是把 `std::variant<...>` 尖括号内的类型都**看作一个 `enum` 集**，且**首项编号为 `0`**，然后**返回该变量类型的 `enum` 编号**。



##### + `.get_if<类型名>(对象指针)` 控制类型分支处理

`.get_if` 是这样工作的：

首先，`.get_if` **获取到对象指针**，

然后，把对象类型与给定类型进行**对比**，

如果**类型一致**，则返回**对象指针**，

如果**类型不一致**，则返回一个**空指针**。

很好记，`.get_if` 就是 `if<>()` 的话就 `get` 对象的指针。

in this case，

```cpp
//用.get_if控制类型分支
#include<iostream>
#include<variant>
#include<string>

int main(){
    std::variant<std::string, int> data;
    data = 2;
    if(auto val = std::get_if<std::string>(&data))
        *val += "Br3o";
    else
        *val += 1;
}

```



## `std::variant`    vs.    联合体 `union`

### 数据储存

我们用 `sizeof()` 来检查一下同等情况下两者的储存大小：

```cpp
//Check size of Variant & Union
#include<iostream>
#include<variant>
#include<string>

int main(){
    std::variant<double, int> data;
    union {
        double a;
        int b;
    };

    std::cout << "Size of double     : " << sizeof(double) << std::endl;
    std::cout << "Size of int        : " << sizeof(int)    << std::endl;
    std::cout << "==================================="     << std::endl;
    std::cout << "Size of variant    : " << sizeof(data)   << std::endl;
    std::cout << "==================================="     << std::endl;
    std::cout << "Size of union in a : " << sizeof(a)      << std::endl;
    std::cout << "Size of union in b : " << sizeof(b)      << std::endl;
}
```

输出：

```
Size of double     : 8
Size of int        : 4
===================================
Size of variant    : 16
===================================
Size of union in a : 8
Size of union in b : 4
```

事实上，他们俩储存数据的方式如下：

+ `std::variant`

	把 `<>` 表中所有数据类型按顺序**首尾相接**，最后取一个**连续的 $2^n$ 的字节大小空间**储存下这一类型。

	e.g. 一个 8字节的 `double` 类型 和 一个 4字节的 `int` 类型，内存上就是开一个8字节后面跟一个4字节，再用 $2^4 = 16$ 个字节囊括所有空间，属于是开了一个连续的16字节的空间当作该类型的储存空间。

	(Tips. `std::string` 在这种情况下的默认大小就是可伸缩的，大小就是 $2^n$ 减去其他类型所占的空间)

+ 联合体 `union`

	当该联合体被用作啥类型时，就用啥类型的空间大小去存储。

	例子见上面代码那例子。



### 性能优化 & 类型安全

从技术上讲，`union` 绝对是更有效率的。

但除非你是在做**底层优化**而需要把内存大小保持在相对较低的水平, 例如**CPU处理**, 这种级别的抠门是没必要的。

在**桌面应用**上，你有相对宽裕的内存储存空间，你就应当考虑更多使用 `std::variant` 。

因为 `std::variant` 是**类型安全**的，它**不会造成未定义行为**，在技术上更安全。



## 用 `std::variant` 做错误系统

你可以使用抛出异常，但如果这是一个内置的系统，它报的错是针对系统而非 `C++` 的编译的话，我们就需要构造一套基于 `cout` 或者什么的报错系统而不是抛异常。

这里就可以用到 `std::variant` 。

以读文件为例，我们基于在介绍 `std::optional` 文章中写的文件读取[代码](optional 处理OPTIONAL 数据.md)，并进行修改：

```cpp
```



