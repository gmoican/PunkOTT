#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Clipper
 * @brief Implements several types of clippers
 *
 * Contains some waveshaping functions
 * On its own, it works as a clipper
 * In combination with filters, adequate input gain and bias, it works as the core of distortion processors
 */
class Clipper
{
public:
    Clipper();
    ~Clipper() = default;

    // Sample processing
    float applySoftClipper(float sample, float gainFactor);
    float applyHardClipper(float sample, float gainFactor);
    float applyTanhClipper(float sample, float gainFactor);
    float applyATanClipper(float sample, float gainFactor);
    
    // Buffer processing
    void applySoftClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor);
    void applyHardClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor);
    void applyTanhClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor);
    void applyATanClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor);

private:
    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Clipper)
};
