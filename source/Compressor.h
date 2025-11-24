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
    Compressor();
    ~Compressor() = default;
    
    float calculateTimeCoeff(float sampleRate, float time_ms);

    void prepare(double sampleRate, int totalNumChannels);
    void updateRatio(float newRatio);
    void updateThres(float newThres);
    void updateKnee(float newKnee);
    void updateAttack(float sampleRate, float newAttMs);
    void updateRelease(float sampleRate, float newRelMs);

    /**
     * @brief Processes the audio buffer in-place, applying downward compression.
     *
     * @param processedBuffer The buffer containing the signal to be compressed. 
     * This buffer is overwritten with the fully compressed signal.
     */
    void processFF(juce::AudioBuffer<float>& processedBuffer);
    void processFB(juce::AudioBuffer<float>& processedBuffer);

private:
    // --- Internal State ---
    std::vector<float> envelope;    // Stores the current applied linear gain factor
    float ratio = 4.0f;             // Linear ratio (e.g., 4.0 for 4:1)
    float thresdB = -12.0f;         // Threshold in dB
    float kneedB = 6.0f;            // Knee width in dB
    float attackCoeff = 0.0f;       // Smoothing coefficient (Attack)
    float releaseCoeff = 0.0f;      // Smoothing coefficient (Release)

    // --- Prevent copy and move ---
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)
};
