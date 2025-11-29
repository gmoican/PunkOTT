#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- LAYOUT ---
    header.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    // header.setButtonText ("Header");
    addAndMakeVisible (header);
    
    footer.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    // footer.setButtonText ("Footer");
    addAndMakeVisible (footer);
    
    sidebarLeft.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
    // sidebarLeft.setButtonText ("Sidebar L");
    addAndMakeVisible (sidebarLeft);
    
    sidebarRight.setColour (juce::TextButton::buttonColourId, juce::Colours::grey);
    // sidebarRight.setButtonText ("Sidebar R");
    addAndMakeVisible (sidebarRight);
    
    lifterContainer.setColour (juce::TextButton::buttonColourId, juce::Colours::lime);
    // lifterContainer.setButtonText("Lifter");
    addAndMakeVisible (lifterContainer);
    
    compContainer.setColour (juce::TextButton::buttonColourId, juce::Colours::yellowgreen);
    // compContainer.setButtonText("Comp");
    addAndMakeVisible (compContainer);
    
    displayContainer.setColour (juce::TextButton::buttonColourId, juce::Colours::orange);
    // displayContainer.setButtonText("Display");
    addAndMakeVisible (displayContainer);
    
    // --- UTILITIES PARAMETERS ---
    // Input knob
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
    
    // Gate knob
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
    
    // Mix knob
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
    
    // Output knob
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
    // Lifter-range knob
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

    // Lifter-time knob
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

    // Comp-thres knob
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
    
    // Comp-time knob
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
    
    // Clipper button
    clipperButton.setClickingTogglesState(true);
    addAndMakeVisible(clipperButton);
    
    clipperLabel.setText(Parameters::clipperName, juce::dontSendNotification);
    clipperLabel.setJustificationType(juce::Justification::centred);
    clipperLabel.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(clipperLabel);
    
    clipperAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::clipperId, clipperButton);
    
    // Version tag
    versionTag.setText(juce::String ("") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " by @punkarra4",
                       juce::dontSendNotification);
    versionTag.setJustificationType(juce::Justification::centred);
    versionTag.setColour(juce::Label::textColourId, UIColors::text);
    addAndMakeVisible(versionTag);

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

    // auto area = getLocalBounds();
    // g.setColour (UIColors::text);
    // g.setFont (16.0f);
    // auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    // g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    // area.removeFromBottom(50);
    // inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop(50);
    auto footerArea = area.removeFromBottom(30);
    auto sideLArea = area.removeFromLeft(50);
    auto sideRArea = area.removeFromRight(50);
    header.setBounds(headerArea);
    footer.setBounds(footerArea);
    sidebarLeft.setBounds(sideLArea);
    sidebarRight.setBounds(sideRArea);
    
    // --- FOOTER ---
    versionTag.setBounds(footerArea);
    
    // --- HEADER CONTROLS ---
    inputSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    // inputLabel.setBounds(5, 40, 50, 10);
    
    gateSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    // gateLabel.setBounds(60, 40, 50, 10);
    
    outputSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    // outputLabel.setBounds(465, 40, 50, 10);
    
    mixSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    // mixLabel.setBounds(410, 40, 50, 10);
    
    clipperButton.setBounds(headerArea.removeFromRight(headerArea.getHeight())
                                      .reduced(10)
                            );
    // clipperLabel.setBounds(400, 350, 50, 50);
    
    // --- LIFTER AND COMP CONTROLS ---
    auto contentItemHeight = 100;
    // Reserve the top area for lifter and comp containers
    auto topArea = area.removeFromTop(contentItemHeight);
    
    // Split the top area in half horizontally
    auto lifterArea = topArea.removeFromLeft(topArea.getWidth() / 2).reduced(5);
    auto compArea = topArea.reduced(5);
    
    lifterContainer.setBounds (lifterArea);
    compContainer.setBounds (compArea);
    
    // Position sliders inside lifter container
    auto lifterSliderArea = lifterArea.reduced(10);
    lifterRangeSlider.setBounds(lifterSliderArea.removeFromLeft(lifterSliderArea.getWidth() / 2));
    lifterTimeSlider.setBounds(lifterSliderArea);
    // lifterRangeSlider.setBounds(dynamicsArea.removeFromLeft(dynamicsArea.getWidth() / 2));
    // // lifterRangeLabel.setBounds(100, 150, 80, 30);
    //
    // lifterTimeSlider.setBounds(dynamicsArea.removeFromLeft(dynamicsArea.getWidth() / 2));
    // // lifterTimeLabel.setBounds(200, 150, 80, 30);
    
    // Position sliders inside comp container
    auto compSliderArea = compArea.reduced(10);
    compThresSlider.setBounds(compSliderArea.removeFromLeft(compSliderArea.getWidth() / 2));
    compTimeSlider.setBounds(compSliderArea);
    // compThresSlider.setBounds(300, 100, 80, 80);
    // compThresLabel.setBounds(300, 150, 80, 30);
    //
    // compTimeSlider.setBounds(400, 100, 80, 80);
    // compTimeLabel.setBounds(400, 150, 80, 30);
    
    // --- DISPLAY ---
    displayContainer.setBounds (area.removeFromTop (contentItemHeight * 2)
                                    .reduced(5)
                                );

}
