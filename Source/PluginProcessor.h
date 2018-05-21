/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DistFilter.h"


//==============================================================================
/**
*/
class TheGlytxAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TheGlytxAudioProcessor();
    ~TheGlytxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;
    
    int getNumParameters() override;
    
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;
    void setMode(bool active);
    
    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    //Adjustable Parameters
    enum Parameters {
        kCutoffFreq = 0, //This is Tone in the UI
        kFineGap,
        kCoarseGap,
        kDryWet,
        kMode,
        kNumParameters
    };
    float cutoffFreq_; //This is the Tone parameter
    int fineGap_, coarseGap_, reinitGap_; //Defines the interval between reinitialisations
    float dryWetMix_;

    DistFilter::Mode mode_;

private:
    //==============================================================================
    DistFilter filter_[2]; //Creates an array of 2 filters (one per channel)
    //bool reinitFlag;
    //Use this bool only if a function for bypassing distortion (filter only) is included
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TheGlytxAudioProcessor)
};
