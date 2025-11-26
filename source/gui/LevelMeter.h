#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "CustomLookAndFeel.h" // For consistent styling

/**
 * @class LevelMeter
 * @brief A custom component to visualize peak input level, peak output level, 
 * and the current gain reduction (GR).
 */
class LevelMeter  : public juce::Component
{
public:
    LevelMeter();
    ~LevelMeter() override = default;

    // --- Data Update Method ---
    /**
     * @brief Updates the meter values.
     * @param inputLevel The current peak input level (0.0 to 1.0, linear).
     * @param outputLevel The current peak output level (0.0 to 1.0, linear).
     * @param gainReductionDB The current gain reduction applied (0.0 to negative dB).
     */
    void update(float inputLevel, float outputLevel, float gainReductionDB);
    
    // JUCE Component methods
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    // --- Meter States (Stored in linear scale or dB) ---
    float currentInputLevel = 0.0f;
    float currentOutputLevel = 0.0f;
    float currentGainReductionDB = 0.0f;

    // --- Look and Feel Instance ---
    // Although the general LNF is applied, sometimes it's easier to grab colors directly.
    CustomLookAndFeel customLookAndFeel;

    /**
     * @brief Converts a linear amplitude value (0.0 to 1.0) to a dB value, with a floor.
     * @param linearLevel The linear amplitude.
     * @return The dB value (e.g., -60.0f to 0.0f).
     */
    float linearToDB(float linearLevel);

    /**
     * @brief Draws a single vertical bar meter.
     * @param g The Graphics context.
     * @param bounds The drawing area for the meter.
     * @param levelDB The level to draw (e.g., -60.0f to 0.0f).
     * @param color The color of the meter fill.
     * @param drawText If true, draws dB labels.
     */
    void drawMeterBar(juce::Graphics& g, juce::Rectangle<float> bounds, float levelDB, juce::Colour color, bool drawText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
