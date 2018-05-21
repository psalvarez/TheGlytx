/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class TheGlytxAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Slider::Listener,
                                    public Timer,
                                    public Button::Listener
{
public:
    TheGlytxAudioProcessorEditor (TheGlytxAudioProcessor&);
    ~TheGlytxAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged (Slider*) override;
    void buttonClicked(Button*) override;

private:
    //Parameters
    Label cutoffFreqLabel_, coarseGapLabel_, fineGapLabel_, dryWetLabel_, modeLabel_;
    Slider cutoffFreqSlider_, coarseGapSlider_, fineGapSlider_, dryWetSlider_;
    ToggleButton modeButton_;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TheGlytxAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheGlytxAudioProcessorEditor)
};
