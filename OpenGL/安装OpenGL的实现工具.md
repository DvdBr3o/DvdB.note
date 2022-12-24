# 安装OpenGL的实现工具

OpenGL实际上是显卡的api规范.

我们主要通过GLFW实现api的对接，通过GLEW实现函数扩展.

## 安装GLFW

在[《在C++中使用库！》](../C++/在C++中使用库！.md)已经介绍.



## 安装GLEW

在[glew.sourceforge.net](glew.sourceforge.net)下载

链接与GLFW相同.

但需要注意的是，如果你没咋看下载的`doc`文档，你可能会有很多报错，你需要逆向工程去理解它的原理，然后进行编译和链接上的修复.

这里整理了几个问题：

1 #error:  gl.h included before glew.h

​	**解决**：在`glfw3.h`前`#include`上`glew32.h`

2 LNK1104	无法打开文件“glew32s.h” (按`doc`在`glfwInit()`后写了`glewInit()`后链接错)

​	**解决**：`Ctrl`点击`glewInit()`发现它以`GLEWAPI`被定义，`Ctrl`点击`GLEWAPI`发现：

```cpp
#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  ifdef GLEW_BUILD
#    define GLEWAPI extern __declspec(dllexport)
#  else
#    define GLEWAPI extern __declspec(dllimport)
#  endif
#endif
```

也就是`#else`被执行了，去链接了`glew`的动态库，按照定义，应该添加一个`GLEW_STATIC`的宏定义

所以解决就是右键项目属性，在预处理器的预处理器定义中添加`GLEW_STATIC;`

...更多问题，说实话，`glew`比`glfw`安装难多了，但是只要看了文档`doc`，基本不是问题

