/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
TheGlytxAudioProcessor::TheGlytxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    cutoffFreq_ = 1000;
    fineGap_ = 1;
    coarseGap_ = 100;
    reinitGap_ = fineGap_ + coarseGap_;
    dryWetMix_ = 100;
    mode_ = DistFilter::mThick;
}

TheGlytxAudioProcessor::~TheGlytxAudioProcessor()
{
}

//==============================================================================
const String TheGlytxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int TheGlytxAudioProcessor::getNumParameters()
{
    return kNumParameters;
}

float TheGlytxAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kCutoffFreq:                return cutoffFreq_;
        case kFineGap:                   return fineGap_;
        case kCoarseGap:                 return coarseGap_;
        case kMode:                      return mode_;
        default:                         return 0.0f;
    }
}

void TheGlytxAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
            //Filter Parameters
        case kCutoffFreq:
            cutoffFreq_ = newValue;
            for(int channel = 0; channel < getTotalNumInputChannels(); ++channel) {
                filter_[channel].setFilter(cutoffFreq_, getSampleRate());
            }
            break;
            
        case kFineGap:
            fineGap_ = newValue;
            reinitGap_ = coarseGap_ + fineGap_;
            for(int channel = 0; channel < getTotalNumInputChannels(); ++channel)
                filter_[channel].resetTime(); //Reset timer if the interval changes
            break;
            
        case kCoarseGap:
            coarseGap_ = newValue;
            reinitGap_ = coarseGap_ + fineGap_;
            for(int channel = 0; channel < getTotalNumInputChannels(); ++channel)
                filter_[channel].resetTime(); //Reset timer if the interval changes
            break;
            
        case kDryWet:
            dryWetMix_ = newValue;
            break;
            
        default:
            break;
    }
}

void TheGlytxAudioProcessor::setMode(bool active)
{
    if(active) { //If the button is active, turn thick mode. Turn fine mode when inactive.
        mode_ = DistFilter::mThick;
    } else {
        mode_ = DistFilter::mFine;
    }
    
    for(int channel = 0; channel < getTotalNumInputChannels(); ++channel) {
        filter_[channel].setFilterMode(mode_); //Change filter mode attribute
        filter_[channel].setFilter(cutoffFreq_, getSampleRate()); //Reset filter coefficients for the new mode.
    }
}

const String TheGlytxAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case kCutoffFreq:            return "Cutoff Frequency";
        case kFineGap:               return "Fine Reinitialisation Interval";
        case kCoarseGap:             return "Coarse Reinitialisation Interval";
        case kDryWet:                return "Dry Wet Mix";
        case kMode:                  return "Distortion Mode";
        default:                     break;
    }
    
    return String::empty;
}

const String TheGlytxAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

bool TheGlytxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheGlytxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheGlytxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheGlytxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheGlytxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TheGlytxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheGlytxAudioProcessor::setCurrentProgram (int index)
{
}

const String TheGlytxAudioProcessor::getProgramName (int index)
{
    return {};
}

void TheGlytxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TheGlytxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for(int channel = 0; channel < getTotalNumInputChannels(); ++channel) {
        filter_[channel].setFilter(cutoffFreq_, sampleRate);
    }
}

void TheGlytxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheGlytxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TheGlytxAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();          // How many samples in the buffer for this block?
    
    AudioSampleBuffer outBuf, inBuf; //This might not be necessary since only the last sample is modified. But leave it for security

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    outBuf.makeCopyOf(buffer);
    inBuf.makeCopyOf(buffer); //A copy of the input is created because later the buffer will be erased
    
    reinitGap_ = coarseGap_ + fineGap_;
    for (int channel = 0; channel < totalNumInputChannels; ++channel) //Processes every Channel
    {
        float* channelData = outBuf.getWritePointer (channel); //Using a copy of the buffer to avoid modifying original data

        // ..do something to the data...
        
        filter_[channel].applyFilter(channelData, numSamples, roundToInt((float(reinitGap_)/100000.0) * getSampleRate()), true); //Distorts each channel separately
        //filter_[channel].applyFilter(channelData, numSamples, false); //Filter only, without distortion
        
        buffer.clear(channel, 0, buffer.getNumSamples());
        buffer.addFrom(channel, 0, inBuf, channel, 0, buffer.getNumSamples(), 1 - (dryWetMix_/100)); //Dry mix
        buffer.addFrom(channel, 0, outBuf, channel, 0, buffer.getNumSamples(), dryWetMix_/100); //Wet mix
    }
    //buffer.makeCopyOf(outBuf);
}

//==============================================================================
bool TheGlytxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TheGlytxAudioProcessor::createEditor()
{
    return new TheGlytxAudioProcessorEditor (*this);
}

//==============================================================================
void TheGlytxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TheGlytxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheGlytxAudioProcessor();
}
