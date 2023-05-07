# 在C++中使用库！

## Cherno的推荐做法

**概要**：在实际解决方案中的实际项目文件夹中，保留使用库的版本.

​			**大多数正经项目**，建议实际构建源代码，即添加一个包含你所依赖库的项目，并将其编译为静态库 (.a/.lib) 或动态库 (.so/.dll)

​			**快速项目 / 拿不到源代码**，链接二进制文件 (.bin)

### 链接二进制文件 [快速项目 / 拿不到源代码]

#### 静态库/静态链接

以GLFW库 (你将来会在OpenGL需要它)为例：

+ 1 **进入[glfw.org](https://www.glfw.org/)**

+ 2 **选择x86/x64？**

	**与操作系统无关**，请选择你**编译的应用程序**的x86/x64.

	然后下载`glfw-x.x.xx`

+ 3 **在你的解决方案文件夹下创建`\Dependencies`文件夹** (养成好习惯)

	​	顺便再在`\Dependencies`下创建相应二进制文件的名称，也就是`\GLFW`

+ 4 **将下载的`\glfw-x.x.xx`下的`\include`和`\lib-vc2022`[^1]拷贝到`\Dependencies\GLFW`下**

	​	(实际上，你会发现`\lib-vc2022`下同时含有`.lib`和`.dll`文件[^2]，甚至一个`.dll.lib`文件[^3])

+ 5 **让C++确定库的位置**

  右击你的VisualStudio项目文件夹，确认你的配置范围，

  然后`>> 属性 >> C/C++ >> 常规 >> 附加包含目录`，输入库`\include`位置.

  由于你有了复制到`\Dependencies`下的好习惯，

  所以 in this case，你可以直接输入`$(SolutionDir)Dependencies\GLFW\include`[^4]

  如果你实在搞不清`$(SolutionDir)`，你可以在框右侧点击下箭头，选`<Edit...>`来查看它是啥.

  你甚至可以再点击右下角的`Macros>>`去发现更多的宏.

+ 6 **让链接器确定库的位置**

  右击你的VisualStudio解决方案项目根文件夹，确认你的配置范围，

  然后`>> 属性 >> 链接器 >> 常规 >> 附加库目录`，输入库`\lib`位置（！注意不是`.lib`，但是是`.lib`所在的文件夹！）

  in this case，输入`$(SolutionDir)Dependencies\GLFW\lib-vc2022`

  最后`>> 属性 >> 链接器 >> 输入 >> 附加依赖项`，输入库的名称，用`;`分隔，

  in this case，在开头添加`glfw3.lib;`.

  (当然你会发现报错了，你其实还要添加一个`OpenGL32.lib;`)

+ 7-1 **你可以开始`include`你的库了！**

	在需要的文件开头键入`#include<GLFW/glfw3.h>`或`#include"GLFW/glfw3.h"`[^5]
	
+ 7-2 **或者因为你有*好习惯*，可以直接声明库函数，让链接器自己去找**.

  在需要的文件较开头键入例如`int glfwInit();`.

  **但是！**因为你使用的是C++，而`glfw`实际上是 C 语言库，所以为了用 C 的 ABI 去链接这个动态库，需要将其改为：

  `extern "C" int glfwInit();`

  从而让 C++ 链接器去链接一个 C 语言库.



[^1]:**认识二进制文件构造**：一般含有`\include`和`\lib`两部分.
[^2]:**认识连接方式**：分为**静态链接**和**动态链接**，**静态链接**会让库被直接编译到`.exe`文件. **动态链接**则是让库在`.exe`运行时被链接到程序中 (因此它要被放在`.exe`旁或特定位置). 一般**静态链接**在技术上更快，因为在此情况下编译器或链接器可以执行**链接时优化**等操作，从而生成更快的应用程序.
[^3]:**认识`.dll.lib`文件**：实际上是一种静态库`.lib`，它与`.dll`一起被使用，这样可以避免实际询问`.dll`文件是否含有指向所有包含函数的函数指针，从而在链接时被链接.
[^4]:**认识`$(SolutionDir)`**：它实际上是VisualStudio的一个宏，表示当前解决方案的所在文件目录.
[^5]:**区分`#include<>`与`#include""`**：**引号**会优先检查相对于当前文件的位置是否有符合的库，若否再去询问环境VisualStudio，**尖角号**则相反. 那么按照非常有经验的Cherno的经验来说，如果库存在于解决方案中，用**引号**；如果库是一种**完全的外部依赖**，用**尖角号**



#### 动态库/动态链接

以glfw为例，其他跟静态链接差不多，修改以下：

+ 1 修改库的名字

	`>> 属性 >> 链接器 >> 输入 >> 附加依赖项`，输入库的名称，用`;`分隔，

	in this case，在开头添加`glfw3.dll.lib;`.（这里只能加载`.dll.lib`文件）

+ 2 让`.dll`和`.exe`在一起

	将`\lib-vc-2022\glfw3.dll`复制到当前项目的`.exe`文件目录下.

------

Cherno提出了一个问题：为什么即使我不在`属性 >> C/C++ >> 预处理器 >> 预处理器定义`下添加`GLFW_DLL`也能通过？

搬运up主的回答：

```
看到很多人不知道最后的cherno的问题在说啥，我尝试回答下，不一定对，环境都是在visual studio中，如果有更好更正确的回答，我也会置顶供大家学习。

1、 _declspec(dllexport)与_declspec(dllimport)是相互呼应，只有在DLL内部用dllexport作了声明，才能在外部函数中用dllimport导入相关代码。

2、如果vs中的dll项目内部的函数前面都没有 _declspec(dllexport)，则无法生成导出库lib，只有dll生成。

3、lib库原理上应该是不做链接过程的，我在一个lib项目上，故意犯下链接错误，编译并不报错。另外，我的一个lib项目，想把opencv的dll库包进来，成为一个包含了opencv功能的lib库，是不行的，因为在这个lib项目中，在vs编译器中都找不到linker选项，再次说明lib库不做链接。

4、回到cherno的问题，因为这个项目已经用了引导库lib，所以不需要_declspec(dllimport)，就已经能够使用dll库了。lib就是起到定位dll的作用，所以用不用这个_declspec(dllimport)都无所谓。但如果此项目不用lib引导库，那必须使用_declspec(dllimport)来定位dll中的导出函数。
```

------



### 构建编译库源代码 [大多数正经项目]

#### 静态链接

有源代码的库尽量就是**静态链接**了，这样允许更多的**优化**在编译与链接时发生.

**假设任务**：我们有一个`Game.exe`的程序项目，要为它链接一个自制的`Engine.lib`静态库项目.

in this case (itc)，步骤如下：

+ 1 创建空项目`Game`

+ 2 右键解决方案，添加`Engine`项目

+ 3 右键`Engine`项目，找到`属性 >> 配置属性 >> 常规 >> 配置类型`更改为`.lib`

+ 4 `Game`项目下文件就像往常一样

+ 5 `Engine`项目可以参考`JUCE`的插件源码结构，通常是一个`Engine.h`和一个`Engine.cpp`

	​	一般是在`.h`文件写下所需名字空间`namespace`成员声明，然后在`.cpp`文件完成定义

+ 6 **让C++确定库的位置**

	同上上上，`>> 属性 >> C/C++ >> 常规 >> 附加包含目录`，输入`.h`所在文件夹

+ 7 **让链接器确定库的位置**

	先右键`Engine`项目 (itc)，然后点击`生成`

	右键`Game`项目 (itc)，然后点击`引用`，勾选`Engine`

	这样会使编译等更加自动化，just do it instead of 修改连接器属性.



Fxxk，我不知道为什么我以前一直用 Visual Studio Solution 构建项目，这很大程度上无法跨平台，而且这样构建项目真的很麻烦。

我根据上面步骤写一下相应的 `xmake.lua` ，看看有多简单：

```lua
target("Engine")
	set_kind("static")
	
	-- sources --
	add_headerfiles("Engine/include/**.h") -- 正确做法应该是在两个目录下分别写xmake.lua
										   -- 这里这样做方便演示
	add_files("Engine/src/**.cpp")
	add_includedirs("Engine/include", {public = true})

target("Game")
	set_kind("binary") -- 可以省略

	-- deps --
	add_deps("Engine")

	-- sources --
	add_headerfiles("Game/include/**.h")
	add_files("Game/src/**.cpp")
```

然后就：

```bash
xmake b
xmake r
```
