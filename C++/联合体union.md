# 联合体union

**定义**：有点像类 (结构体) ，只不过一次只能占用一个成员的内存，

​			也就是它实际上只有一个成员的内存地址，其他成员是对它的引用.



​			你可以对它添加方法，但是他不支持虚方法.



​			它一般与[类型双关](类型双关.md)紧密相连.

​			通常union是匿名使用的，但是**匿名union不允许成员函数**.

​			



**实现**：

**一般实现**

```cpp
union {
	类型名 成员名;
    ...
/*struct是选用的*/
    struct {
//相当于添加一个union成员，但该成员的表现形式是一个结构体，也就是说".结构体括号内不同成员" 的内存就不一样
		类型名 成员名;
    };
};
成员名...;//就像一个普通成员一样使用，*只不过同样内存会以所用成员名的类型呈现
```



**类内联合体** 实现 **类型双关**

```cpp
int main() {
    struct Transform {
        union {
            int a;
            float b;
        };
    };
    Transform t;
    t.a = 2;
    std::cout << t.a << std::endl << t.b << std::endl;
}
```

输出：

```
2
2.8026e-45
```



------

Cherno举的例子：

```cpp
#include<iostream>
#include<cstdio>

struct Vector2 {
	float x, y;
};

struct Vector4 {
	union {
		struct {//把 四维向量 分成 四个方向的基底
			float x, y, z, t;
		};
		struct {//把 四维向量 看成 两个平面向量
			Vector2 a, b;
		};
	};
};

void PrintVector2(const Vector2& vec) {
	std::cout << vec.x << ", " << vec.y << std::endl;
}

int main() {
	Vector4 example = { 1.14f,5.14f,11.4f,51.4 };
	PrintVector2(example.a);
	PrintVector2(example.b);
	printf("===================================\n");
	example.y = 233333.33f;
	PrintVector2(example.a);
	PrintVector2(example.b);
}
```

输出：

```
1.14, 5.14
11.4, 51.4
===================================
1.14, 233333
11.4, 51.4
```

添加多层修改方式了属于是.

------

​			