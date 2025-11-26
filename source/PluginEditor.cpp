#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- UTILITIES PARAMETERS ---
    // Configure the input knob
    inputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    inputSlider.setRange(0.0, 100.0, 0.1);
    inputSlider.setValue(50.0);
    addAndMakeVisible(inputSlider);
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(inputLabel);
    
    // Configure the gate knob
    gateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    gateSlider.setRange(0.0, 100.0, 0.1);
    gateSlider.setValue(50.0);
    addAndMakeVisible(gateSlider);
    gateLabel.setText("Gate", juce::dontSendNotification);
    gateLabel.setJustificationType(juce::Justification::centred);
    gateLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(gateLabel);
    
    // Configure the mix knob
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setRange(0.0, 100.0, 0.1);
    mixSlider.setValue(50.0);
    addAndMakeVisible(mixSlider);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(mixLabel);
    
    // Configure the output knob
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    outputSlider.setRange(0.0, 100.0, 0.1);
    outputSlider.setValue(50.0);
    addAndMakeVisible(outputSlider);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(outputLabel);
    
    // --- DYNAMIC PARAMETERS ---
    // Configure the lifter-range knob
    lifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterRangeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lifterRangeSlider.setRange(0.0, 100.0, 0.1);
    lifterRangeSlider.setValue(50.0);
    addAndMakeVisible(lifterRangeSlider);
    lifterRangeLabel.setText("Lift", juce::dontSendNotification);
    lifterRangeLabel.setJustificationType(juce::Justification::centred);
    lifterRangeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(lifterRangeLabel);
    
    // Configure the lifter-time knob
    lifterTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lifterTimeSlider.setRange(0.0, 100.0, 0.1);
    lifterTimeSlider.setValue(50.0);
    addAndMakeVisible(lifterTimeSlider);
    lifterTimeLabel.setText("α", juce::dontSendNotification);
    lifterTimeLabel.setJustificationType(juce::Justification::centred);
    lifterTimeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(lifterTimeLabel);
    
    // Configure the comp-thres knob
    compThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compThresSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    compThresSlider.setRange(0.0, 100.0, 0.1);
    compThresSlider.setValue(50.0);
    addAndMakeVisible(compThresSlider);
    compThresLabel.setText("Comp", juce::dontSendNotification);
    compThresLabel.setJustificationType(juce::Justification::centred);
    compThresLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(compThresLabel);
    
    // Configure the comp-time knob
    compTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    compTimeSlider.setRange(0.0, 100.0, 0.1);
    compTimeSlider.setValue(50.0);
    addAndMakeVisible(compTimeSlider);
    compTimeLabel.setText("β", juce::dontSendNotification);
    compTimeLabel.setJustificationType(juce::Justification::centred);
    compTimeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(compTimeLabel);
    
    // Configure the clipper button
    addAndMakeVisible(clipperButton);
    clipperLabel.setText("Clipper", juce::dontSendNotification);
    clipperLabel.setJustificationType(juce::Justification::centred);
    clipperLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(clipperLabel);

    // --- MELATONIN  ---
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (520, 410);
}

PluginEditor::~PluginEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (UIColors::background);

    auto area = getLocalBounds();
    g.setColour (UIColors::text);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
    
    // Position the slider and label
    inputSlider.setBounds(5, 5, 50, 50);
    inputLabel.setBounds(5, 40, 50, 10);
    
    gateSlider.setBounds(60, 5, 50, 50);
    gateLabel.setBounds(60, 40, 50, 10);
    
    mixSlider.setBounds(410, 5, 50, 50);
    mixLabel.setBounds(410, 40, 50, 10);
    
    outputSlider.setBounds(465, 5, 50, 50);
    outputLabel.setBounds(465, 40, 50, 10);
    
    lifterRangeSlider.setBounds(100, 100, 80, 80);
    lifterRangeLabel.setBounds(100, 150, 80, 30);
    
    lifterTimeSlider.setBounds(200, 100, 80, 80);
    lifterTimeLabel.setBounds(200, 150, 80, 30);
    
    compThresSlider.setBounds(300, 100, 80, 80);
    compThresLabel.setBounds(300, 150, 80, 30);
    
    compTimeSlider.setBounds(400, 100, 80, 80);
    compTimeLabel.setBounds(400, 150, 80, 30);
    
    clipperButton.setBounds(400, 300, 50, 50);
    clipperLabel.setBounds(400, 350, 50, 50);
}
