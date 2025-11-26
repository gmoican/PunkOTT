#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

/**
 * @class Knob
 * @brief A composite component combining a Rotary Slider and a Label.
 *
 * This class encapsulates the standard visual and attachment setup for 
 * all rotary controls in the plugin.
 */
class CustomKnob : public juce::Slider
{
public:
    /**
     * @brief Constructor for the Knob component.
     * @param name The name of the parameter this knob controls (used for the label).
     */
    CustomKnob(const juce::String& name);
    ~CustomKnob() override;

    /**
     * @brief Sets up the APVTS attachment for this slider.
     * @param apvts The AudioProcessorValueTreeState instance.
     * @param parameterID The ID of the parameter to attach to.
     */
    // void attachToParameter(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterID);
    
    // JUCE Component methods
    void resized() override;

private:
    // The label displayed above the knob
    juce::Label paramLabel;

    // The attachment object that links the slider to the APVTS parameter
    // std::unique_ptr<juce::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomKnob)
};
