# midi常用刊登代码



## `juce::TextEditor` 初始化

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



## 生成信息

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



## 刊登

```cpp
void PracticeAudioProcessorEditor::logMessage(const juce::String& message, 				    										   juce::TextEditor& obj)
{
    obj.moveCaretToEnd();//[1]
    obj.insertTextAtCaret(message + juce::newLine);//[2]
}
```

