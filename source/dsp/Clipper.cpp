#include "Clipper.h"

Clipper::Clipper()
{
}

// --- --- SAMPLE PROCESSING --- ---
float Clipper::applySoftClipper(float sample, float gainFactor)
{
    return gainFactor * sample / (std::abs(sample) + 1.0f);
}

float Clipper::applyHardClipper(float sample, float gainFactor)
{
    if (sample > 1.0f)
        return gainFactor * 2.0f / 3.0f;
    else if (sample < -1.0f)
        return gainFactor * -2.0f / 3.0f;
    else
        return gainFactor * (sample - std::pow(sample, 3.0f) / 3.0f);
}

float Clipper::applyTanhClipper(float sample, float gainFactor)
{
    return gainFactor * 2.0f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(sample);
}

float Clipper::applyATanClipper(float sample, float gainFactor)
{
    return gainFactor * 2.0f / juce::MathConstants<float>::pi * std::atan(sample);
}

// --- --- BUFFER PROCESSING --- ---
void Clipper::applySoftClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = gainFactor * x / (abs(x) + 1.0f);
        }
    }
}

void Clipper::applyHardClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            if (x > 1.0f)
                channelData[sample] = gainFactor * 2.0f / 3.0f;
            else if (x < -1.0f)
                channelData[sample] = gainFactor * -2.0f / 3.0f;
            else
                channelData[sample] = gainFactor * (x - std::pow(x, 3.0f) / 3.0f);
        }
    }
}

void Clipper::applyTanhClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = gainFactor * 2.0f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(x);
        }
    }
}

void Clipper::applyATanClipper(juce::AudioBuffer<float>& processedBuffer, float gainFactor)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = gainFactor * 2.0f / juce::MathConstants<float>::pi * std::atan(x);
        }
    }
}
