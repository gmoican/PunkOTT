#include "Clipper.h"

Clipper::Clipper()
{
}

void Clipper::applySoftClipper(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = 1.2f * x / (abs(x) + 1.0f);
        }
    }
}

void Clipper::applyHardClipper(juce::AudioBuffer<float>& processedBuffer)
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
                channelData[sample] = 2.0f / 3.0f;
            else if (x < -1.0f)
                channelData[sample] = -2.0f / 3.0f;
            else
                channelData[sample] = x - std::pow(x, 3.0f) / 3.0f;
        }
    }
}

void Clipper::applyTanhClipper(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = 2.0f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(x);
        }
    }
}

void Clipper::applyATanClipper(juce::AudioBuffer<float>& processedBuffer)
{
    const int numSamples = processedBuffer.getNumSamples();
    const int numChannels = processedBuffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = processedBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float x = channelData[sample];
            channelData[sample] = 2.0f / juce::MathConstants<float>::pi * std::atan(x);
        }
    }
}
