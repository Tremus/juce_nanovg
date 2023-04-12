#pragma once
#include "PluginProcessor.h"
// #include "MainComponent.h"
#include "NVGDemoComponent.h"
#include "FramebufferTest.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    NanoVGGraphics graphics;
    // MainComponent comp;
    NVGDemoComponent comp;
    // FramebufferTest comp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
