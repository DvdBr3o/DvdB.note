# `std::any` | 存储任意类型的数据

它在 **`C++17`** 被引入，被包含于 `<any>` 。

非常像 `python` 的一个特性，直接快把 `C++` 怼成弱类型了。

同样的效果可以用 **`void*` 指针**来实现。

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
