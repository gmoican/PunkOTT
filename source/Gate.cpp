#include "Gate.h"

// Define a small constant to prevent division by zero near the noise floor.
static constexpr float minMagnitude = 0.000001f; // Approx -120 dB

Gate::Gate()
{
}

void Gate::prepare(double sampleRate, int totalNumChannels)
{
    juce::ignoreUnused(sampleRate);
    
    // Resize the envelope vector to match the channel count
    envelope.assign(totalNumChannels, 0.0f); // Initialize gain to 0 dB
}

float Gate::calculateTimeCoeff(float sampleRate, float time_ms)
{
    return std::exp(-1.0f / (sampleRate * (time_ms / 1000.0f)));
}

void Gate::updateRatio(float newRatio)
{
    ratio = newRatio;
}

void Gate::updateThres(float newThres)
{
    thresdB = newThres;
}

void Gate::updateKnee(float newKnee)
{
    kneedB = newKnee;
}

void Gate::updateAttack(float sampleRate, float newAttMs)
{
    attackCoeff = calculateTimeCoeff(sampleRate, newAttMs);
}

void Gate::updateRelease(float sampleRate, float newRelMs)
{
    releaseCoeff = calculateTimeCoeff(sampleRate, newRelMs);
}

void Gate::process(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    // --- Pre-calculations ---
    // Calculate the slope for gain reduction
    const float expansionSlope = ratio - 1.0f;
    const float kneeStart = thresdB - (kneedB / 2.0f);
    const float kneeEnd = thresdB + (kneedB / 2.0f);

    // Ensure envelope vector is correctly sized (safety)
    if ((int)envelope.size() != numChannels)
        envelope.assign(numChannels, 1.0f);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Pointers for reading input and writing wet output
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
            // float targetGR_dB = (inputDB < thresdB) ? (inputDB - thresdB) * compressionSlope : 0.0f;
            
            // Soft Knee Equation
            float targetGR_dB = 0.0f;
            if (inputDB < kneeStart)
            {
                targetGR_dB = (inputDB - thresdB) * expansionSlope;
            }
            else if (inputDB < kneeEnd)
            {
                const float x = inputDB - kneeEnd;
                targetGR_dB = expansionSlope / (2.0f * kneedB) * (x * x);
            }
            
            // 3. ENVELOPE SMOOTHING (in dB)
            float alpha = (-targetGR_dB > -currentGR_dB) ? attackCoeff : releaseCoeff;
            
            const float smoothedReductionAmount = (alpha * -currentGR_dB) + ((1.0f - alpha) * -targetGR_dB);
            
            currentGR_dB = -smoothedReductionAmount;
            
            // 4. APPLY GAIN (in-place)
            const float gainReductionLinear = juce::Decibels::decibelsToGain(currentGR_dB);
            channelData[sample] = inputSample * gainReductionLinear;
        }
        
        // Store the final envelope value for the start of the next block
        envelope[channel] = currentGR_dB; 
    }
}
