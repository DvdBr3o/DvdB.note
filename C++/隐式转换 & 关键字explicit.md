# 隐式转换 & 关键字explicit



### 隐式转换

**定义**：C++允许的**一次最多一次**的类型自动转换.

0 我们有一个含有两种需要不同参数的构造函数的类`Knob`，以及一个打印该类的外函数：

```cpp
class Knob {
	public: // .cpp 中你不得不先声明变量
	    std::string m_Name;
	    int m_Value;    

	public:
		Knob(const std::string& Init_Name) :
 	       m_Name(Init_Name), m_Value(0) {}
		Knob(int Init_Value) :
 	       m_Name("Unknown"), m_Value(Init_Value) {}
    	
    	void PrintKnob();
};

void PrintKnob(const Knob& knob) {
	printf("%s:%d\n", knob.m_Name.c_str(), knob.m_Value);
}
```

------

1 **隐式转换**就包含如下操作：

```cpp
Knob a = 89; // [1]
Knob b = std::string{ "Volume" }; // [2]
```

[1] 调用了 `operator=(&&another)` ，而 `another` 由 `Knob(int)` 构造

[2] 调用了 `operator=(&&another)` ，而 `another` 由 `Knob(const std::string&)` 构造

------

2 甚至**外函数**里也能隐式转换：

```cpp
PrintKnob(114513);
PrintKnob(std::string{ "Volume" });
```

输出：

```
Unknown:114513
Volume:-1
```

------

3 ***为什么不能**：

```cpp
PrintKnob("Volume");
```

因为要完成此操作，需要`const char*` -> `std::string` -> `Knob`两次隐式转换，但隐式转换一次最多一次.

这确实，要是隐式转换能链式传播，那类型安全无从谈起。这是简洁与安全的平衡。

------



### 关键字explicit

**定义/功能**：禁用隐式转换.

**实现**：在构造函数前加`explicit`.

```cpp
class ClassName{
public:
    explicit ClassName() : ... {...}
};
```

那么当你再用隐式转换时就不行了，只能显示转换.

在上一个小标题的例子，就只能使用显示转换：

```cpp
Knob a = Knob(114514);//用显式构造函数赋值
Knob b = (Knob)114514;//114514整形强制转化成Knob类
```

**意义**：防止性能问题或bug，保障类型安全.