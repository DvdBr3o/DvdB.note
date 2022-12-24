# 优化字符串String

在很多实时运算的桌面应用中，字符串的多次分配会降低程序性能，这需要我们反思如何去优化String本身的分配，以达到更有的性能。



## 检测分配

想要搞清字符串是如何分配的，我们需要重载 `new[]` 运算符，让他在分配时输出一些提示：

```cpp
void* operator new(size_t size){
    
    std::cout << "[NOTICE] Aollcating " << size <<" bytes\n"; //[1]
    _Allocate_Num ++; //[2]
    return malloc(size); //[3]
}
```

[1] 提示分配了多少空间。

[2] 记录分配次数。

[3] 为调用此运算符的对象分配空间，这也是我们不重载时它本来该做的。



## 原生分配

让我们摆弄一些 `std::string` 试一试：

```cpp
void PrintName(const std::string& str){
    std::cout << str << std::endl;
}

int main(){
    std::string str = "DvdBr3oooooooooooooooooooooooooooooooooooooooooo"; //[1]
    PrintName(str);
    std::cout << "=================================" << std::endl;
    std::string str1 = str.substr(0, 15); //[2]
    PrintName(str1);
    std::cout << "=================================" << std::endl;
    std::string str2 = str.substr(0, 16); //[3]
    PrintName(str2);
    std::cout << "=================================" << std::endl;

    std::cout << "[SUM] Allocated " << _Allocate_Num << " times!\n" << 	std::endl;
}
```

输出：

```
[NOTICE] Aollcating 49 bytes...
DvdBr3oooooooooooooooooooooooooooooooooooooooooo
=================================
DvdBr3ooooooooo
=================================
[NOTICE] Aollcating 17 bytes...
DvdBr3oooooooooo
=================================
[SUM] Allocated 2 times!
```

什么意思呢？

注意到 [2] 是没有用到 `new` 重新分配的。

也就是说，当定义一个 `std::string` 大小超过 $2^4 = 16$ 个字节时，他会需要用 `new` 在**堆**上重新分配。

然而，堆上分配对性能的损伤都是很大的。



## 字符窗口 `std::string_view`

在 **`C++17`** 引入的新特性，尽管在 `C++17` 之前，人们就在使用相似的功能。

它被用来专门解决这种**子串问题**。

基本上，它就是对给定 `std::string` 类型数据的给定区域，相当于开了一个窗口，**返回了一个指针和大小**。

这样一来，可比复制一个 `substr` 来的快。

```cpp
std::string_view str3(str.c_str(), 3); //[4]
PrintName(str3);
std::cout << "=================================" << std::endl;
std::string_view str4(str.c_str() + 3, 46); //[5]
PrintName(str4);
std::cout << "=================================" << std::endl;
```

输出： 

```
Dvd
=================================
Br3oooooooooooooooooooooooooooooooooooooooooo
=================================
```

没有在堆上分配空间！



## 继续优化

但事实上在定义 `std::string` 本体 `str` 时，会**不可避免**地在堆上分配空间。

所以优化 `std::string` 最好的办法就是不用它。。。

in this case, 你的 `str` 定义时给的是一个字面量，那直接使用 `const char*` 是更好的。

```cpp
void PrintName(const char* str){
    std::cout << str << std::endl;
}

void PrintName(std::string_view strv){
    std::cout << strv << std::endl;
}

int main(){
    const char* str = "DvdBr3oooooooooooooooooooooooooooooooooooooooooo"; //[1]
    PrintName(str);
    std::cout << "=================================" << std::endl;
    std::string_view str3(str, 3); //[4]
    PrintName(str3);
    std::cout << "=================================" << std::endl;
    std::string_view str4(str + 3, 46); //[5]
    PrintName(str4);
    std::cout << "=================================" << std::endl;

    std::cout << "[SUM] Allocated " << _Allocate_Num << " times!\n" << std::endl;
}
```

输出：

```
DvdBr3oooooooooooooooooooooooooooooooooooooooooo
=================================
Dvd
=================================
Br3oooooooooooooooooooooooooooooooooooooooooo
=================================
[SUM] Allocated 0 times!
```

