#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setOpaque(false);

    if (auto* window = juce::TopLevelWindow::getTopLevelWindow(0))
    {
        window->setUsingNativeTitleBar(true);
    }

    // addChildComponent(mainComp);
    // addChildComponent(demoComp);
    addChildComponent(cacheTest);
    setColour(juce::ResizableWindow::ColourIds::backgroundColourId, juce::Colours::black);
    

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    getConstrainer()->setMaximumSize(1000, 600);
    setSize (1000, 600);
    setResizable(true, true);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================

void AudioPluginAudioProcessorEditor::resized()
{
    // mainComp.setBounds(getLocalBounds());
    // demoComp.setBounds(0, 0, getWidth(), getHeight());
    cacheTest.setBounds(getLocalBounds());
}
//==============================================================================
