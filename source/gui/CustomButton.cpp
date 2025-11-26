#include "CustomButton.h"
/*
CustomButton::CustomButton(const juce::String& buttonName)
    : juce::TextButton(buttonName)
{
    // --- Button Setup ---
    setClickingTogglesState(true); // Make it a toggle button (essential for on/off switches)
    setToggleState(false, juce::dontSendNotification); // Start in the OFF state
    setWantsKeyboardFocus(false);
    
    // The visual styling for the OFF and ON states will be handled by CustomLookAndFeel::drawButtonText
}

CustomButton::~CustomButton()
{
    // The unique_ptr 'attachment' manages memory cleanup automatically
}

void CustomButton::attachToParameter(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterID)
{
    // Create the attachment linking the button's toggle state to the given APVTS parameter ID
    attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, parameterID, *this));
}
*/
