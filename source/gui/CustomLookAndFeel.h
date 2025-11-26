#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class CustomLookAndFeel
 * @brief Defines the visual style and color palette for all JUCE components in the plugin.
 *
 * This class overrides various draw methods to implement a custom color palette 
 * Said palette is defined in the cpp file.
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override = default;

    // --- Color Palette Definitions ---
    // Use these constants throughout the GUI for consistency.
    static const juce::Colour bgPrimary;
    static const juce::Colour bgSecondary;
    static const juce::Colour accentPrimary;
    static const juce::Colour accentSecondary;
    static const juce::Colour textDefault;

    // --- Component Drawing Overrides ---

    /**
     * @brief Custom drawing logic for rotary sliders (knobs).
     *
     * This draws a filled circle with a line indicator to show the value.
     */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    /**
     * @brief Custom drawing logic for button text.
     */
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};
