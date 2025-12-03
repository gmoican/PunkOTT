#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIConstants.h"

class LevelMeter : public juce::Component, private juce::Timer
{
public:
    LevelMeter(std::atomic<float>* levelPtr, const juce::String label) : levelPointer(levelPtr), meterLabel(label)
    {
        startTimerHz(30);
    }
    
    ~LevelMeter() override
    {
        stopTimer();
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds();
        
        // Draw background
        // g.setColour(UIColors::secondary.withAlpha(0.2f));
        // g.fillRect(bounds);
        
        // Draw label
        g.setColour(UIColors::text);
        g.setFont(9.0f);
        auto labelArea = bounds.removeFromTop(25);
        g.drawText(meterLabel, labelArea, juce::Justification::centred);

        
        // Adjust level
        float min_dB = -40.0f;
        float max_dB = 0.0f;
        
        float clampedLevel = juce::jlimit(min_dB, max_dB, currentLevel);
        float normalizedLevel = juce::jmap(clampedLevel, min_dB, max_dB, 0.0f, 1.0f);
        
        // Draw background bar
        g.setColour(UIColors::secondary.withAlpha(0.5f));
        g.fillRect(bounds);
        
        // Draw level bar
        int meterHeight = (int)(bounds.getHeight() * normalizedLevel);
        auto meterRect = bounds.removeFromBottom(meterHeight);
        g.setColour(UIColors::primary);
        g.fillRect(meterRect);
        
        // Draw dB text
        // g.setColour(UIColors::text.withAlpha(0.7f));
        // g.setFont(10.0f);
        // g.drawText(juce::String(clampedLevel, 1) + " dB", bounds, juce::Justification::centred);
    }
    
    // void setLevel(const float value) { level = value; }
    
private:
    void timerCallback() override
    {
        if (levelPointer == nullptr)
            return;
        
        // Get new level from atomic pointer
        float newLevel = levelPointer->load();
        
        // Apply smoothing with ballistics
        if (newLevel > currentLevel)
        {
            // Fast attack for rising levels
            currentLevel = newLevel;
        }
        else
        {
            // Slow decay for falling levels
            float decayRate = 0.95f;
            currentLevel = currentLevel * decayRate + newLevel * (1.0f - decayRate);
        }
        
        repaint();
    }
    
    std::atomic<float>* levelPointer;
    juce::String meterLabel;
    float currentLevel = -100.f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
