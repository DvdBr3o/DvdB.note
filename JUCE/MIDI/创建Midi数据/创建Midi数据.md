# 创建Midi数据

## 序

基于：[JUCE: Tutorial: Create MIDI data](https://docs.juce.com/master/tutorial_midi_message.html)

本节将会：

+ 生成 `noteOn` 和 `noteOff` 信号
+ 使用 `juce::MidiBuffer` 加载midi信号
+ 使用 `juce::Timer` 回调midi信号，让midi信号随正确的时间行进被读取
+ [OPTIONAL] 刊登midi信号



## 代码

函数修改全部在 `Editor` 中：

- [PluginEditor.h](PluginEditor.h)
- [PluginEditor.cpp](PluginEditor.cpp)

你可能会用到的刊登midi信号的代码：

+ [midi常用刊登代码.md](midi常用刊登代码.md)



## 生成一个midi信号

### 用 noteOn 生成 midi 开始信号

我们所说的midi信号，在 `juce` 中就是 `juce::MidiMessage` 。

想要生成一个midi信号，需要用到 `juce::MidiMessage::noteOn(...)` ，

它在库中定义如下：

```cpp
/* juce_MidiMessage.cpp */
MidiMessage MidiMessage::noteOn (const int channel, const int noteNumber, const uint8 velocity) noexcept
{
    jassert (channel > 0 && channel <= 16);
    jassert (isPositiveAndBelow (noteNumber, 128));

    return MidiMessage (MidiHelpers::initialByte (0x90, channel),
                        noteNumber & 127, MidiHelpers::validVelocity (velocity));
}
```

可以看到，`noteOn` 本身是一个 `MidiMessage` 型。

### 用noteOff 终止 midi 信号

midi信号的生成也需要对应一个终止信号。

事实上，`MidiMessage` 就是在钢琴卷帘上的特定音符轨道添加时间戳，`noteOn` 信号往后乐器就在此音高音符上持续发音，直到到达一个 `noteOff` 时间戳。

生成终止信号需要用到 `juce::MidiMessage::noteOff`。

事实上，**`noteOff` 是一个力度为零的 `noteOn` 信号**。

### 实操

由此，我们可以编写一个自定义的 `setNoteNumber` 函数，来生成一个midi信号：

```cpp
void PracticeAudioProcessorEditor::setNoteNumber(int MidiChannel, 
                                                 int NoteNum, 
                                                 juce::uint8 NoteVel, 
                                                 double NoteDuration)
{
    auto message = juce::MidiMessage::noteOn(MidiChannel, NoteNum, NoteVel);//[1]
    message.setTimeStamp(juce::Time::getMillisecondCounterHiRes()*0.001-startTime);//[2]
    addMessageToBuffer(message);//[3]

    auto messageOff = juce::MidiMessage::noteOff(MidiChannel, NoteNum);
    messageOff.setTimeStamp(message.getTimeStamp() + NoteDuration);
    addMessageToBuffer(messageOff);
}
```

**[1]** 生成一个 `noteOn` 信号

**[2]** 设置 `noteOn` 信号的时间戳为当前时间

**[3]** 把信号加载到buffer上



## 把midi信号加载到缓冲区buffer

生成的midi信号需要加载到缓冲区buffer上才能起作用，这需要用到 `juce::MidiBuffer` 。

### 创建缓冲区

`juce::MidiBuffer` 是一个可实例化的类，在 `Editor` 中定义一个缓冲区实例以及相关参数：

```cpp
juce::MidiBuffer bufferMidi;//[1]
double SampleRate = 44100.0;//[2]
int previousSampleNumber = 0;//[3]
double startTime;//[4]
```

**[1]** 创建缓冲区

**[2]** 设置采样率

**[3]** 初始化 **上一次读到的采样数序号**[^1]。

**[4]** 设置初始时间，用于获取事件的相对时间。

[^1]: 采样数序号，指线性数据数组的数组。计算机中处理音频是离散化的，所以当对midi信号进行回调callback时，需要将时间戳转化成数组下标，这下标也就被叫做 **采样数序号SampleNumber** 。而所谓previous，就是在多线程循环调用到callback时，只需要处理上一次callback结束到当前时间这一时间段的midi信号，所以要记录一下上一次处理到了哪里。 

### 把midi信号加载到缓冲区buffer

自定义一个函数 `addMessageToBuffer(...)` ：

```cpp
void PracticeAudioProcessorEditor::addMessageToBuffer(const juce::MidiMessage& message)
{
    int SampleNumber = int(message.getTimeStamp() * SampleRate);
    bufferMidi.addEvent(message, SampleNumber);
}
```



## midi信号的回调callback

回调callback，实际上是一个一直在走的 `Timer` ，它负责随正常时间行进去读midi事件的过程。

### 创建回调Timer

让 `Editor` 类继承于 `juce::Timer` 类，并将基类成员作为自己的私有成员：

```cpp
class PracticeAudioProcessorEditor  : 
    public juce::AudioProcessorEditor,
    private juce::Timer
{
	//...
}
```

*[C++：private继承与public继承___kingzone__的专栏-CSDN博客_c++ public继承和private继承的区别](https://blog.csdn.net/kingzone_2008/article/details/10066181)

### 定义Timer回调

`juce::Timer` 类中的 `juce::Timer::timerCallback()` 函数是**纯虚函数**，因此需要我们去覆写：

```cpp
void PracticeAudioProcessorEditor::timerCallback() override
{
    /*获取当前采样数序号*/
    auto currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
    int currentSample = currentTime * SampleRate;

    /*遍历midi缓冲区*/
    for (const auto& metadata : bufferMidi) {
        //超过当前时间Timer停止工作，等待下一次被调用，防止timer超前读midi
        if (metadata.samplePosition > currentSample)
            break;

        //读midi事件，刊登到信息板(一个自定义的、人类可读的juce::TextEditor)上
        auto message = metadata.getMessage();
        message.setTimeStamp(metadata.samplePosition / SampleRate);
        addMessageToList(message);
    }

    //释放已读的缓冲区，防内存泄漏？
    bufferMidi.clear(previousSampleNumber, currentSample - previousSampleNumber);
    previousSampleNumber = currentSample;
}
```

### 让Timer开始工作

在 `Editor` 的构造函数中添加：

```cpp
PracticeAudioProcessorEditor::PracticeAudioProcessorEditor (PracticeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //...
    startTimer(1);
}
```





## midi信号刊登 [OPTIONAL]

### 初始化

需要一个 `juce::TextEditor` ：

```cpp
juce::TextEditor textMidiInfo;
```

初始化：

```cpp
PracticeAudioProcessorEditor::PracticeAudioProcessorEditor (PracticeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //...
    textMidiInfo.setBounds(400, 5, 395, 590);
    textMidiInfo.setMultiLine(true);
    textMidiInfo.setReturnKeyStartsNewLine(true);
    textMidiInfo.setReadOnly(true);
    textMidiInfo.setScrollbarsShown(true);
    textMidiInfo.setCaretVisible(false);
    textMidiInfo.setPopupMenuEnabled(true);
    textMidiInfo.setColour(juce::TextEditor::backgroundColourId, 			        							   juce::Colour(0x32ffffff));
    textMidiInfo.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    textMidiInfo.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));
    addAndMakeVisible(textMidiInfo);
    //...
}
```

如果你想用 `juce::Label` 来实现这种功能，也没什么大碍。

### 刊登函数

想要在 `juce::TextEditor` 上刊登信息，需要移动光标 `Caret` 到末尾，再添加内容。

具体写在一个自定义函数 `logMessage(...)` 中：

```cpp
void PracticeAudioProcessorEditor::logMessage(const juce::String& message, 				    										   juce::TextEditor& obj)
{
    obj.moveCaretToEnd();//[1]
    obj.insertTextAtCaret(message + juce::newLine);//[2]
}
```

[1] 将光标移到末尾。

[2] 在光标处添加内容。

### 将midi信号进行刊登

用到自定义函数 `logMessageToList(...)` ：

```cpp
void PracticeAudioProcessorEditor::addMessageToList(const juce::MidiMessage& message)
{
    auto Time = message.getTimeStamp();
    int hours = int(Time / 3600) % 24;
    int minutes = int(Time / 60) % 60;
    int seconds = int(Time) % 60;
    int milliseconds = int(Time * 1000) % 1000;

    juce::String timeCode = juce::String::formatted("%02d:%02d:%02d.%03d",
                                                    hours,
                                                    minutes,
                                                    seconds,
                                                    milliseconds);

    logMessage(timeCode + " - " + message.getDescription(), textMidiInfo);
}
```

然后在你的 [`timerCallback()` 函数](#定义Timer回调)里用到此函数，就实现了刊登❀~

