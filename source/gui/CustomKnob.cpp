#include "CustomKnob.h"
/*
CustomKnob::CustomKnob(const juce::String& name)
{
    // --- Slider Setup ---
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20); // Value display at the bottom
    setWantsKeyboardFocus(false);
    
    // Set the range to ensure smooth movement (though APVTS will handle the actual value range)
    // TODO: setRange(0.0, 1.0, 0.0);

    // --- Label Setup ---
    paramLabel.setText(name, juce::dontSendNotification);
    paramLabel.setJustificationType(juce::Justification::centred);
    paramLabel.attachToComponent(this, false); // Attach the label relative to the knob component

    // Add the label as a child component
    addAndMakeVisible(paramLabel);
}

CustomKnob::~CustomKnob()
{
    // The unique_ptr 'attachment' will automatically manage memory here.
}

void CustomKnob::attachToParameter(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterID)
{
    // Create the attachment linking this slider to the given APVTS parameter ID
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, parameterID, *this));
}

void CustomKnob::resized()
{
    auto bounds = getLocalBounds();
    
    // 1. Label Positioning (Top section)
    // Give the label a small space at the top for the parameter name
    auto labelHeight = 16;
    paramLabel.setBounds(bounds.removeFromTop(labelHeight));
    
    // 2. Slider Positioning (Remaining area, including the textbox)
    // The custom LookAndFeel draws the rotary element based on the available space.
    juce::Slider::resized();
}
*/
