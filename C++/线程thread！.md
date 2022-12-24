# 线程thread！

## std::thread

**需要包含库**：`<thread>`

**基本实现**：

```cpp
std::thread 线程名(函数指针);//新建线程，用以运行函数
线程名.join();//等待结束
auto LocalThreadId = std::this_thread::get_id();//获取当前作用域线程id
using namespace std::literals::chrono_literals;//时间单位名称空间获取
constexpr auto OneSecond = 1s;//获取1秒时间
std::this_thread::sleep_for(OneSecond);//当前线程休眠(1秒)
std::this_thread::sleep_for(std::chrono::seconds(1));//当前线程休眠(1秒)
```

------

更多见官方文档/博客：
[thread Class | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/standard-library/thread-class?view=msvc-170)

[en.cppreference.com](https://en.cppreference.com/w/cpp/thread/thread)

[C++11中std::thread的使用 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/389140084)

[深入剖析C++11线程库std::thread，迈入多线程编程的大门 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/354676653)

------

Cherno举的例子：

```cpp
#include <iostream>
#include <thread>

static bool isFinished = false;

void actDoingWork() {
	std::cout << "Thread id: " << std::this_thread::get_id() << std::endl;
	while (!isFinished) {
		using namespace std::literals::chrono_literals;
		std::cout << "Working..." << std::endl;
		std::this_thread::sleep_for(0.5s);
	}
}

int main() {
	std::thread MyThread(actDoingWork);
	std::cin.get();
	isFinished = true;
	MyThread.join();
	std::cout << "Finished!" << std::endl;
	std::cout << "Thread id: " << std::this_thread::get_id() << std::endl;
	std::cin.get();
}
```

