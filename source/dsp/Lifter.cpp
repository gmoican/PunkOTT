#include "Lifter.h"

// Define a small constant to prevent division by zero near the noise floor.
static constexpr float minMagnitude = 0.0001f; // Approx -100 dB

Lifter::Lifter()
{
    // Initialize envelope vector size to 0
}

void Lifter::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);
    
    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 1.0f); // Initialize gain to 0 dB 
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

void Lifter::updateMakeUp(float newMakeUp_dB)
{
    makeUpGain_linear = juce::Decibels::decibelsToGain(newMakeUp_dB);
}

void Lifter::updateMix(float newMix)
{
    mix = newMix / 100.0f;
}

float Lifter::getGainAddition()
{
    return juce::Decibels::gainToDecibels(currentGA_linear);
}

void Lifter::process_inplace(juce::AudioBuffer<float>& processedBuffer)
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
        currentGA_linear = envelope[channel];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample];
            float magnitude = std::abs (inputSample);
            
            // Clamp magnitude to prevent log(0)
            magnitude = std::max(magnitude, minMagnitude);
            
            // 1. SIDECHAIN: Convert magnitude to dB
            const float inputDB = juce::Decibels::gainToDecibels(magnitude);
            
            // 2. TARGET GAIN REDUCTION - Get targetGR in dB, then convert to linear
            // Soft Knee Equation
            float targetGR = 0.0f;
            if (inputDB < kneeStart)
            {
                targetGR = (rangedB - inputDB) * compressionSlope;
            }
            else if (inputDB < kneeEnd)
            {
                const float x = kneeEnd - inputDB;
                targetGR = compressionSlope / (2.0f * kneedB) * (x * x);
            }
            
            targetGR = juce::Decibels::decibelsToGain(targetGR);
            
            // 3. ENVELOPE SMOOTHING (in dB)
            float alpha = (targetGR > currentGA_linear) ? attackCoeff : releaseCoeff;
            
            currentGA_linear = (alpha * currentGA_linear) + ((1.0f - alpha) * targetGR);
            currentGA_linear = juce::jlimit(0.0f, 100.0f, currentGA_linear);
            
            // 4. APPLY GAIN (in-place)
            channelData[sample] = inputSample * currentGA_linear * makeUpGain_linear * mix + inputSample * (1.0f - mix);
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGA_linear;
    }
}

juce::AudioBuffer<float> Lifter::process(juce::AudioBuffer<float>& inputBuffer)
{
    const int numSamples = inputBuffer.getNumSamples();
    const int numChannels = inputBuffer.getNumChannels();
    
    // Create output buffer
    juce::AudioBuffer<float> outputBuffer(numChannels, numSamples);
    
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
        const float* inputChannelData = inputBuffer.getReadPointer(channel);
        float* outputChannelData = outputBuffer.getWritePointer(channel);
        currentGA_linear = envelope[channel];

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = inputChannelData[sample];
            float magnitude = std::abs (inputSample);
            
            // Clamp magnitude to prevent log(0)
            magnitude = std::max(magnitude, minMagnitude);
            
            // 1. SIDECHAIN: Convert magnitude to dB
            const float inputDB = juce::Decibels::gainToDecibels(magnitude);
            
            // 2. TARGET GAIN REDUCTION - Get targetGR in dB, then convert to linear
            // Soft Knee Equation
            float targetGR = 1.0f;
            if (inputDB < kneeStart)
            {
                targetGR = (rangedB - inputDB) * compressionSlope;
            }
            else if (inputDB < kneeEnd)
            {
                const float x = kneeEnd - inputDB;
                targetGR = compressionSlope / (2.0f * kneedB) * (x * x);
            }
            
            targetGR = juce::Decibels::decibelsToGain(targetGR);
            
            // 3. ENVELOPE SMOOTHING (in dB)
            float alpha = (targetGR > currentGA_linear) ? attackCoeff : releaseCoeff;
            
            currentGA_linear = (alpha * currentGA_linear) + ((1.0f - alpha) * targetGR);
            
            // 4. APPLY GAIN (in-place)
            outputChannelData[sample] = inputSample * currentGA_linear * makeUpGain_linear * mix + inputSample * (1.0f - mix);
        }

        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGA_linear;
    }
    return outputBuffer;
}
