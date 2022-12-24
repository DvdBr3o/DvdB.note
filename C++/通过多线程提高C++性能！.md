# 通过多线程提高C++性能！



## 为什么多线程？

当今很多硬件都是多核结构，数据可以进行并行处理。

运用多线程，能大大提升程序的性能。

`C++11` 以来，`std` 库多了很多对实现多线程有帮助的东东，

其中就包括我们要讲的 `std::async` ，它包含于 `<future>` 。 



## 多线程 的 实现

尼玛Cherno直接把游戏引擎代码搬过来了，直接看不懂我屮了，你有时间自己看吧：

[【79】【Cherno C++】【中字】如何让C++运行得更快_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1UR4y1j7YL?spm_id_from=333.1007.top_right_bar_window_history.content.click)

还有其他网站的资料：

[C++多线程并发基础入门教程 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/194198073)

[C++11 多线程（std::thread）详解_sjc_0910的博客-CSDN博客_c c++ 多线程](https://blog.csdn.net/sjc_0910/article/details/118861539)