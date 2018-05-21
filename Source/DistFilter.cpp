/*
 ==============================================================================
 
 DistFilter.cpp
 Created: 13 Mar 2018 9:40:19am
 Author:  Pedro Sánchez Álvarez
 
 ==============================================================================
*/

#include "DistFilter.h"

#define FILTER_ORDER 2

//====================PUBLIC================================================

DistFilter::DistFilter() //Initialise coefficients and previous inputs and outputs to avoid glitches
{
    for(int i = 0; i < FILTER_ORDER; ++i) {
        a[i] = 0;
    }
    for(int i = 0; i < FILTER_ORDER + 1; ++i) {
        b[i] = 0;
        lastIn[i] = 0;
        lastOut[i] = 0;
    }
    filterMode_ = mThick;
};

DistFilter::~DistFilter()
{
};

//==============================================================================

//This calculates the coefficients for a low pass filter given the sample rate and cutoff frequency
void DistFilter::setLowPass(const float freqCutoff, const float sampleRate)
{
    initFilterMemory(); //Initialise stored samples
    
    float phi = M_PI*freqCutoff/sampleRate;
    float omega = M_PI*freqCutoff;
    float kappa = omega/tan(phi);
    float delta = kappa*kappa + omega*omega + (2*kappa)*omega;
    
    b[0] = (omega*omega)/delta;
    b[1] = 2*b[0];
    b[2] = b[0];
    
    a[0] = (-2*kappa*kappa + 2*omega*omega)/delta;
    a[1] = (-2*kappa*omega + 2*omega*omega + kappa*kappa)/delta;
    
}

//This calculates the coefficients for a high pass filter given the sample rate and cutoff frequency
void DistFilter::setHighPass(const float freqCutoff, const float sampleRate)
{
    initFilterMemory(); //Initialise stored samples
    
    float phi = M_PI*freqCutoff/sampleRate;
    float omega = M_PI*freqCutoff;
    float kappa = omega/tan(phi);
    float delta = kappa*kappa + omega*omega + (2*kappa)*omega;

    b[0] = (kappa*kappa)/delta;
    b[1] = -2*b[0];
    b[2] = b[0];
    
    a[0] = (-2*kappa*kappa + 2*omega*omega)/delta;
    a[1] = (-2*kappa*omega + 2*omega*omega + kappa*kappa)/delta;
}

void DistFilter::setFilter(const float freqCutoff, const float sampleRate)
{
    switch (filterMode_) { //Use Low Pass Filter in Thick mode, High Pass in thin mode
        case mThick:
            setLowPass(freqCutoff, sampleRate);
            break;
            
        case mFine:
            setHighPass(freqCutoff, sampleRate);
            break;
            
        default:
            break;
    }
}

void DistFilter::applyFilter(float *samples, int numSamples, int reinitGap, bool reinit)
{
    for (int n = 0; n < numSamples; ++n) {
        
        //Read current input sample
        lastIn[0] = samples[n];
        
        //Filter operation
        lastOut[0] = lastIn[0] * b[0] + lastIn[1] * b[1] + lastIn[2] * b[2] - lastOut[1] * a[0] - lastOut[2] * a[1];
        
        //Output Sample
        samples[n] = lastOut[0];
        
        //Update previous inputs and outputs
        lastIn[2] = lastIn[1];
        lastIn[1] = lastIn[0];
        
        lastOut[2] = lastOut[1];
        lastOut[1] = lastOut[0];
        
        if(reinit && (sampleCount >= reinitGap)) { //If distortion is not bypassed and the count goes beyond the specified interval, then reinitialise memory
            initFilterMemory(); //This should have modes on a more advanced stage
            resetTime();
        } else if (sampleCount < reinitGap){ //Count if not reinitialising
            passTime();
        }
    }
}

void DistFilter::setFilterMode (Mode newMode)
{
    filterMode_ = newMode;
}

//==============================================================================
void DistFilter::passTime()
{
    ++sampleCount;
}

int DistFilter::getTime()
{
    return sampleCount;
}

void DistFilter::resetTime()
{
    sampleCount = 0;
}

//====================PRIVATE================================================
void DistFilter::initFilterMemory()
{
    lastIn[2] = 0;
    lastIn[1] = 0;
    
    lastOut[2] = 0;
    lastOut[1] = 0;
}

