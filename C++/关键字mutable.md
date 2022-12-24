# 关键字mutable

### 关键字mutable & 关键字const

见[《关键字const》](关键字const.md) -> **关键字const & 类** -> **mutable关键字 对 类中const化方法 的妙用**.



### 关键字mutable & lambda表达式

这种用法小概率用到.

首先理解啥是lambda表达式.

lambda表达式实际上是C++11新增的语法糖，可以看看[C++ Lambda表达式的完整介绍](https://zhuanlan.zhihu.com/p/384314474).

当我们写出如下lambda表达式：

```cpp
int x = 0;
auto f = [=]() {
	std::cout << ++x << std::endl;
};
for (int i = 0; i < 5; i++)
	f();
```

程序报错，编译器认为lambda表达式的`x`不是可以修改的左值.

我的理解是lambda表达式中`[=]`以值捕获，实际上就是**常值引用**捕获的`x`变量.

相当于我们在一个`const`化的、保证不修改param值的函数里做出了`++x`的操作.

------

要修修的话只能：

```cpp
int x = 0;
auto f = [=]() {
    int y = x;
    y++;
	std::cout << y << std::endl;
};
for (int i = 0; i < 5; i++)
	f();
```

可这不相当于引入了一个形参去复制实参嘛？

------

这时就可以用到关键字`mutable`：

```cpp
int x = 0;
auto f = [=]() mutable {
	std::cout << ++x << std::endl;
};
for (int i = 0; i < 5; i++)
	f();
```

程序正常运行：

```
1
2
3
4
5
```

啊？那x不相当于被改变了嘛？这还叫形参？

------

nonono，我们在代码末尾加上`std::cout << std::endl << x << std::endl;`：

```cpp
#include<iostream>

int main() {
	int x = 0;
	auto f = [=]() mutable {
		std::cout << ++x << std::endl;
	};
	for (int i = 0; i < 5; i++)
		f();
	std::cout << std::endl << x << std::endl;
}
```

输出：

```
1
2
3
4
5

0
```

------

x实际上并未被改变，而是被正常的常值引用成了形参进入了lambda表达式.

不仅代码更整洁了，甚至还省下了形参复制实参的空间.

