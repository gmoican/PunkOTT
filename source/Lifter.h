#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * This class isolates the state (envelope) and the calculations (gain reduction,
 * attack/release smoothing) from the main PluginProcessor.
 */
class Lifter
{
public:
    // --- DSP-ready Parameter Struct ---
    // This struct holds all the values needed by the real-time processing loop.
    struct LifterParams
    {
        float rangeLinear = 1.0f;
        float downAmount = 0.5f;
        float attackCoeff = 0.0f;
        float releaseCoeff = 0.0f;
    };

    Lifter();
    ~Lifter() = default;

    /**
     * @brief Called during prepareToPlay to set sample rate and initialize state.
     */
    void prepare(double sampleRate, int totalNumChannels);

    /**
     * @brief Updates the internal parameters used for the next processing block.
     * @param newParams The structure containing the latest parameter coefficients.
     */
    void update(const LifterParams& newParams);

    /**
     * @brief Processes the audio buffer in-place, applying the upward and downward compression.
     *
     * @param processedBuffer The buffer containing the dry signal (post-gate). This buffer
     * is overwritten with the fully wet, compressed signal.
     */
    void process(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Internal State ---
    
    // Envelope: Stores the current applied gain for smoothing across blocks.
    std::vector<float> envelope;

    // Parameters used for the current block
    LifterParams currentParams;

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lifter)
};
