# 关键字new

**定义**：返回同类型指向堆区的空指针.

​			它是一个可以重载的操作符.

​			它会**调用构造函数**.



### 不要万事都用new

它开辟内存的方式很慢.

关键字`new`根据空闲列表去寻找连续的堆区内存给数据使用，这很慢.



### 关键字delete

关键字`new`需要你手动释放内存.

```cpp
int* ptr = new int;
...
delete ptr;
```

当然，如果你用关键字`new`定义了数组的话，请这样释放内存：

```cpp
int* ptr = new int[50];
...
delete[] ptr;
```



### placement new优化

**定义**：决定前面的内存指向哪里.

​			因此你没有真正的分配内存，你只需要在一个特定的内存地址中初始化你的类.

**实现**：

```cpp
int MemorySpace = new int[50];
ClassName s = new(MemorySpace) ClassName();
//类型都不是一定的，可以用其他类型
```



