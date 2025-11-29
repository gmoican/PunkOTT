#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- UTILITIES PARAMETERS ---
    // Configure the input knob
    inputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    inputSlider.setRange(Parameters::inMin, Parameters::inMax, 0.1);
    inputSlider.setValue(Parameters::inDefault);
    addAndMakeVisible(inputSlider);
    
    inputLabel.setText(Parameters::inName, juce::dontSendNotification);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(inputLabel);
    
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::inId, inputSlider);
    
    // Configure the gate knob
    gateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    gateSlider.setRange(Parameters::gateMin, Parameters::gateMax, 0.1);
    gateSlider.setValue(Parameters::gateDefault);
    addAndMakeVisible(gateSlider);
    
    gateLabel.setText(Parameters::gateName, juce::dontSendNotification);
    gateLabel.setJustificationType(juce::Justification::centred);
    gateLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(gateLabel);
    
    gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::gateId, gateSlider);
    
    // Configure the mix knob
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setRange(Parameters::mixMin, Parameters::mixMax, 0.1);
    mixSlider.setValue(Parameters::mixDefault);
    addAndMakeVisible(mixSlider);
    
    mixLabel.setText(Parameters::mixName, juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(mixLabel);
    
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::mixId, mixSlider);
    
    // Configure the output knob
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    outputSlider.setRange(Parameters::outMin, Parameters::outMax, 0.1);
    outputSlider.setValue(Parameters::outDefault);
    addAndMakeVisible(outputSlider);
    
    outputLabel.setText(Parameters::outName, juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(outputLabel);
    
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::outId, outputSlider);

    // --- DYNAMIC PARAMETERS ---
    // Configure the lifter-range knob
    lifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterRangeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    lifterRangeSlider.setValue(Parameters::lifterThresDefault);
    addAndMakeVisible(lifterRangeSlider);
    
    lifterRangeLabel.setText(Parameters::lifterThresName, juce::dontSendNotification);
    lifterRangeLabel.setJustificationType(juce::Justification::centred);
    lifterRangeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(lifterRangeLabel);
    
    lifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterThresId, lifterRangeSlider);

    // Configure the lifter-time knob
    lifterTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lifterTimeSlider.setRange(Parameters::lifterTimeMin, Parameters::lifterTimeMax, 0.1);
    lifterTimeSlider.setValue(Parameters::lifterTimeDefault);
    addAndMakeVisible(lifterTimeSlider);
    
    lifterTimeLabel.setText("α", juce::dontSendNotification);
    lifterTimeLabel.setJustificationType(juce::Justification::centred);
    lifterTimeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(lifterTimeLabel);
    
    lifterTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterTimeId, lifterTimeSlider);

    // Configure the comp-thres knob
    compThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compThresSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    compThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    compThresSlider.setValue(Parameters::compThresDefault);
    addAndMakeVisible(compThresSlider);
    
    compThresLabel.setText(Parameters::compThresName, juce::dontSendNotification);
    compThresLabel.setJustificationType(juce::Justification::centred);
    compThresLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(compThresLabel);
    
    compThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compThresId, compThresSlider);
    
    // Configure the comp-time knob
    compTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    compTimeSlider.setRange(Parameters::compTimeMin, Parameters::compTimeMax, 0.1);
    compTimeSlider.setValue(Parameters::compTimeDefault);
    addAndMakeVisible(compTimeSlider);
    
    compTimeLabel.setText("β", juce::dontSendNotification);
    compTimeLabel.setJustificationType(juce::Justification::centred);
    compTimeLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(compTimeLabel);
    
    compTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compTimeId, compTimeSlider);
    
    // Configure the clipper button
    clipperButton.setClickingTogglesState(true);
    addAndMakeVisible(clipperButton);
    
    clipperLabel.setText(Parameters::clipperName, juce::dontSendNotification);
    clipperLabel.setJustificationType(juce::Justification::centred);
    clipperLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(clipperLabel);
    
    clipperAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::clipperId, clipperButton);

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
    // juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
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
