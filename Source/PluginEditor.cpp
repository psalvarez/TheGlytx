/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
TheGlytxAudioProcessorEditor::TheGlytxAudioProcessorEditor (TheGlytxAudioProcessor& p)
    : AudioProcessorEditor (&p),
    cutoffFreqLabel_("", "Tone"), //Should change this label depending on how it affects perception
    coarseGapLabel_("", "Coarse Distortion Amount:"),
    fineGapLabel_("", "Fine Distortion Amount:"),
    dryWetLabel_("", "Dry/Wet Mix"),
    modeLabel_("", "Thick Distortion"),
    processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 400);
    
    addAndMakeVisible(&cutoffFreqSlider_);
    cutoffFreqSlider_.setSliderStyle(Slider::Rotary);
    cutoffFreqSlider_.addListener(this);
    cutoffFreqSlider_.setRange(20.0, 18000.0, 0.1);
    
    addAndMakeVisible(&fineGapSlider_);
    fineGapSlider_.setSliderStyle(Slider::Rotary);
    fineGapSlider_.addListener(this);
    fineGapSlider_.setRange(1, 25, 1);
    
    addAndMakeVisible(&coarseGapSlider_);
    coarseGapSlider_.setSliderStyle(Slider::Rotary);
    coarseGapSlider_.addListener(this);
    coarseGapSlider_.setRange(0, 500, 25);
    
    addAndMakeVisible(&dryWetSlider_);
    dryWetSlider_.setSliderStyle(Slider::Rotary);
    dryWetSlider_.addListener(this);
    dryWetSlider_.setRange(0, 100, 1);
    
    addAndMakeVisible(&modeButton_);
    modeButton_.addListener(this);
    
    cutoffFreqLabel_.attachToComponent(&cutoffFreqSlider_, true);
    fineGapLabel_.attachToComponent(&fineGapSlider_, true);
    coarseGapLabel_.attachToComponent(&coarseGapSlider_, true);
    dryWetLabel_.attachToComponent(&dryWetSlider_, true);
    modeLabel_.attachToComponent(&modeButton_, true);
    
    startTimer(50);
}

TheGlytxAudioProcessorEditor::~TheGlytxAudioProcessorEditor()
{
}

//==============================================================================
void TheGlytxAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    /*g.drawFittedText ("Beware, b**ch. Here comes The Glytx", getLocalBounds(), Justification::centred, 1);*/
}

void TheGlytxAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    fineGapSlider_.setBounds(100, 100, 150, 40);
    coarseGapSlider_.setBounds(450, 100, 150, 40);
    cutoffFreqSlider_.setBounds(100, 300, 150, 40);
    dryWetSlider_.setBounds(500, 300, 150, 40);
    modeButton_.setBounds(350, 300, 150, 40);
}

//==============================================================================
void TheGlytxAudioProcessorEditor::timerCallback() {
    cutoffFreqSlider_.setValue(processor.cutoffFreq_, dontSendNotification);
    fineGapSlider_.setValue(processor.fineGap_, dontSendNotification);
    coarseGapSlider_.setValue(processor.coarseGap_, dontSendNotification);
    dryWetSlider_.setValue(processor.dryWetMix_, dontSendNotification);

    if(processor.mode_ == DistFilter::mThick) { //Toggle active when thick mode
        modeButton_.setToggleState(true, dontSendNotification);
    } else if (processor.mode_ == DistFilter::mFine) {
        modeButton_.setToggleState(false, dontSendNotification);
    }
}

void TheGlytxAudioProcessorEditor::sliderValueChanged (Slider* slider) {
    if (slider == &cutoffFreqSlider_) {

        processor.setParameterNotifyingHost(TheGlytxAudioProcessor::kCutoffFreq, (float)cutoffFreqSlider_.getValue());
        
    } else if (slider == &fineGapSlider_){
        
        processor.setParameterNotifyingHost(TheGlytxAudioProcessor::kFineGap, (float)fineGapSlider_.getValue());
        
    } else if (slider == &coarseGapSlider_) {
        
        processor.setParameterNotifyingHost(TheGlytxAudioProcessor::kCoarseGap, (float)coarseGapSlider_.getValue());
        
    } else if (slider == &dryWetSlider_) {
        
        processor.setParameterNotifyingHost(TheGlytxAudioProcessor::kDryWet, (float)dryWetSlider_.getValue());
        
    }
}

void TheGlytxAudioProcessorEditor::buttonClicked(Button* button) {
    if(button == &modeButton_) { //This 'if' is not actually necessary, just in case more buttons are added
        processor.setMode(modeButton_.getToggleState());
    }
}
