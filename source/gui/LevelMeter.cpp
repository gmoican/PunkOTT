#include "LevelMeter.h"
#include <cmath> // For std::log10
/*
LevelMeter::LevelMeter()
{
    // Initialize timer or animation frame logic if needed, but for simple meters, 
    // we'll rely on the Processor calling update() in processBlock.
}

// --- Data Update ---
void LevelMeter::update(float inputLevel, float outputLevel, float gainReductionDB)
{
    // Note: We use jmax to ensure the decay is handled by subsequent smaller updates.
    // In a real plugin, you'd use a Peak Detector class with attack/release for smoothing.
    
    currentInputLevel = std::max(inputLevel, currentInputLevel);
    currentOutputLevel = std::max(outputLevel, currentOutputLevel);
    currentGainReductionDB = gainReductionDB; // GR is a continuous signal, not a peak hold

    // Re-draw the meter area
    repaint();
}

// --- Helper for dB Conversion ---
float LevelMeter::linearToDB(float linearLevel)
{
    // Clamp the value to prevent log(0)
    linearLevel = juce::jmax(0.00001f, linearLevel); // ~ -100 dBFS floor
    return 20.0f * std::log10(linearLevel);
}

// --- Drawing Function ---
void LevelMeter::drawMeterBar(juce::Graphics& g, juce::Rectangle<float> bounds, float levelDB, juce::Colour color, bool drawText)
{
    // Constants for meter scaling
    const float maxDB = 0.0f;
    const float minDB = -60.0f;
    const float rangeDB = maxDB - minDB; // 60 dB range

    // 1. Draw the meter track background
    g.setColour(CustomLookAndFeel::bgSecondary.darker(0.3f));
    g.fillRoundedRectangle(bounds, 3.0f);

    // 2. Calculate the fill height
    // Map the current levelDB from minDB..maxDB to 0.0..1.0
    float normalizedLevel = juce::jmap(levelDB, minDB, maxDB, 0.0f, 1.0f);
    normalizedLevel = juce::jlimit(0.0f, 1.0f, normalizedLevel);

    // Height of the filled area
    float fillHeight = bounds.getHeight() * normalizedLevel;
    
    // Rectangle for the filled level
    juce::Rectangle<float> fillRect = bounds.withHeight(fillHeight).withY(bounds.getBottom() - fillHeight);

    // 3. Draw the level fill (gradient or solid)
    g.setColour(color);
    g.fillRoundedRectangle(fillRect, 3.0f);

    // 4. Draw Peak Hold Line (Simplified: just draw the current peak)
    // In a full implementation, you'd hold the peak for a few seconds.
    if (levelDB > minDB)
    {
        g.setColour(juce::Colours::white);
        float peakY = bounds.getBottom() - (bounds.getHeight() * normalizedLevel);
        g.drawLine(bounds.getX(), peakY, bounds.getRight(), peakY, 1.0f);
    }


    // 5. Draw dB Labels (only for the Input Meter)
    if (drawText)
    {
        g.setColour(CustomLookAndFeel::textDefault.withAlpha(0.5f));
        g.setFont(10.0f);
        
        // Define common label positions in dB
        int levels[] = { 0, -6, -12, -24, -48 };
        
        for (int db : levels)
        {
            float normalizedPosition = juce::jmap((float)db, minDB, maxDB, 0.0f, 1.0f);
            float yPos = bounds.getBottom() - (bounds.getHeight() * normalizedPosition);
            
            // Draw line
            g.drawLine(bounds.getX() - 3.0f, yPos, bounds.getX() + 0.0f, yPos, 0.5f);

            // Draw text
            juce::String label = juce::String(db);
            g.drawText(label, bounds.getX() - 25, (int)yPos - 5, 20, 10, juce::Justification::centredRight, false);
        }
    }
}

// --- Main Paint Method ---
void LevelMeter::paint (juce::Graphics& g)
{
    // Clear the background of the component
    g.fillAll(CustomLookAndFeel::bgPrimary); 
    
    auto bounds = getLocalBounds().toFloat();
    
    // Define the drawing areas for the three meters
    auto meterWidth = 15.0f;
    auto spacing = (bounds.getWidth() - (meterWidth * 3.0f)) / 4.0f;
    
    juce::Rectangle<float> inputBounds = bounds.removeFromLeft(spacing).removeFromRight(meterWidth);
    
    // 1. Input Meter (with dB labels)
    float inputDB = linearToDB(currentInputLevel);
    drawMeterBar(g, inputBounds, inputDB, CustomLookAndFeel::accentPrimary.darker(0.2f), true);

    // Draw Input Label
    g.setColour(CustomLookAndFeel::textDefault);
    g.setFont(12.0f);
    g.drawText("IN", inputBounds.withY(inputBounds.getBottom() + 2).toNearestInt(), juce::Justification::centredTop, false);

    // 2. Gain Reduction Meter
    bounds.removeFromLeft(spacing); // Move past input meter spacing
    juce::Rectangle<float> grBounds = bounds.removeFromLeft(meterWidth);
    
    // GR is already in dB (negative or zero)
    // Draw the GR meter inverted (0dB is the top, positive reduction is the bottom)
    drawMeterBar(g, grBounds, currentGainReductionDB, CustomLookAndFeel::accentSecondary, false); 
    
    // Draw GR Label
    g.drawText("GR", grBounds.withY(grBounds.getBottom() + 2).toNearestInt(), juce::Justification::centredTop, false);


    // 3. Output Meter
    bounds.removeFromLeft(spacing); // Move past GR meter spacing
    juce::Rectangle<float> outputBounds = bounds.removeFromLeft(meterWidth);
    
    float outputDB = linearToDB(currentOutputLevel);
    drawMeterBar(g, outputBounds, outputDB, CustomLookAndFeel::accentPrimary, false);

    // Draw Output Label
    g.drawText("OUT", outputBounds.withY(outputBounds.getBottom() + 2).toNearestInt(), juce::Justification::centredTop, false);
}

void LevelMeter::resized()
{
    // Not needed as paint calculates positions relative to the current bounds.
}
*/
