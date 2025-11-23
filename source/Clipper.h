#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Clipper
 * @brief Implements several types of clippers
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Clipper
{
public:
    Clipper();
    ~Clipper() = default;

    void applySoftClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyHardClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyTanhClipper(juce::AudioBuffer<float>& processedBuffer);
    void applyATanClipper(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Clipper)
};
