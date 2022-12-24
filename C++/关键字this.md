# 关键字this

**定义**：关键字`this`是一个指向当前对象实例的指针，该方法属于这个对象实例.

​			这个方法必须用一个有效的对象来调用.

## 1 用关键字this解决 方法形参与本类成员重复

```cpp
class Knob {
private:
    float PosX, PosY;
public:
    Knob(float PosX, float PosY) {
        this->PosX = PosX;
        this->PosY = PosY;
    }
};
```

## 2 避免delete this

害，还是自己看Cherno的[视频](https://www.bilibili.com/video/BV1qo4y1o7W5/?spm_id_from=pageDriver)吧...