# `std::optional` | 处理OPTIONAL数据

该特性在 **`C++17`** 被引入，被包含在 `<optional>`

## 为什么 `std::optional` ？

有时，我们需要处理[^1]**一些可能存在、也可能不存在的数据**，抑或是[^2]**一些错误的数据格式**。

我们还有可能需要读取函数在遇到这些错误时**返回某些值**。

`C++17` 引入了包括 `std::optional` 等共三个这样好用东东，本篇只介绍 `std::optional` 。

这三个好东东就是： `std::optional` 、[`std::variant`](variant 单一变量存放多种类型数据！.md) 、[`std::`any](any 存储任意类型的数据.md)



## `std::optional` 的使用

*以**读取文件**为例。

### 原本处理 不存在文件 的 方法

```cpp
//我们原本处理 不存在文件 的 方式
#include<iostream>
#include<fstream>
#include<string>

std::string readFileAsString(const std::string& filePath, bool& fileOpenedSuccessfully){
    std::ifstream stream(filePath);
    if(stream){
        std::string result;
        //读文件操作...
        fileOpenedSuccessfully = true;
        stream.close();
        return result;
    }
    fileOpenedSuccessfully = false;
    return "";
}

int main(){
    static bool myFileIsOpenedSuccessfully = false;
    std::string myFilePath;
    std::printf("File Path:");
    std::scanf("%s",myFilePath);
    std::printf("%s",readFileAsString(myFilePath,myFileIsOpenedSuccessfully));
    if(!myFileIsOpenedSuccessfully)
        printf("[ERROR] File is NOT opened successfully!\n");
    else
        printf("[NOTICE] File is opened successfully!\n");
}
```



### 使用 OPTIONAL 处理 不存在文件 类问题

使用 `std::optional` 可以大大简洁我们的函数参数及使用。

```cpp
//使用 OPTIONAL 处理 不存在文件 类问题
#include<iostream>
#include<fstream>
#include<optional>
#include<string>

std::optional<std::string> readFileAsString(const std::string& filePath){
    std::ifstream stream(filePath);
    if(stream){
        std::string result;
        //读文件操作...
        stream.close();
        return result;
    }
    return {};
}

int main(){
    std::optional<std::string> data = readFileAsString("data.txt");
    if(data.has_value()) //这里可以直接写if(data), 更简洁
        printf("[NOTICE] file has been read successfully!\n");
    else
        printf("[ERROR] file has NOT been read successfully!\n");
}
```



### OPTIONAL 设置默认值

用到 `std::optional::value_or(...)` 。

当文件没值 (即打开失败) 时，将目标值赋为括号内的值。



**如何记忆？** 假设当文件不存在时，设定读取的内容是 `"Content empty..."`，那么使用 `std::optional::value_or(...)` 就应当如下：

```cpp
std::optional <std::string> data = ...;
std::string content = data.value_or("Content empty...\n");//[1]
```

把 [1] 看作 `content = data的值 or "Content empty"`，

当 `data` 值空时，`content` 自然赋为 `"Content empty...\n"` 啦！

非常的形象~



那么完整代码就像酱紫：

```cpp
//用 value_or 为 OPTIONAL数据 设定默认值
#include<iostream>
#include<fstream>
#include<optional>
#include<string>

std::optional<std::string> readFileAsString(const std::string& filePath){
    std::ifstream stream(filePath);
    if(stream){
        std::string result;
        //读文件操作...
        stream.close();
        return result;
    }
    return {};
}

int main(){
    std::optional<std::string> data = readFileAsString("data.txt");
    std::string content = data.value_or("Content empty...\n");
    if(data.has_value()) //这里可以直接写if(data), 更简洁
        printf("[NOTICE] file has been read successfully!\n");
    else
        printf("[ERROR] file has NOT been read successfully!\n");
}
```

