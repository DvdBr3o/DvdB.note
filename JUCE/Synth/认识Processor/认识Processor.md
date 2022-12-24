# 认识Processor

## 序

在 [`MIDI` 篇](../../MIDI) 一章包括其他不在教程中的更基础的操作中，你已经认识了 `ProcessorEditor` 的基本使用。

现在我们从 `Synth` 合成开始了解 `vst` 的核心： `Processor` 。

本篇主要介绍：

+ `Processor` 的基本构成



## 总括

这是 `Processor.h` 中 `Processor` 的声明：

```cpp
class PracticeAudioProcessor  : public juce::AudioProcessor
{
public:
    //==[0]=========================================================================
    PracticeAudioProcessor();
    ~PracticeAudioProcessor() override;

    //==[1]=========================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==[2]=========================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==[3]=========================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==[4]=========================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==[5]=========================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PracticeAudioProcessor)
};
```

**[0]** 构造函数和析构函数。

**[1]** 包括三个重要方法：

+ `prepareToPlay` 在每次开始播放操作时被调用，用于初始化，相当于小构造。
+ `processBlock` 用于规定处理音频信号的方法。
+ `releaseResources` 在每次结束播放操作时被调用，用于释放资源，相当于小析构。

**[2]** 用于构建相应的 `ProcessorEditor` ，顺便记录有无对应 `Editor` 。

**[3]** 用于返回关于 `Processor` 的基本信息。

**[4]** 关于 `Program` 的 `set` , `get` , `change` 。

**[5]** 关于 `State` 的读写，这将是 [`Plugin` 篇](../../Plugin) 的重点。
