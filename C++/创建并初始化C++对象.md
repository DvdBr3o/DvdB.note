# 创建并初始化C++对象

Lnk：[栈作用域生存期.md](栈作用域生存期.md)

​		  [堆与栈的内存比较.md](堆与栈的内存比较.md)

### 了解栈区与堆区对象的生命期

假设我们有这样的一个类：

```CPP
class Knob {
private:
    std::string m_Name;
public:
    Knob() :
        m_Name("Unknown") {}
    Knob(const std::string& Init_Name) :
        m_Name(Init_Name) {}
    std::string GetName() { return m_Name; }
};
```



#### 在栈Stack区创建的对象.

作用域仅限于当前的声明区域，当超出当前声明区，由于储存在栈区，他会和当前区域一起被弹出而失效.

```cpp
int main() {
    Knob* ptr;
    {
        Knob knob;//这里我们在栈上创建了一个Knob类实例knob
        ptr = &knob;
        std::cout << (*ptr).GetName() << std::endl;
    }//一个代码块，此时实例knob已被弹出释放
    std::cout << (*ptr).GetName() << std::endl;
}
```

输出：

```
Unknown

```

**注意！**通常我们也**不**像这样**在栈上创建类**，因为栈的内存通常很小 (哦~，还记得深搜吗)

所以我们会在堆区创建类，创建方式就在下方👇



#### 在堆Heap区创建的对象.

作用域自定义，又我们决定何时创建，何时释放该对象.

定义方式：

```cpp
Konb* konb = new Knob();
```

关键在于关键字`new`.

关键字`new`返回的是一个在堆上分配的对象的指针.

**但是！**你不应该把所有的对象创建在堆上.

一是**性能问题**，在堆上分配要比栈花费更长的时间，堆上的内存还需要你手动释放.



#### 如何选择创建对象所在的区？

+ 在堆上创建：
	+ 对象太大
	+ 想要显式控制对象的生命期
+ 在栈上创建：
	+ 不满足以上堆的任何条件
	+ 想要更快的速度
	+ 想要更自动的释放

