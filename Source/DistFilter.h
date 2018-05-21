/*
 ==============================================================================
 
 DistFilter.h
 Created: 13 Mar 2018 9:40:19am
 Author:  Pedro Sánchez Álvarez
 
 ==============================================================================
 */

#ifndef __DistFilter_H_6E48F605__
#define __DistFilter_H_6E48F605__

#define _USE_MATH_DEFINES
#include "../JuceLibraryCode/JuceHeader.h"

#define FILTER_ORDER 2
//==============================================================================
/**
 * This class implements a biquad parametric EQ section according
 * to the equations in the Reiss and McPherson text.
 */

class DistFilter
{
public:
    DistFilter();
    ~DistFilter();
    
    //==============================================================================
    
    void setFilter(const float freqCutoff, const float sampleRate);
	
    void applyFilter(float* samples, int numSamples, int reinitGap, bool reinit); //This applies the filter processing to a set of samples. Reinit bool is used to bypass the distortion and use it only as a filter
    
    void passTime(); //Increments sampleCount
    int getTime();
    void resetTime();
    
    void changeMode();
    
    enum Mode { //This type defines the type of filter being used
        mThick,
        mFine, //WATCH OUT! THIS IS NOT FINE REINITIALISATION GAP! IT'S THE OPPOSITE OF THICK MODE!
        mNumModes
    };
    
    void setFilterMode (Mode newMode);
    
private:
    
    //==============================================================================
    
    //Functions
    void initFilterMemory(); //Initialises filter memory in the specified mode
    void setLowPass(const float freqCutoff, const float sampleRate); //This calculates the coefficients for a low pass filter given a sample rate and cutoff frequency
    void setHighPass(const float freqCutoff, const float sampleRate); //This calculates the coefficients for a high pass filter given a sample rate and cutoff frequency
    
    //Filter coefficients
    float a[FILTER_ORDER];
    float b[FILTER_ORDER + 1];
    
    //Last input and output samples (to preserve state between calls)
    float lastIn[FILTER_ORDER + 1];
    float lastOut[FILTER_ORDER + 1];
    
    //Sample counter (to keep track of time)
    int sampleCount;
    
    //Distortion mode
    Mode filterMode_;
    
	JUCE_LEAK_DETECTOR (DistFilter);
};


#endif // __DistFilter_H_6E48F605__
