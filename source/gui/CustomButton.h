#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class CustomButton
 * @brief A custom TextButton component designed primarily for parameter toggles.
 *
 * This class applies the CustomLookAndFeel styling and includes logic to 
 * attach the button's toggle state directly to a boolean APVTS parameter.
 */
class CustomButton  : public juce::TextButton
{
public:
    /**
     * @brief Constructor for the CustomButton component.
     * @param buttonName The text displayed on the button.
     */
    CustomButton(const juce::String& buttonName);
    ~CustomButton() override;

    /**
     * @brief Sets up the APVTS attachment for this button.
     * @param apvts The AudioProcessorValueTreeState instance.
     * @param parameterID The ID of the boolean parameter to attach to (e.g., "CLIPPER_ON_OFF").
     */
    // void attachToParameter(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterID);

private:
    // The attachment object that links the button's toggle state to the APVTS parameter
    // std::unique_ptr<juce::ButtonAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomButton)
};
