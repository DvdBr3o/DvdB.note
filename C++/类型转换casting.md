# 类型转换casting

**定义**：OI-tormentee, u must've known it~

## C风格类转

### 显式转换

OI-tormentee, u must've known it~

但是，还是举个例子：

```cpp
double a = 5.5;
double result1 = (int)a + 5.5;
double result2 = (double)(int)(a + 5.5);
/*Ouput*/
//result1 = 10.5
//result2 = 11
```

### 隐式转换

见[隐式转换 & 关键字explicit.md](隐式转换 & 关键字explicit.md)



## C++风格类转

**主要四种**：`static_cast`[^1], `reinterpret_cast`[^2], `dynamic_cast`[^3], `const_cast`[^4]

**实现**：

```cpp
... = ..._cast<类型>(对象);
```



**与C风格类转的异同**：
1 没有开销，性能消耗完全相同，**除了`dynamic_cast`**

2 只是一种C++的语法糖

3 提供了后期搜索类转类型的方便，例如当你觉得性能开销太大时，你可以直接搜索`dynamic_cast`并替换

4 编译器可以 **随时提示** 类转是否可行



请在你的新项目里尽量使用C++风格的类转，因为它会让你的代码更可靠.



[^1]:**static_cast**：标准的类转，与C风格的**显示转换**意义相同
[^2]:**reinterpret_cast**：用于**类型双关**，指将一定区域的内存直接解释为另一种类型
[^3]:[**dynamic_cast**](dynamic_cast.md)：当**类转不成功**时，返回**NULL** (需要RTTI，所以有一定开销)
[^4]: **const_cast**：用来**添加/删除`const`修饰符**

