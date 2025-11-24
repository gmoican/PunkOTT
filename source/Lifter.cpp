#include "Lifter.h"

// Define a small constant to prevent division by zero near the noise floor.
static constexpr float minMagnitude = 0.00001f; // Approx -100 dB

Lifter::Lifter()
{
    // Initialize envelope vector size to 0
}

void Lifter::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);
    
    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 0.0f); // Initialize gain to 0 dB 
}

float Lifter::calculateTimeCoeff(float sampleRate, float time_ms)
{
    return std::exp(-1.0f / (sampleRate * (time_ms / 1000.0f)));
}

void Lifter::updateRatio(float newRatio)
{
    ratio = newRatio;
}

void Lifter::updateRange(float newRange)
{
    rangedB = newRange;
}

void Lifter::updateKnee(float newKnee)
{
    kneedB = newKnee;
}

void Lifter::updateAttack(float sampleRate, float newAttMs)
{
    attackCoeff = calculateTimeCoeff(sampleRate, newAttMs);
}

void Lifter::updateRelease(float sampleRate, float newRelMs)
{
    releaseCoeff = calculateTimeCoeff(sampleRate, newRelMs);
}

void Lifter::process(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();
    
    // --- Pre-calculations ---
    // Calculate the slope for gain reduction: 1 - (1 / Ratio)
    const float compressionSlope = 1.0f - (1.0f / ratio);
    const float kneeStart = rangedB - (kneedB / 2.0f);
    const float kneeEnd = rangedB + (kneedB / 2.0f);
    
    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 1.0f);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Pointers for reading input and writing wet output
        float* channelData = processedBuffer.getWritePointer(channel);
        float currentGain_dB = envelope[channel];

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
            // float targetGR_dB = (inputDB < rangedB) ? (rangedB - inputDB) * compressionSlope : 0.0f;
            
            // Soft Knee Equation
            float targetGR_dB = 0.0f;
            if (inputDB < kneeStart)
            {
                targetGR_dB = (rangedB - inputDB) * compressionSlope;
            }
            else if (inputDB < kneeEnd)
            {
                const float x = kneeEnd - inputDB;
                targetGR_dB = compressionSlope / (2.0f * kneedB) * (x * x);
            }
            
            // 3. ENVELOPE SMOOTHING (in dB)
            float alpha = (targetGR_dB > currentGain_dB) ? attackCoeff : releaseCoeff;
            
            currentGain_dB = (alpha * currentGain_dB) + ((1.0f - alpha) * targetGR_dB);
            
            // 4. APPLY GAIN (in-place)
            const float finalGainLinear = juce::Decibels::decibelsToGain(currentGain_dB);
            channelData[sample] = inputSample * finalGainLinear;
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGain_dB; 
    }
}
