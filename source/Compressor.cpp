#include "Compressor.h"

// Define a floor for magnitude detection to prevent log(0) and instability.
static constexpr float minMagnitude = 0.00001f; // Approx -100 dB

Compressor::Compressor()
{
    // Initialize envelope vector size to 0
}

void Compressor::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);

    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 1.0f); // Initialize gain to 1.0 (0 dB)
    // currentParams = {}; 
}

void Compressor::update(float thresLin, float attCoeff, float relCoeff)
{
    thresholdLinear = thresLin;
    attackCoeff = attCoeff;
    releaseCoeff = relCoeff;
}

void Compressor::process(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();
    
    // --- Pre-calculations ---
    
    // Calculate the threshold in dB for the sidechain analysis
    const float thresholdDB = juce::Decibels::gainToDecibels(thresholdLinear);
    
    // Calculate the slope for gain reduction: 1 - (1 / Ratio)
    // Example: Ratio=4 (4:1) -> slope = 1 - 0.25 = 0.75
    const float compressionSlope = 1.0f - (1.0f / ratio); 
    
    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 1.0f);

    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        float currentEnvelope = envelope[channel]; 

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample]; 
            float magnitude = std::abs (inputSample);
            
            // Clamp magnitude to prevent log(0)
            magnitude = std::max(magnitude, minMagnitude);
            
            // 1. SIDECHAIN: Convert magnitude to dB
            const float inputDB = juce::Decibels::gainToDecibels(magnitude);

            // 2. GAIN COMPUTATION (In dB)
            float gainReductionDB = (inputDB > thresholdDB) ? ((inputDB - thresholdDB) * compressionSlope) : 0.0f;
            
            // 3. TARGET GAIN (In Linear)
            // Convert gain reduction (positive dB) back to a linear gain factor (0.0 to 1.0)
            const float targetGain = juce::Decibels::decibelsToGain(-gainReductionDB);
            
            // 4. ENVELOPE SMOOTHING
            float alpha = (targetGain < currentEnvelope) ? attackCoeff : releaseCoeff;

            currentEnvelope = (alpha * currentEnvelope) + ((1.0f - alpha) * targetGain);
            
            // 5. APPLY GAIN (in-place)
            channelData[sample] = inputSample * currentEnvelope;
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentEnvelope; 
    }
}
