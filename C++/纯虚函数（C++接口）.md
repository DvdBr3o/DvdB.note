# 纯虚函数（C++接口）

**功能**：在父类定义没有实现的函数，然后**强制**子类去实现该函数.

​			事实上，纯虚函数之所以是“纯”的，就是他的**所有方法**都是**虚（virtual defined）**的，**强制由子类定义**.

​			又因为它的所有方法都未定义，所以它**不能被实例化**[^1].

​			同样，如果纯虚函数的子类**还未覆写所有父类的虚函数**时，也**不能被实例化**（强制性）.

[^1]: **实例化**：指用该类定义一个变量，如ClassName是一个类，那么```ClassName a;```就是实例化了ClassName

**实现**：

```cpp
class PureVirtualFather_i{//纯虚函数基类
	virtual spcTypeName_i spcMethodName_i() = 0;
    ...
};
class ClassSon : public PureVirtualFather_1,PureVirtualFather_2, ...{//子类继承
    spcTypeName_i spcMethodName_i() overrride{...}//每个方法都要定义
    ...
}
```



e.g.

```cpp
#include<iostream>
#include<cstdio>

class PureVirtualA {//纯虚函数A
public:
	virtual void MethodFromPVA() = 0;
};

class PureVirtualB {//纯虚函数B
	virtual void MethodFromPVB() = 0;
};

class ClassOrphan : public PureVirtualA {//孤儿类，只继承自纯虚函数A（这个孤儿其实是有个马的）
public:
	void MethodFromPVA() override {
		std::cout << "I'm doing method from Pure Virtual A" << std::endl;
	}
};

class ClassSon : public PureVirtualA, PureVirtualB {//子类，继承纯虚函数A和B
    //啊对对对，同时继承多个基类可以在基类间加逗号
    //但继承多个纯虚函数时，别忘了所有基类的所有成员都要定义，否则不能被实例化
public:
	void MethodFromPVA() override {
		std::cout << "I'm doing method from Pure Virtual A" << std::endl;
	}
	void MethodFromPVB() override {
		std::cout << "I'm doing method from Pure Virtual B" << std::endl;
	}
};

int main() {
	ClassOrphan Orphan;ClassSon Son;//定义一个孤儿实例，和一个子类实例
    
	std::cout << "Orphan:" << std::endl;
	Orphan.MethodFromPVA();
    
	std::cout << std::endl;
    
	std::cout << "Son:" << std::endl;
	Son.MethodFromPVA();
	Son.MethodFromPVB();
}
```

输出：

```
Orphan:
I'm doing method from Pure Virtual A

Son:
I'm doing method from Pure Virtual A
I'm doing method from Pure Virtual B
```



纯虚函数因为其**强制性**，适合用来**规范派生类的行为**.







