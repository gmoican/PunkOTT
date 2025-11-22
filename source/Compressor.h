#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Compressor
 * @brief Implements a standard, stable, feed-forward downward compressor.
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Compressor
{
public:
    // // --- DSP-ready Parameter Struct ---
    // // Note: The processor is responsible for converting ms to coefficients and
    // // dB values to linear scale before passing them here.
    // struct CompParams
    // {
    //     float ratio = 4.0f;             // Linear ratio (e.g., 4.0 for 4:1)
    //     float thresholdLinear = 0.25f;  // Linear threshold (e.g., 0.25 for -12 dBFS)
    //     float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    //     float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)
    // };

    Compressor();
    ~Compressor() = default;

    void prepare(double sampleRate, int totalNumChannels);
    void update(float thresLin, float attCoeff, float relCoeff);

    /**
     * @brief Processes the audio buffer in-place, applying downward compression.
     *
     * @param processedBuffer The buffer containing the signal to be compressed. 
     * This buffer is overwritten with the fully compressed signal.
     */
    void process(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Internal State ---
    std::vector<float> envelope; // Stores the current applied linear gain factor
    // CompParams currentParams;
    float ratio = 4.0f;             // Linear ratio (e.g., 4.0 for 4:1)
    float thresholdLinear = 0.25f;  // Linear threshold (e.g., 0.25 for -12 dBFS)
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)
};
