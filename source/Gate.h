#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Gate
 * @brief Implements a standard, stable, feed-forward downward expansor.
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Gate
{
public:
    Gate();
    ~Gate() = default;

    void prepare(double sampleRate, int totalNumChannels);
    
    float calculateTimeCoeff(float sampleRate, float time_ms);

    void updateRatio(float newRatio);
    void updateThres(float newThres);
    void updateKnee(float newKnee);
    void updateAttack(float sampleRate, float newAttMs);
    void updateRelease(float sampleRate, float newRelMs);

    /**
     * @brief Processes the audio buffer in-place, applying the downward expansion.
     *
     * @param processedBuffer The buffer containing the dry signal (post-input-gain).
     * This buffer is overwritten with the fully wet, expanded/gated signal.
     */
    void process(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Internal State ---
    std::vector<float> envelope; // Stores the current applied linear gain factor
    
    // Parameters used for the current block
    float ratio = 6.0f;             // Linear ratio
    float thresdB = -80.0f;         // Threshold in dB
    float kneedB = 9.0f;            // Knee width in dB
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gate)
};
