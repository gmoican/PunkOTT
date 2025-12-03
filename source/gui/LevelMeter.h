#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "UIConstants.h"

class LevelMeter : public juce::Component
{
public:
    LevelMeter();
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        
        g.setColour(UIColors::secondary);
        g.fillRoundedRectangle(bounds, 5.f);
        
        g.setColour(UIColors::primary);
        const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, bounds.getHeight());
        g.fillRoundedRectangle(bounds.removeFromLeft(scaledY), 5.f);
    }
    
    void setLevel(const float value) { level = value; }
    
private:
    float level = -60.f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
