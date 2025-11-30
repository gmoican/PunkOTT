#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace UIColors
{
    // Define your 5-color palette
    const juce::Colour background    = juce::Colour(0xffffffee);  // White background
    const juce::Colour container     = juce::Colour(0xffe64f25);  // Light Orange
    const juce::Colour primary       = juce::Colour(0xffed884c);  // Strong Orange
    const juce::Colour secondary     = juce::Colour(0xff6b6b6b);  // Mid Orange
    const juce::Colour text          = juce::Colour(0xff991b27);  // Strong Red
    const juce::Colour highlight     = juce::Colour(0xffbd2630);  // Mid Red
}

namespace UIDimensions
{
    // You can also add sizing constants here
    const int knobSize = 80;
    const int margin = 20;
}
