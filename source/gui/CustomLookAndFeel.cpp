#include "CustomLookAndFeel.h"

// --- Static Color Initializations ---
const juce::Colour CustomLookAndFeel::bgPrimary         = juce::Colour(0xFF161619); // Deep Black/Gray
const juce::Colour CustomLookAndFeel::bgSecondary       = juce::Colour(0xFF2C2C2C); // Panel Gray
const juce::Colour CustomLookAndFeel::accentPrimary     = juce::Colour(0xFF00D8FF); // Bright Cyan
const juce::Colour CustomLookAndFeel::accentSecondary   = juce::Colour(0xFFFF9500); // Vibrant Orange
const juce::Colour CustomLookAndFeel::textDefault       = juce::Colour(0xFFDCDCDC); // Near-White

CustomLookAndFeel::CustomLookAndFeel()
{
    // --- Global Color Setup ---
    // Set colors for various component states using the LookAndFeel mechanism.
    
    // Backgrounds and Borders
    setColour(juce::DocumentWindow::backgroundColourId, bgPrimary);
    setColour(juce::TextButton::buttonColourId, bgSecondary);
    setColour(juce::TextButton::buttonOnColourId, accentSecondary);
    setColour(juce::TextButton::textColourOffId, textDefault);
    setColour(juce::TextButton::textColourOnId, juce::Colours::black); 
    setColour(juce::ComboBox::backgroundColourId, bgSecondary.darker(0.1f));
    
    // Sliders (Knobs)
    setColour(juce::Slider::backgroundColourId, bgSecondary);
    setColour(juce::Slider::thumbColourId, accentPrimary);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::trackColourId, bgSecondary.darker(0.3f));
    
    // Text
    setColour(juce::Label::textColourId, textDefault);
    setColour(juce::Slider::textBoxTextColourId, textDefault);

    // Set a default global font
    // setDefaultSansSerifFont(juce::Font("Inter", 14.0f, juce::Font::plain));
}

// --- drawRotarySlider Implementation ---
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                         const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    // Define the central area for the knob drawing
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(1.0f);
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    float centreX = bounds.getCentreX();
    float centreY = bounds.getCentreY();
    float lineThickness = radius * 0.12f;

    // The angle of the indicator line (current value)
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // 1. Draw the Background Track (Inactive part of the knob)
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                                rotaryStartAngle, rotaryEndAngle, true);
    
    // g.setColour(getColour(juce::Slider::trackColourId));
    g.strokePath(backgroundArc, juce::PathStrokeType(lineThickness, juce::PathStrokeType::mitered, juce::PathStrokeType::butt));

    // 2. Draw the Value Track (Active part of the knob)
    if (sliderPos > 0.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                                rotaryStartAngle, angle, true);
        
        // g.setColour(getColour(juce::Slider::thumbColourId));
        g.strokePath(valueArc, juce::PathStrokeType(lineThickness, juce::PathStrokeType::mitered, juce::PathStrokeType::butt));
    }
    
    // 3. Draw the Knob Cap (Inner Circle)
    juce::Rectangle<float> knobArea = bounds.reduced(radius * 0.15f);
    
    // Inner Shadow effect for depth
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillEllipse(knobArea.reduced(2.0f));

    // Main knob body
    g.setColour(bgSecondary.brighter(0.2f));
    g.fillEllipse(knobArea);

    // 4. Draw the Pointer (line indicator)
    juce::Point<float> thumbPoint(centreX + radius * 0.8f * std::cos(angle),
                                  centreY + radius * 0.8f * std::sin(angle));

    juce::Path p;
    p.startNewSubPath(centreX, centreY);
    p.lineTo(thumbPoint);
    
    // g.setColour(getColour(juce::Slider::thumbColourId).brighter(0.5f));
    g.strokePath(p, juce::PathStrokeType(lineThickness * 0.5f)); 
}


// --- drawButtonText Implementation ---
void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Rectangle<int> textBounds = button.getLocalBounds().reduced (5, 4);

    // Determine the colour based on state
    juce::Colour colourToUse = button.isColourSpecified(juce::TextButton::textColourOffId) 
                                ? button.findColour(juce::TextButton::textColourOffId) 
                                : textDefault;

    if (button.getToggleState())
    {
        // If toggled ON, use black text on the accent color
        colourToUse = juce::Colours::black;
    }
    else if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
    {
        // Slightly lighten text on hover/down
        colourToUse = colourToUse.brighter(0.3f);
    }
    
    g.setColour(colourToUse);
    // g.setFont(button.getFont());

    g.drawFittedText(button.getButtonText(), textBounds, juce::Justification::centred, 1);
}
