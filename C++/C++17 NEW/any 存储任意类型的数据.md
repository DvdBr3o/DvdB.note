# `std::any` | 存储任意类型的数据

它在 **`C++17`** 被引入，被包含于 `<any>` 。

well，python默认声明不用前置类型，以及 C++ 的 auto 声明，跟 `std::any` 都不一样。因为前者他本身是有类型的，只不过在声明的时候类型可以被推导出来，在之后也可以通过隐式转换在某些场景下不需要在意类型。而后者则是一个模板类，你是没法在静态环境下得知它的确切类型的，而且会有动态分配造成**动态开销**，这一点 somehow 很像 js & lua 。

同样的效果可以用 **`void*` 指针**来实现 (类型擦除)，这样就不会有很大的开销，只不过就舍弃了类型安全。

LNK：`C++17` 给我们带来的三个类：  [`std::optional` ](optional 处理OPTIONAL数据.md)、[`std::variant`](variant 单一变量存放多种类型数据！.md) 、`std::any`

## `std::any` 的使用

### 定义

```cpp
std::any data;                                               //空定义
std::any data = std::make_any</*类型名*/>(/*相应类型名字面量*/); //初值定义
std::any data = std::make_any<int>(1);                       //初值定义e.g.
```

### 赋值

像TMD `python` 一样，比如：

```cpp
std::any data;
data = 114514;
data = "DvdBr3o";
data = (std::string)"DvdBr3o";
```

### 强制转换

用到 `std::any_cast`，比如：

```cpp
std::any data;
data = "DvdBr3o";
data = std::any_cast<std::string>(data);
```

优化 (引用优化)：

```cpp
std::any data;
data = "DvdBr3o";
data = std::any_cast<std::string&>(data);
std::string& MyString = std::any_cast<std::string&>(data);
```

## `std::any`    vs.    `std::variant`

### 内存分配

`std::any` 的内存是**动态分配**的，而 `std::variant` 不是。

这意味着在**量大的数据**面前，`std::variant` 更加优秀。

### 类型安全

`std::variant` 需要你**列出所有可能的类型**， 这添加了一点限制性，但同时也**增加了你的类型安全**。

`std::any` 在类型安全这一方面就要比 `std::variant` 逊色了。

总而言之，怎么说 `std::variant` 都比 `std::any` 更安全，更实用。

那如果确实有地方需要存储任意类型的数据呢？

Cherno建议你对你的程序设计回炉重造。
