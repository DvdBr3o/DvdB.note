/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PracticeAudioProcessorEditor  : 
    public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    PracticeAudioProcessorEditor (PracticeAudioProcessor&);
    ~PracticeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void setNoteNumber(int, int, juce::uint8, double);
    void addMessageToList(const juce::MidiMessage&);
    void addMessageToBuffer(const juce::MidiMessage&);
    void logMessage(const juce::String&, juce::TextEditor&);
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PracticeAudioProcessor& audioProcessor;

    juce::TextEditor textMidiInfo;
    double startTime;

    juce::TextButton buttonGenerateMidi;

    juce::MidiBuffer bufferMidi;
    double SampleRate = 44100.0;
    int previousSampleNumber = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PracticeAudioProcessorEditor)
};