#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors for components
    setColour(juce::Slider::thumbColourId, UIColors::primary);
    setColour(juce::Slider::trackColourId, UIColors::secondary);
    setColour(juce::Slider::backgroundColourId, UIColors::background);

    setColour(juce::TextButton::buttonColourId, UIColors::secondary);
    setColour(juce::TextButton::buttonOnColourId, UIColors::primary);
    setColour(juce::TextButton::textColourOffId, UIColors::text);
    setColour(juce::TextButton::textColourOnId, UIColors::background);

    setColour(juce::ComboBox::backgroundColourId, UIColors::secondary);
    setColour(juce::ComboBox::textColourId, UIColors::text);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 5.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw background circle
    g.setColour(UIColors::secondary.withAlpha(0.3f));
    g.fillEllipse(rx, ry, rw, rw);

    // Draw track (background arc)
    g.setColour(UIColors::secondary);
    juce::Path trackArc;
    trackArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                           rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(trackArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Draw filled arc (value)
    g.setColour(UIColors::primary);
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
                           rotaryStartAngle, angle, true);
    g.strokePath(valueArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Draw thumb (indicator)
    auto thumbWidth = 6.0f;
    auto thumbHeight = radius * 0.5f;
    g.setColour(UIColors::primary);

    juce::Path thumb;
    thumb.addEllipse(0.0f, 0.0f, thumbWidth, thumbHeight);
    thumb.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.fillPath(thumb);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Use the default JUCE drawing for linear sliders
    juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour& backgroundColour,
                                             bool shouldDrawButtonAsHighlighted,
                                             bool shouldDrawButtonAsDown)
{
    // Draw the button with rounded corners
    juce::Rectangle<int> bounds = button.getBounds();
    g.setColour(shouldDrawButtonAsDown ? UIColors::primary : backgroundColour);
    g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
}

void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown)
{
    // Set text color based on button state
    g.setColour(shouldDrawButtonAsDown ? UIColors::background : UIColors::text);
    g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
}
