# 关键字auto

**定义**：让C++自动识别定义类型 并进行定义

​			这会让C++在此刻变得更像**弱类型语言**

**权衡**：

**好**：

+ 当了解该参数的类型实在是不必时，可以免去麻烦.

e.g.一个vector的遍历本来要这么写的：

```cpp
std::vector<std::string> strings;
	for (std::vector<std::string>::iterator it = strings.begine(); 
         it != strings.end(); it++)
	{
		std::cout << *it << std::endl;
	}
```

有了`auto`你却可以这么写：

```cpp
std::vector<std::string> strings;
for (auto it = strings.begin(); it != strings.end(); it++)
	{
		std::cout << *it << std::endl;
	}
```

当然你可以使用`using`[^1]和`typedef`[^2]来解决

+ 当api类型改变时，我们客户端（依赖api写出的扩展代码）的代码也不需要改动.



**坏**：

+ 当需要了解该参数的类型时，查找它的类型比较麻烦（将鼠标悬停在变量上/手动去库中查找）

+ 当api类型改变时，客户端的功能可能会遭到破坏，而且这种情况下的维修相比而言更难.

e.g.原本代码是这样的

```cpp
#include<iostream>

std::string GetName() //一个api接口
{
	return "DvdBr3o";
}

int main()
{
	auto name = GetName();
	std::cout << name.size() << std::endl;//不自觉地用了.size()
}
```

但如果api更新了

```cpp
#include<iostream>

char* GetName() 
{
	return (char*)"DvdBr3o";
}

int main()
{
	auto name = GetName();
	std::cout << name.size() << std::endl;
}
```

那你用`auto`，报错肯定没有原先直接用`std::string`来的直接.



[^1]:**using**用法：`using Alt = Original_Type; `
[^2]:**typedef**用法：`typedef Orginal_type Alt;`

