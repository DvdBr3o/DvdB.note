# CRTP & Mixin

当某些抽象需要极致性能，无法承受虚函数动态分发时函数寻址的开销时，可以考虑通过 CRTP习语 或者 Mixin语义 进行**静态分发**。



## CRTP

全称 Curiously Recursive Template Pattern。主要原理是定义一个 CRTP 基类，带有派生类的类型参数，通过将 `this` 类型转换成派生类从而调用派生类的相应函数，达到静态分发。

定义一个 CRTP 基类：

```cpp
template<class Derived>
class CRTP {
public:
	Derived& interface() { return *static_cast<Derived*>(this); }

public:
	void do_something(int param) { interface().do_something_impl(param); }
};
```

然后让派生类继承 `CRTP<派生类>`，并实现相关接口即可：

```cpp
class DerivedCRTP : public CRTP<DerivedCRTP> {
public:
	void do_something_impl(int param) { std::cout << std::format("{}\n", param); }
};
```

即可实现：

```cpp
int main() {
	DerivedCRTP crtp;
	crtp.do_something(1); // output: 1
}
```

cppreference的范例：[Curiously Recurring Template Pattern - cppreference.com](https://en.cppreference.com/w/cpp/language/crtp)



## Mixin

跟 CRTP 完全相反。是由 CRTP基类 继承子类。然而因为 C++ 模板只在被用到时才实例化，所以派生类不会被认为是 `incomplete type`。

```cpp
template<class T>
struct CRTP : T {
	T&	 interface() { return *static_cast<T*>(this); }

	void do_something1(int param) { interface().do_something1(param); }

	void do_something2(const std::string& param) { interface().do_something2(param); }
};

struct DerivedCRTP_A {
	void do_something1(int param) { std::cout << std::format("DerivedCRTP_A: {}\n", param); }

	void do_something2(const std::string& param) { std::cout << std::format("{}\n", param); }
};

using Standard_DerivedCRTP_A = CRTP<DerivedCRTP_A>;

int main() {
	Standard_DerivedCRTP_A crtp;
	crtp.do_something1(1);
}
```



## CRTP/Mixin after C++20

众所周知，C++20 引入了 concept。那么为了不让用户对实现接口感到困惑，我们是不是可以用 concept 进行一下静态约束呢？

在这篇文章提到了这个想法：[Replacing CRTP Static Polymorphism With Concepts - Fluent C++ (fluentcpp.com)](https://www.fluentcpp.com/2020/09/11/replacing-crtp-static-polymorphism-with-concepts/)。没错，C++20 with concept 下更好的方式是直接不用 CRTP，完全只用 concept 约束即可。

使用了约束，我们也就不需要再对派生类继承 `CRTP<派生类>`，而是直接使用 **Mixin语义**。

```cpp
template<class T>
concept CRTPable = requires(T t) {
	t.do_something1(1);
	t.do_something2(std::string {});
};

template<CRTPable T>
struct CRTP : T {
	T&	 interface() { return *static_cast<T*>(this); }

	void do_something1(int param) { interface().do_something1(param); }

	void do_something2(const std::string& param) { interface().do_something2(param); }
};

struct DerivedCRTP_A {
	void do_something1(int param) { std::cout << std::format("DerivedCRTP_A: {}\n", param); }

	void do_something2(const std::string& param) { std::cout << std::format("{}\n", param); }
};

using Standard_DerivedCRTP_A = CRTP<DerivedCRTP_A>;

int main() {
	Standard_DerivedCRTP_A crtp;
	crtp.do_something1(1);
}
```



## CRTP after C++23

C++23 引入了 `this auto` 推导 `this` 的类型，以后派生类继承 CRTP基类 就不用带上派生类类型作为模板参数了。

```cpp
struct base {
    template <class Self>
    void f(this Self&& self);
};

struct derived : base {};

int main() {
    derived my_derived;
    my_derived.f();
}
```

具体点，比如说：

```cpp
struct add_postfix_increment {
    template <typename Self>
    auto operator++(this Self&& self, int) {
        auto tmp = self;
        ++self;
        return tmp;
    }
};

struct some_type : add_postfix_increment {
    // 这是前缀递增，后缀递增依靠它实现
    some_type& operator++();
};
```

以上来自：[译 | C++23 之推导 this：是什么，为什么，怎么用 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/563527205) 