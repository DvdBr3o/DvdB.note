/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PracticeAudioProcessorEditor::PracticeAudioProcessorEditor (PracticeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    startTime = juce::Time::getMillisecondCounterHiRes() * 0.001;

    textMidiInfo.setBounds(400, 5, 395, 590);
    textMidiInfo.setMultiLine(true);
    textMidiInfo.setReturnKeyStartsNewLine(true);
    textMidiInfo.setReadOnly(true);
    textMidiInfo.setScrollbarsShown(true);
    textMidiInfo.setCaretVisible(false);
    textMidiInfo.setPopupMenuEnabled(true);
    textMidiInfo.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x32ffffff));
    textMidiInfo.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    textMidiInfo.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));
    addAndMakeVisible(textMidiInfo);

    buttonGenerateMidi.setBounds(20, 20, 360, 30);
    buttonGenerateMidi.setButtonText("Generate a random MIDI...");
    buttonGenerateMidi.onClick = [this] {
        juce::Range<int> Range(0, 127);
        auto& random = juce::Random::getSystemRandom();
        int Note = random.nextInt(Range);
        double Duration = random.nextDouble() * 10;
        int Velocity = random.nextInt(Range);
        int MidiChannel = random.nextInt(juce::Range<int>(0,64));
        setNoteNumber(MidiChannel, Note, Velocity, Duration);
    };
    addAndMakeVisible(buttonGenerateMidi);

    setSize (800, 600);
    startTimer(1);
}

PracticeAudioProcessorEditor::~PracticeAudioProcessorEditor()
{
}

//==============================================================================
void PracticeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    static bool keyOnce = false;
    if (!keyOnce) {
        setNoteNumber(1, 36, 100, 0.25);
        setNoteNumber(1, 38, 100, 1.0);
        setNoteNumber(1, 59, 100, 0.5);
        keyOnce = true;
    }
    
}

void PracticeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void PracticeAudioProcessorEditor::setNoteNumber(int MidiChannel, int NoteNum, juce::uint8 NoteVel, double NoteDuration)
{
    auto message = juce::MidiMessage::noteOn(MidiChannel, NoteNum, NoteVel);
    message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToBuffer(message);

    auto messageOff = juce::MidiMessage::noteOff(MidiChannel, NoteNum);
    messageOff.setTimeStamp(message.getTimeStamp() + NoteDuration);
    addMessageToBuffer(messageOff);
}

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

void PracticeAudioProcessorEditor::addMessageToBuffer(const juce::MidiMessage& message)
{
    int SampleNumber = int(message.getTimeStamp() * SampleRate);
    bufferMidi.addEvent(message, SampleNumber);
}

void PracticeAudioProcessorEditor::logMessage(const juce::String& message, juce::TextEditor& obj)
{
    obj.moveCaretToEnd();
    obj.insertTextAtCaret(message + juce::newLine);
}

void PracticeAudioProcessorEditor::timerCallback()
{
    auto currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
    int currentSample = currentTime * SampleRate;

    for (const auto& metadata : bufferMidi) {
        if (metadata.samplePosition > currentSample)
            break;

        auto message = metadata.getMessage();
        message.setTimeStamp(metadata.samplePosition / SampleRate);
        addMessageToList(message);
    }

    bufferMidi.clear(previousSampleNumber, currentSample - previousSampleNumber);
    previousSampleNumber = currentSample;

}