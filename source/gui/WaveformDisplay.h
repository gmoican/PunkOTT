#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "UIConstants.h"

class WaveformDisplay : public juce::Component, private juce::Timer
{
public:
    WaveformDisplay(juce::AudioProcessorValueTreeState& apvts)
    : parameters(apvts), historyLength(200)
    {
        startTimerHz(60); // Update 60 times per second for smooth display
    }
    
    ~WaveformDisplay() override
    {
        stopTimer();
    }
    
    // Call this from your processor to push audio data
    void pushBuffer(const juce::AudioBuffer<float>& buffer)
    {
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();
        
        // Sum channels to mono
        if (numChannels > 0)
        {
            audioBuffer.setSize(1, numSamples, false, false, false);
            audioBuffer.clear();
            for (int channel = 0; channel < numChannels; ++channel)
            {
                audioBuffer.addFrom(0, 0, buffer, channel, 0, numSamples);
            }
            audioBuffer.applyGain(1.0f / numChannels); // Normalize
        }
        
        newDataAvailable = true;
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        
        // Draw background
        g.setColour(UIColors::background);
        g.fillRect(bounds);
        
        // Draw border
        g.setColour(UIColors::secondary);
        g.drawRect(bounds, 2);
        
        if (audioBuffer.getNumSamples() == 0)
            return;
        
        // Get threshold values
        float compThreshold = parameters.getRawParameterValue("compThresh")->load();
        float lifterRange = parameters.getRawParameterValue("lifterRange")->load();
        
        // Draw waveform
        const float* data = audioBuffer.getReadPointer(0);
        const int numSamples = audioBuffer.getNumSamples();
        const float width = (float)bounds.getWidth();
        const float height = (float)bounds.getHeight();
        const float centerY = height * 0.5f;
        
        // Create a buffer of samples to draw
        std::vector<float> samples(numSamples);
        for (int i = 0; i < numSamples; ++i)
        {
            samples[i] = data[i];
        }
        
        // Normalize samples to [0, 1] for drawing
        float maxVal = *std::max_element(samples.begin(), samples.end());
        float minVal = *std::min_element(samples.begin(), samples.end());
        float range = maxVal - minVal;
        
        if (range == 0.0f)
            range = 1.0f;
        
        for (int i = 0; i < numSamples; ++i)
        {
            samples[i] = (samples[i] - minVal) / range;
        }
        
        // Draw the waveform with scrolling effect
        for (int i = 0; i < numSamples; ++i)
        {
            float x = juce::jmap((float)i, 0.0f, (float)numSamples, 0.0f, width);
            float sample = samples[i];
            float y = centerY - (sample * centerY * 0.9f); // 0.9 for some margin
            
            // Determine color based on sample level
            float sampleLevel = std::abs(samples[i]);
            
            if (sampleLevel > compThreshold)
            {
                g.setColour(juce::Colours::red);
            }
            else if (sampleLevel < lifterRange)
            {
                g.setColour(juce::Colours::yellow);
            }
            else
            {
                g.setColour(UIColors::primary);
            }
            
            // Draw a vertical line for this sample
            g.drawLine(x, y, x, y + 1);
        }
    }
    
    
    void resized() override
    {
        // No child components
    }
    
private:
    void timerCallback() override
    {
        if (newDataAvailable)
        {
            newDataAvailable = false;
            repaint();
        }
    }
    
    juce::AudioProcessorValueTreeState& parameters;
    juce::AudioBuffer<float> audioBuffer;
    std::atomic<bool> newDataAvailable { false };
    int historyLength;
};
