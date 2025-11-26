#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace UIColors
{
    // Define your 5-color palette
    const juce::Colour background    = juce::Colour(0xff1a1a1a);  // Dark background
    const juce::Colour primary       = juce::Colour(0xff00d4ff);  // Bright accent
    const juce::Colour secondary     = juce::Colour(0xff6b6b6b);  // Mid grey
    const juce::Colour text          = juce::Colour(0xffffffff);  // White text
    const juce::Colour highlight     = juce::Colour(0xffff6b35);  // Orange highlight
}

namespace UIDimensions
{
    // You can also add sizing constants here
    const int knobSize = 80;
    const int margin = 20;
}
