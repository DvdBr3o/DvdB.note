# 关键字const

**定义**：将该量规定为常量，不得被修改.

​			修改该类的行为将被(某些)编译器视为**未定义行为**.

​			它像一种**承诺**，像声明一样让编译器安心.

​			但是否遵守承诺取决于你自己.



### 关键字const & 指针

+ **常内容指针**

  定义方式：

  ```cpp
  const TypeName* ptr;
  TypeName const* ptr;
  ```

  使用：

  ```cpp
  (*ptr)=2;//报错，常内容指针的内容不能被修改
  ptr = &CONST_VALUE;//OK
  std::cout << (*ptr) << std::endl;//OK，常内容指针的内容只是被调用，而未被修改
  ```

+ **常地址指针**

  定义方式：

  ```cpp
  TypeName* const ptr;
  ```

  使用：

  ```cpp
  (*ptr)=2;//OK
  ptr = &CONST_VALUE;//报错，常地址指针的地址不能被修改
  another_ptr = ptr//OK，常地址指针的地址只是被调用，而未被修改
  ```

+ **常内容常地址指针**

  定义方式：

  ```cpp
  const TypeName* const ptr;
  ```

  使用：

  ```cpp
  (*ptr)=2;//报错，常常指针的内容不能被修改
  ptr = &CONST_VALUE;//报错，常常指针的地址不能被修改
  std::cout << (*ptr) << std::endl;//OK，常常指针的内容只是被调用，而未被修改
  another_ptr = ptr//OK，常常指针的地址只是被调用，而未被修改
  ```

  

+ **怎么区分理解三种指针？(I)**

  时刻记住：(I)  将`*`作用范围理解到类型开头到`*`前一个字.

  ​				   (II) `const`的位置在同一级是可以调换的.

  <虽然这可能不是`* `& `const`在库中真正的定义>

  + `const TypeName*`指针

    =`(const TypeName)*`，指针指向的类型是`const TypeName`，即常量，所以是**常内容指针**.

  + `TypeName const*`指针

    =`(TypeName const)*`，指针指向的类型是常量（倒装一下，或理解为以`TypeName`类型定义了一个`const`，所以也是常量），所以是**常内容指针**.

  + `TypeName* const`指针

    =`(TypeName*) const` = `const (TypeName*)`，指针本身是常量，也就是指指针的地址是常量，所以是**常地址指针**.

  + `const TypeName* const`指针

    = `const ((const TypeName)*)`，用一个常量类型定义指针，指针本身还是常量，所以是**常内容常地址指针**.
  
+ **怎么区分理解三种指针？(II)**

  `const`和`*`修饰的都是它们后面那个单词块。

  + `const TypeName*`指针

      =`const (TypeName *a)`，`const`的是一个指针内容`*a`，所以是**常内容指针**.

  + `TypeName const*`指针

      =`TypeName （const *a)`，`const`了一个指针内容`*a`，以`TypeName`定义这个指针，所以是**常内容指针**.

  + `TypeName* const`指针

      =`TypeName *(const a)` ，`const`的是一个指针本身`a`，以`TypeName`定义该指针，所以是**常地址指针**.

  + `const TypeName* const`指针

      = `const (TypeName *(const a))`，`const`一个指针本身`a`，以`TypeName`定义该指针，再用一个`const`修饰这个`*(a)`，即这个指针的内容，所以是**常内容常地址指针**.

**插一嘴！**关于一连串指针定义：

```cpp
int* ptr1, ptr2,ptr3;//ptr1是整形指针，ptr2、ptr3是整形变量
```

```cpp
int* ptr1, * ptr2, * ptr3;//ptr1、ptr2、ptr3都是整形指针
```



### 关键字const & 类

#### const化方法 兼容 常量引用函数

假设我们有一个类：

```cpp
class Entity{
private:
    int m_X, m_Y;
    int DebugCount;
    ...//很多参数，非常非常多
public:
    int GetX(){
        return m_X;
    }
};
```

我们有一个外函数用于打印`Entity`类的`X`参数，但奈何`Entity`类的参数太多，让形参复制实参会大量浪费空间，于是我们选择使用**常量引用**的方法设定函数形参：

```cpp
void PrintEntityX(const Entity& e){
	std::cout << e.GetX() << std::endl;
}
```

程序会报错.

因为形参是对实参的引用，所以理解为直接将实参带入到参数中，但用`const`保证实参**不被修改**.

但类的`GetX()`方法**不能保证**`e`的参数不被改变，所以编译器报错.

解决办法是**用`const`让方法保证不对本类成员进行修改**：

```cpp
int GetX() const{
    return m_X;
}
```

**注意！**你可以**同时保留**类方法的`const`和非`const`版本，常量引用函数会**自动选择**`const`版本.

#### mutable关键字 对 类中const化方法 的妙用

假设`Entity`类有一个`DebugCount`在调试时必须在被`const`化的方法`GetX()`中被累加，怎么办？

可以用到一个C++的关键字**`mutable`**.

如下即可：

```cpp
class Entity{
private:
    int m_X, m_Y;
    mutable int DebugVar;
    ...//很多参数，非常非常多
public:
    int GetX(){
        DebugCount ++;
        return m_X;
    }
};
```

