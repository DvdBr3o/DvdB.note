# 隐式转换 & 关键字explicit



### 隐式转换

**定义**：C++允许的**一次最多一次**的类型自动转换.

0 我们有一个含有两种需要不同参数的构造函数的类`Knob`，以及一个打印该类的外函数：

```cpp
class Knob {
private:
    std::string m_Name;
    int Value;
public:
    Knob(const std::string& Init_Name) :
        m_Name(Init_Name), Value(0) {}
    Knob(int Init_Value) :
        m_Name("Unknown"), Value(Init_Value) {}
};

void PrintKnob(Knob aKnob){
    printf("%s:%d\n",)
}
```

------

1 **隐式转换**就包含如下操作：

```cpp
Knob a = 89;
Knob b = (std::string)"Volume";
```

------

2 甚至**外函数**里也能隐式转换：

```cpp
PrintKnob(114513);
PrintKnob((std::string)"Volume");
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

**意义**：防止性能问题或bug.