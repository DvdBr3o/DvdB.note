# 动态数组std::vector

啊...更多`std::vector`内容见：

官方文档：[vector 类 | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/standard-library/vector-class?view=msvc-170)

某些博客：[[C++\] std::vector - immjc - 博客园 (cnblogs.com)](https://www.cnblogs.com/immjc/p/7681173.html)

​					[C++ vector 容器浅析 | 菜鸟教程 (runoob.com)](https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html)

## STL标准模板库

**定义**：本质上是一个库，里面装满了容器（容器类型），这些容器包含特定的数据.

​			之所以是**“模板库”**，是因为他可以模板化任何东西，也就是说容器所包含的**数据类型由你决定**.



## 初步认识std::vector

**定义**：IOI-tormentee, u must have known~

**需要知道的原理**：当数组内存不够用时，找到新的内存，将数据**复制**. (拖慢代码的原因之一)

**实现**：

```cpp
std::vector <ClassName/TypeName> Object;
//请仔细考虑使用对象建立vector还是指针建立vector，通常是对象优先于指针
Object.push_back(.../*与Object相同类型对象*/);//末尾增加对象
size = Object.size();//访问数组大小
for(int i = 0; i < Object.size(); i++)//遍历
	{...}
for(ClassName/TypeName& obj : Object)//遍历
	{...}//该作用域obj == Object[i]
Object.erase(.../*一个std::iterator类型的迭代器, e.g. Object.begin() + 1 */);//删除...位置
```



## 优化std::vector

我们首先分析为什么`STL`的`std::vector`慢.

------

我们有如下代码：一个包含三个浮点成员的`Knob`类，当被拷贝时提示，在`Knob`类的`vector`中添加三次成员.

```cpp
#include<iostream>
#include<vector>

class Knob
{
private:
	float PosX, PosY, Value;
public:
	Knob(float x,float y,float v)
		: PosX(x),PosY(y),Value(v) {}
	Knob(const Knob& another)
		: PosX(another.PosX), PosY(another.PosY), Value(another.Value)
	{
		std::cout << "[DEBUG]: Knob Copied..." << std::endl;//当类被复制时提示
	}
};

int main()
{
	std::vector <Knob> knobs;
	knobs.push_back(Knob(1.1f, 1.1f, 1.1f));//*1
	knobs.push_back(Knob(1.1f, 1.1f, 1.1f));//*2
	knobs.push_back(Knob(1.1f, 1.1f, 1.1f));//*3
}
```

输出：

```
[DEBUG]: Knob Copied...
[DEBUG]: Knob Copied...
[DEBUG]: Knob Copied...
[DEBUG]: Knob Copied...
[DEBUG]: Knob Copied...
[DEBUG]: Knob Copied...
```

添加三次成员，类成员被拷贝六次，这十分拖慢代码.

人话：

```
"Knob(1.1f, 1.1f, 1.1f)"*1在main栈帧创建了一个对象[1]，由于它的作用域有限，它被拷贝到vector所在的内存...{问题类型I}

"Knob(1.1f, 1.1f, 1.1f)"*2将要创建对象，vector空间不够，[1]被拷贝到新的vector内存...{问题类型II}
"Knob(1.1f, 1.1f, 1.1f)"*2在main栈帧创建了一个对象[2]，由于它的作用域有限，它被拷贝到vector所在的内存...{问题类型I}

"Knob(1.1f, 1.1f, 1.1f)"*3将要创建对象，vector空间不够，[1]被拷贝到新的vector内存...{问题类型II}
"Knob(1.1f, 1.1f, 1.1f)"*3将要创建对象，vector空间不够，[2]被拷贝到新的vector内存...{问题类型II}
"Knob(1.1f, 1.1f, 1.1f)"*3在main栈帧创建了一个对象[2]，由于它的作用域有限，它被拷贝到vector所在的内存...{问题类型I}
```

------

解决方法如下👇

+ **{问题类型I}** 作用域有限，需要被拷贝到`vector`所在内存 

	用`.emplace_back()`直接在`vector`内存构造类：

	```cpp
	knobs.emplace_back(1.1f, 1.1f, 1.1f);//直接用所给参数 在vector内存构造类
	```

+ **{问题类型II}** `vector`新增对象时内存不够，就对象需要被拷贝到新分配的内存区 

	如果可以的话，一开始就告诉`vector`数组的大小，让`vector`**预留所需内存**：

	```cpp
	knobs.reserve(3);//预留所需内存
	```



***绝对地干货嗷~**
