#include "Compressor.h"

// Define a floor for magnitude detection to prevent log(0) and instability.
static constexpr float minMagnitude = 0.00001f;     // Approx -100 dB
static constexpr float makeUpGainLinear = 1.0f;     // Approx +12dB

Compressor::Compressor()
{
    // Initialize envelope vector size to 0
}

void Compressor::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);

    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 0.0f); // Initialize gain to 0 dB
}

void Compressor::update(float newThresdB, float newAttCoeff, float newRelCoeff)
{
    thresdB = newThresdB;
    attackCoeff = newAttCoeff;
    releaseCoeff = newRelCoeff;
}

void Compressor::process(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();
    
    // --- Pre-calculations ---
    // Calculate the slope for gain reduction: 1 - (1 / Ratio)
    const float compressionSlope = 1.0f - (1.0f / ratio); 
    
    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 0.0f);

    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        float currentGR_dB = envelope[channel]; 

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample];
            float magnitude = std::abs (inputSample);
            
            // Clamp magnitude to prevent log(0)
            magnitude = std::max(magnitude, minMagnitude);
            
            // 1. SIDECHAIN: Convert magnitude to dB
            const float inputDB = juce::Decibels::gainToDecibels(magnitude);

            // 2. TARGET GAIN REDUCTION (in dB)
            float targetGR_dB = (inputDB > thresdB) ? (inputDB - thresdB) * compressionSlope : 0.0f;
            
            // 3. ENVELOPE SMOOTHING (in dB)
            float alpha = (targetGR_dB > -currentGR_dB) ? attackCoeff : releaseCoeff;
            
            const float smoothedReductionAmount = (alpha * -currentGR_dB) + ((1.0f - alpha) * targetGR_dB);
            currentGR_dB = -smoothedReductionAmount;
            
            // 4. APPLY GAIN (in-place)
            const float gainReductionLinear = juce::Decibels::decibelsToGain(currentGR_dB);
            channelData[sample] = inputSample * gainReductionLinear * makeUpGainLinear;
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGR_dB;
    }
}
