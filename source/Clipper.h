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
    float applySoftClipper(float sample);
    float applyHardClipper(float sample);
    float applyTanhClipper(float sample);
    float applyATanClipper(float sample);
    
    // Buffer processing
    void applySoftClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyHardClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyTanhClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyATanClipper(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Clipper)
};
