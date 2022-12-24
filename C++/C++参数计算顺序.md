# C++参数计算顺序

你或许会在一些工作面试考试题遇到这样的读程序写结果的题：

```cpp
void Print(int a, int b) {
	std::cout << a << " + " << b << " = " << a + b << std::endl;
}

int main() {
    int val = 0;
	Print(val++, val++); //[1]
}
```

重点在 **[1]** 上的后缀表达式对吧？

而事实上，这是**未定义行为**。

也就是说，在不同的编译器环境下会有不同的答案。



值得注意的是，在 `C++17` 中新添了一项规则：

<blockquote>
    实参和形参的计算不能同时做，即使没有规定这一过程的顺序。
</blockquote>

面试答到这个挺加分。

更多关于参数计算的细则在这：[C++17之定义表达式求值顺序_janeqi1987的专栏-CSDN博客_c++求值顺序](https://blog.csdn.net/janeqi1987/article/details/100181769)

但怎么说这种坑壁问题也只有在面试才会遇到了。



相似的问题在知乎上也有：

[i＝1，为什么 (++i)+(++i)＝6？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/347864795/answer/2252846703)

很明显这人被面试坑得挺惨啊。



*以后遇到这样关于编译器的问题，可以到 **[Wandbox](https://wandbox.org/)** 寻找答案。