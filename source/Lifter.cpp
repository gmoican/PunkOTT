#include "Lifter.h"

// Define a small constant to prevent division by zero near the noise floor.
static constexpr float minMagnitude = 0.00001f; // Approx -100 dB
// Define the maximum gain factor for the upward compression stage.
// 10.0f = +20 dB boost. This prevents runaway gain and instability.
static constexpr float maxUpwardGain = 10.0f;

Lifter::Lifter()
{
    // Initialize envelope vector size to 0
}

void Lifter::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);
    
    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 1.0f); // Initialize gain to 1.0 (0 dB)

    // Reset parameters on prepare to ensure no stale coefficients are used
    currentParams = {}; 
}

void Lifter::update(const LifterParams& newParams)
{
    // Simply copy the calculated DSP-ready parameters
    currentParams = newParams;
}

void Lifter::process(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    // Cache parameters from the internal struct
    const float rangeLinear = currentParams.rangeLinear;
    const float downAmount = currentParams.downAmount;
    const float attackCoeff = currentParams.attackCoeff;
    const float releaseCoeff = currentParams.releaseCoeff;
    
    // Fixed threshold for downward compression (-12 dBFS / 0.25 linear)
    const float downThreshold = 0.25f;
    
    // Calculate the Downward Compression Ratio based on Amount (0.0 to 1.0)
    const float maxRatio = 8.0f;
    const float ratio = 1.0f + downAmount * (maxRatio - 1.0f); 

    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 1.0f);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Pointers for reading input and writing wet output
        float* channelData = processedBuffer.getWritePointer(channel);
        float currentEnvelope = envelope[channel];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample];
            float magnitude = std::abs(inputSample);
            magnitude = std::max(magnitude, minMagnitude);
            
            float G_up = 1.0f;    
            float G_down = 1.0f;  

            // 1. UPWARD COMPRESSION LOGIC (Boosting quiet signals)
            if (magnitude < rangeLinear)
            {
                // Gain to push the signal up to the Range threshold
                // Note: If magnitude is near zero, this gain can be huge.
                G_up = rangeLinear / magnitude;
                
                G_up = std::min(G_up, maxUpwardGain);
            }

            // 2. DOWNWARD COMPRESSION LOGIC (Limiting loud signals)
            if (magnitude > downThreshold)
            {
                // Calculate the amount signal is above threshold (in linear)
                float excursion = magnitude - downThreshold;
                
                // Linear gain reduction factor calculation
                G_down = downThreshold + (excursion / ratio); 
                G_down = downThreshold / G_down; // Convert magnitude scaling back to VCA gain
            }

            // 3. COMBINED TARGET GAIN & SMOOTHING
            const float targetGain = G_up * G_down;

            // Attack/Release decision logic
            float alpha = (targetGain > currentEnvelope) ? attackCoeff : releaseCoeff;

            // Simple 1-pole filter to smooth the applied gain
            currentEnvelope = (alpha * currentEnvelope) + ((1.0f - alpha) * targetGain);
            
            // Apply smoothed gain to the sample
            channelData[sample] = inputSample * currentEnvelope;
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentEnvelope; 
    }
}
