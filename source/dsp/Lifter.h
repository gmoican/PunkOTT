#pragma once

#include "juce_dsp/juce_dsp.h"

/**
 * @class Lifter
 * @brief Implements a standard, stable, feed-forward upward compressor.
 *
 * All core dynamics processing, including sidechain detection, gain computation,
 * and envelope smoothing, is contained here.
 */
class Lifter
{
public:
    Lifter();
    ~Lifter() = default;

    void prepare(double sampleRate, int totalNumChannels);
    
    float calculateTimeCoeff(float sampleRate, float time_ms);
    
    void updateRatio(float newRatio);
    void updateRange(float newRange);
    void updateKnee(float newKnee);
    void updateAttack(float sampleRate, float newAttMs);
    void updateRelease(float sampleRate, float newRelMs);
    void updateMakeUp(float newMakeUp_dB);
    
    float getGainAddition();
    
    /**
     * @brief Processes the audio buffer in-place, applying the upward compression.
     *
     * @param processedBuffer The buffer containing the dry signal (post-gate). This buffer
     * is overwritten with the fully wet, compressed signal.
     */
    void process(juce::AudioBuffer<float>& processedBuffer);
    // void processFB(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Internal State ---
    std::vector<float> envelope; // Stores the current applied linear gain factor
    
    // Parameters used for the current block
    float ratio = 4.0f;             // Linear ratio (e.g., 4.0 for 4:1)
    float rangedB = -40.0f;         // Threshold in dB
    float kneedB = 6.0f;            // Knee width in dB
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)
    float makeUpGaindB = 0.0f;      // Compensation gain after the compression takes place
    float currentGA_dB = 0.0f;      // Gain reduction (in dB) being applied currently

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lifter)
};
