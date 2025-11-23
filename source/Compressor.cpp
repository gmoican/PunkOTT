#include "Compressor.h"

// Define a floor for magnitude detection to prevent log(0) and instability.
static constexpr float minMagnitude = 0.00001f;     // Approx -100 dB
static constexpr float makeUpGainLinear = 1.0f;     // 0 dB TODO: Change if needed
static constexpr float makeUpGainDB = 0.0f;         // 0 dB TODO: Change if needed

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

void Compressor::updateRatio(float newRatio)
{
    ratio = newRatio;
}

void Compressor::updateThres(float newThres)
{
    thresdB = newThres;
}

void Compressor::updateKnee(float newKnee)
{
    kneedB = newKnee;
}

void Compressor::updateAttack(float newAttCoeff)
{
    attackCoeff = newAttCoeff;
}

void Compressor::updateRelease(float newRelCoeff)
{
    releaseCoeff = newRelCoeff;
}

// =========================================================================
// FEED-FORWARD TOPOLOGY (processFF)
// Sidechain detects signal BEFORE gain is applied.
// =========================================================================
void Compressor::processFF(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();
    
    // --- Pre-calculations ---
    // Calculate the slope for gain reduction: 1 - (1 / Ratio)
    const float compressionSlope = 1.0f - (1.0f / ratio);
    const float kneeStart = thresdB - (kneedB / 2.0f);
    const float kneeEnd = thresdB + (kneedB / 2.0f);
    
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
            // Hard Knee Equation
            // float targetGR_dB = (inputDB > thresdB) ? (inputDB - thresdB) * compressionSlope : 0.0f;
            
            // Soft Knee Equation
            float targetGR_dB = 0.0f;
            if (inputDB > kneeEnd)
            {
                targetGR_dB = (inputDB - thresdB) * compressionSlope;
            }
            else if (inputDB > kneeStart)
            {
                const float x = inputDB - kneeStart;
                targetGR_dB = compressionSlope / (2.0f * kneedB) * (x * x);
            }
            
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

// =========================================================================
// FEED-BACK TOPOLOGY (processFB)
// Sidechain detects signal AFTER gain is applied.
// =========================================================================
void Compressor::processFB(juce::AudioBuffer<float>& processedBuffer)
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
            
            // 1. APPLY CURRENT GAIN (Based on previous sample's envelope state)
            const float finalGainDB = currentGR_dB + makeUpGainDB;
            const float finalGainLinear = juce::Decibels::decibelsToGain(finalGainDB);
            float outputSample = inputSample * finalGainLinear;
            
            // 2. SIDECHAIN: Detect magnitude of the OUTPUT (Feed-Back)
            float magnitude = std::abs (outputSample);
            magnitude = std::max(magnitude, minMagnitude);
            const float outputDB = juce::Decibels::gainToDecibels(magnitude);
            
            // 3. TARGET GAIN REDUCTION COMPUTATION (In dB)
            float targetGR_dB = (outputDB > thresdB) ? (outputDB - thresdB) * compressionSlope : 0.0f;
            
            // 4. ENVELOPE SMOOTHING (In dB Domain)
            float alpha = (targetGR_dB > -currentGR_dB) ? attackCoeff : releaseCoeff;
            
            const float smoothedReductionAmount = (alpha * -currentGR_dB) + ((1.0f - alpha) * targetGR_dB);
            
            currentGR_dB = -smoothedReductionAmount;
            
            // 5. STORE OUTPUT (The output sample calculated in step 1)
            channelData[sample] = outputSample;
        }
        
        envelope[channel] = currentGR_dB;
    }
}
