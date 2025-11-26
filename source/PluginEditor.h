#pragma once

#include "PluginProcessor.h"
// #include "BinaryData.h"
#include "gui/CustomLookAndFeel.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PunkOTTProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PunkOTTProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };
    
    // Custom Look and Feel
    CustomLookAndFeel myCustomLnF;
    
    // Sliders - Rotary knobs
    juce::Slider inputSlider, gateSlider, mixSlider, outputSlider, lifterRangeSlider, lifterTimeSlider, compThresSlider, compTimeSlider;
    juce::Label inputLabel, gateLabel, mixLabel, outputLabel, lifterRangeLabel, lifterTimeLabel, compThresLabel, compTimeLabel, clipperLabel;
    
    juce::TextButton clipperButton;
    
    // Attachments for linking sliders-parameters TODO: Uncomment
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
