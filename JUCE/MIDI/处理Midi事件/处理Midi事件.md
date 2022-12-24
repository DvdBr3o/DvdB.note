# 处理Midi事件

## 序

基于：[JUCE: Tutorial: Handling MIDI events](https://docs.juce.com/master/tutorial_handling_midi_events.html)

本节将会：

+ 使用 `juce::AudioDeviceManager` 来实现外部 `midi` 输入设备的管理
+ 使用 `juce::MidiKeyboardComponent` 创建使用 `midi` 键盘组件
+ [OPTIONAL] 刊登 `midi` 信息
+ [OPTIONAL] 控制 `midiCC` 信号



## 初始化

### 成员

```cpp
//管理midi设备
juce::AudioDeviceManager DeviceManager;               //[1]
juce::ComboBox listMidiInputDevices;                  //[2]
juce::Label labelListMidiInputDevices;
int indexLastMidiDevice = 0; 						  //[3]
bool isAddingMidiMessage = false; 					  //[4]
    
//midi键盘组件
juce::MidiKeyboardComponent MidiKeyboardOnScreen; 	  //[5]
juce::MidiKeyboardState stateMidiKeyboardOnScreen;    //[6]

//杂项
double startTime; 								      //[7]
```

**[1]** 用于管理 `midi` 设备

**[2] **用于选择 `midi` 设备

**[3] **当选择新 `midi` 设备时，需注销先前选择的 `midi` 设备，此项记录需要注销的序号

**[4]** 此项记录是否有外部 `midi` 设备正在输入

**[5]** 键盘组件，就像 `serum` 底下那种

**[6]** 键盘组件状态

**[7]** 记录开始时间，用于获取 采样序号 `SampleNumber` 

### 构造函数成员初始化列表

```cpp
PracticeAudioProcessorEditor::PracticeAudioProcessorEditor (PracticeAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
    MidiKeyboardOnScreen(stateMidiKeyboardOnScreen, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
	//...
}
```



## 管理设备

### 初始化

主要是处理好 `juce::ComboBox` 类的 `listMidiInputDevices` 。

```cpp
PracticeAudioProcessorEditor::PracticeAudioProcessorEditor (PracticeAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
    MidiKeyboardOnScreen(stateMidiKeyboardOnScreen, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    //没有可用设备时设定提示文本
    listMidiInputDevices.setTextWhenNoChoicesAvailable("No external midi input devices enabled...");
    
    //获取可用设备列表
    const auto& midiInputs = juce::MidiInput::getAvailableDevices();

    //将可用设备的名字录入ComboBox并编号
    juce::StringArray midiInputNames;
    for (auto& input : midiInputs)
        midiInputNames.add(input.name);
    listMidiInputDevices.addItemList(midiInputNames, 1);

    //更改设备时修改监听
    listMidiInputDevices.onChange = [this] {setMidiInput(listMidiInputDevices.getSelectedId()); };
    //这样我们仅修改ComboBox时，设备监听也会被同步修改

    //默认选择第一个可用设备
    for(auto& input : midiInputs)
        if (DeviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            listMidiInputDevices.setSelectedId(midiInputs.indexOf(input));
            break;
        }

    //无可用设备情况处理
    if (!listMidiInputDevices.getSelectedId()) 
        setMidiInput(0); //因为我们设置了偏移量，所以当没有设备被选择时SelectId就是0
    
    //显示组件
    addAndMakeVisible(listMidiInputDevices);

    setSize (400, 300);
}
```

**[1]** **`.identifier` vs. `.name`** ： `.identifier` 根据操作系统不同可能不同，比如在 `macOS` 是一个整型数字，而在 `Windows` 下则是一串字符串。`.name` 一定是字符串，且一定是可读的。由于不同设备 `.name` 可能相同，而 `.identifier` 人类不可读，所以我们建议当设备信息**面向用户**时用 `.name` ，**面向程序**时用 `.identifier` 。

### 管理监听 

通过自定义函数 `setMidiInput(...)` 管理设备监听的切换。

```cpp
void PracticeAudioProcessorEditor::setMidiInput(int index)
{
    //获取可用设备列表
    const auto& list = juce::MidiInput::getAvailableDevices();

    //移除对上一个被选设备的监听
    DeviceManager.removeMidiInputDeviceCallback(list[indexLastMidiDevice].identifier, this);

    //设定对选中设备的激活与监听
    const auto& newInput = list[index];
    if (!DeviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        DeviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);
    DeviceManager.addMidiInputDeviceCallback(newInput.identifier, this);

    //ComboBox Stuff...
    listMidiInputDevices.setSelectedId(index + 1);

    //更新被选设备序号
    indexLastMidiDevice = index;
}
```

对于文中的 `this` ，要求是 `juce::MidiInputCallback` 类型，所以需要在 `ProcessorEditor` 中私有继承一个 `private juce::MidiInputCallback` 。

```cpp
class PracticeAudioProcessorEditor  : 
    public juce::AudioProcessorEditor,
    private juce::MidiInputCallback
{
    //...
}
```

关于私有继承：[C++ 私有继承和保护继承（学习笔记：第7章 03） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/103949527)

> ### 私有继承(private)
>
> - 继承的访问控制
>
> - - 基类的**public**和**protected成员：都以private**身份出现在派生类中；
> 	- 基类的private成员：**不可直接访问**。
>
> - 访问权限
>
> - - 派生类中的成员函数：可以直接访问基类中的public和protected成员，但不能直接访问基类的private成员；
> 	- 通过派生类的对象：不能直接访问从基类继承的任何成员。

不知道为什么，你要是公有继承 `juce::midiInputCallback` 去使用 `this` 是会报错的。(?)



## 处理midi信号

`juce::MidiInputCallback` 内有一个纯虚函数：

```cpp
virtual void handleIncomingMidiMessage (MidiInput* source,
                                            const MidiMessage& message) = 0;
```

我们需要对他进行覆写：

```cpp
void PracticeAudioProcessorEditor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    juce::ScopedValueSetter<bool> ScopedInputFlag(isAddingMidiMessage, true); //[1]
    stateMidiKeyboardOnScreen.processNextMidiEvent(message);				  //[2]
    addMessageToList(message);												  //[3]
}
```

**[1]** `juce::ScopedValueSetter` 是一个类，用于实现仅在某作用域 (代码块) 对数据进行赋值，作用域过期后恢复原值。它的实现是在构造函数对对象进行复制并记录原来值，在析构函数又将对象赋回原来的值。这里它实现了 `isAddingMidiMessage` 状态的正确更新。

**[2]** 让键盘组件开始处理midi信号

**[3]** 刊登消息



## 处理键盘组件

### 初始化

非常简单的两个东西，一个添加监听，一个显示组件。

```cpp
stateMidiKeyboardOnScreen.addListener(this);
addAndMakeVisible(MidiKeyboardOnScreen);
```

为了让对 `this` 的监听符合类型规定，还需要在 `Editor` 下私有继承一个 `juce::MidiKeyboardListener`：

```cpp
class PracticeAudioProcessorEditor  : 
    public juce::AudioProcessorEditor,
    private juce::MidiInputCallback,
    private juce::MidiKeyboardStateListener
{
    //...
}
```

### 让组件处理midi信号

`juce::MidiKeyboardComponent` 下有两个纯虚函数：

```cpp
/* juce::MidiKeyboardState.h */
virtual void handleNoteOn (MidiKeyboardState* source,
                           int midiChannel, int midiNoteNumber, float velocity) = 0;
virtual void handleNoteOff (MidiKeyboardState* source,
                            int midiChannel, int midiNoteNumber, float velocity) = 0;
```

就是让你决定怎么处理信号。

我们这样覆写：

```cpp
void PracticeAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState* source, 
                                                int midiChannel, 
                                                int midiNoteNumber, 
                                                float velocity)
{
    if (!isAddingMidiMessage) {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        addMessageToList(m);
    }
}

void PracticeAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState* source, 
                                                 int midiChannel, 
                                                 int midiNoteNumber, 
                                                 float velocity)
{
    if (!isAddingMidiMessage) {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        addMessageToList(m);
    }
}
```

添加 `if (!isAddingMidiMessage)` 的判断是因为我们显然不想让键盘组件打扰外接设备。



还需要添加 `juce::MidiBuffer` 和 `juce::Timer` 来缓存midi和回调midi，详见[创建Midi数据.md](../创建Midi数据/创建Midi数据.md)。



## 指针安全

当你按照上面的步骤写完代码后，你运行时会发现数组越界。。。

我现在还没有找到解决方法，下周回来再写。尼玛《核按钮》就是个姬霸，月考也是。



[2022/4/14 Thur.]

尼玛真行！我花了一个上午没听网课，吃午饭时去 JUCE Forum 上看了下就解决了：
[Problems using MidiKeyboardComponent (resolved!) - General JUCE discussion - JUCE](https://forum.juce.com/t/problems-using-midikeyboardcomponent-resolved/12189)

真正的问题是：

> `MidiKeyboardComponent` 在成员初始化列表中开始初始化时调用已声明的 `MidiKeyboardState` ，但是实际上按声明顺序来看，`MidiKeyboardState` 害妹开始初始化，所以它的 `.listeners` 数组是乱码 (有可能它是在栈上定义的)。
>
> 详细来说，在 `MidiKeyboardComponent` 初始化中必须对 `state` 进行绑定并 `addListener(...)` ，于是调用 `MidiKeyboardState` 私有成员 `.listeners` 的 `.add(...)` 。而 `.add(...)` 又需要查重，也就是用到 `addIfNotAlreadyThere(...)`，这个函数又需要遍历数组 `.listeners` ，而此时数组**指针为空**，也就是错误地引用悬空指针，于是 `0xFFFFFFFF` 。

解决方法是让 `MidiKeyboardState` 先初始化，也就是在声明顺序上先声明 `MidiKeyboardState` 。

这是一个关于[**成员初始化列表**](../../../C++/成员初始化列表.md)的大坑。。。

虽然说 `.h ` 的声明就是为了让你在 `.cpp` 里为所欲为，但有时还是要注意初始化顺序啊啊啊！
