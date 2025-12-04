#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- LAYOUT ---
    header.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.5f));
    header.setEnabled(false);
    // header.setButtonText ("Header");
    addAndMakeVisible (header);
    
    footer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.5f));
    footer.setEnabled(false);
    // footer.setButtonText ("Footer");
    addAndMakeVisible (footer);
    
    sidebarLeft.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarLeft.setEnabled(false);
    // sidebarLeft.setButtonText ("Sidebar L");
    addAndMakeVisible (sidebarLeft);
    
    sidebarRight.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    sidebarRight.setEnabled(false);
    // sidebarRight.setButtonText ("Sidebar R");
    addAndMakeVisible (sidebarRight);
    
    lifterContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    lifterContainer.setEnabled(false);
    // lifterContainer.setButtonText("Lifter");
    addAndMakeVisible (lifterContainer);
    
    compContainer.setColour (juce::TextButton::buttonColourId, UIColors::container.withAlpha(0.25f));
    compContainer.setEnabled(false);
    // compContainer.setButtonText("Comp");
    addAndMakeVisible (compContainer);
    
    displayContainer.setColour (juce::TextButton::buttonColourId, juce::Colours::red.withAlpha(0.25f));
    displayContainer.setEnabled(false);
    // displayContainer.setButtonText("Display");
    addAndMakeVisible (displayContainer);
    
    // --- UTILITIES PARAMETERS ---
    // Input knob
    inputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputSlider.setRange(Parameters::inMin, Parameters::inMax, 0.1);
    inputSlider.setValue(Parameters::inDefault);
    inputSlider.setName("In");
    addAndMakeVisible(inputSlider);
    
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::inId, inputSlider);
    
    // Gate knob
    gateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gateSlider.setRange(Parameters::gateMin, Parameters::gateMax, 0.1);
    gateSlider.setValue(Parameters::gateDefault);
    gateSlider.setName(Parameters::gateName);
    addAndMakeVisible(gateSlider);
    
    gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::gateId, gateSlider);
    
    // Output knob
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputSlider.setRange(Parameters::outMin, Parameters::outMax, 0.1);
    outputSlider.setValue(Parameters::outDefault);
    outputSlider.setName("Out");
    addAndMakeVisible(outputSlider);
    
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::outId, outputSlider);

    // --- LIFTER PARAMETERS ---
    // Lifter-range knob
    lifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    lifterRangeSlider.setValue(Parameters::lifterThresDefault);
    lifterRangeSlider.setName("Lift");
    addAndMakeVisible(lifterRangeSlider);
    
    lifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterThresId, lifterRangeSlider);

    // Lifter-time knob
    lifterTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterTimeSlider.setRange(Parameters::lifterTimeMin, Parameters::lifterTimeMax, 0.01);
    lifterTimeSlider.setValue(Parameters::lifterTimeDefault);
    lifterTimeSlider.setName(u8"β");
    addAndMakeVisible(lifterTimeSlider);
    
    lifterTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterTimeId, lifterTimeSlider);
    
    // Mix knob
    lifterMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterMixSlider.setRange(Parameters::lifterMixMin, Parameters::lifterMixMax, 0.01);
    lifterMixSlider.setValue(Parameters::lifterMixDefault);
    lifterMixSlider.setName("Mix");
    addAndMakeVisible(lifterMixSlider);
    
    lifterMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterMixId, lifterMixSlider);

    // --- COMPRESSOR PARAMETERS ---
    // Comp-thres knob
    compThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    compThresSlider.setValue(Parameters::compThresDefault);
    compThresSlider.setName("Comp");
    addAndMakeVisible(compThresSlider);
    
    compThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compThresId, compThresSlider);
    
    // Comp-time knob
    compTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compTimeSlider.setRange(Parameters::compTimeMin, Parameters::compTimeMax, 0.01);
    compTimeSlider.setValue(Parameters::compTimeDefault);
    compTimeSlider.setName(u8"α");
    addAndMakeVisible(compTimeSlider);
    
    compTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compTimeId, compTimeSlider);
    
    // Mix knob
    compMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compMixSlider.setRange(Parameters::compMixMin, Parameters::compMixMax, 0.01);
    compMixSlider.setValue(Parameters::compMixDefault);
    compMixSlider.setName("Mix");
    addAndMakeVisible(compMixSlider);
    
    compMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compMixId, compMixSlider);
    
    // --- CLIPPER ---
    // Clipper button
    clipperButton.setClickingTogglesState(true);
    addAndMakeVisible(clipperButton);
    
    clipperAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::clipperId, clipperButton);
    
    // --- LEVEL METERS ---
    // Level Meters - Create meters based on channel count
    int numInputChannels = processorRef.getTotalNumInputChannels();
    int numOutputChannels = processorRef.getTotalNumOutputChannels();
    
    // Input meters
    inputLeftMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsInputLeft, "In L");
    addAndMakeVisible(*inputLeftMeter);
    
    if (numInputChannels > 1)
    {
        inputRightMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsInputRight, "In R");
        addAndMakeVisible(*inputRightMeter);
    }
    
    // Output meters
    outputLeftMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsOutputLeft, "Out L");
    addAndMakeVisible(*outputLeftMeter);
    
    if (numOutputChannels > 1)
    {
        outputRightMeter = std::make_unique<LevelMeter>(&processorRef.levelMeters.rmsOutputRight, "Out R");
        addAndMakeVisible(*outputRightMeter);
    }
    
    // --- VERSION ---
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
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(1.2683);
    setResizeLimits(520, 410, 1040, 820);
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
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop((int)(area.getHeight() * 0.12f));
    auto sideLArea = area.removeFromLeft((int)(area.getWidth() * 0.08f));
    auto sideRArea = area.removeFromRight((int)(area.getWidth() * 0.08f));
    auto footerArea = area.removeFromBottom(30);
    
    header.setBounds(headerArea);
    footer.setBounds(footerArea);
    sidebarLeft.setBounds(sideLArea);
    sidebarRight.setBounds(sideRArea);
    
    // // --- LEVEL METERS ---
    // --- INPUT METERS ---
    int numInputChannels = processorRef.getTotalNumInputChannels();
    
    if (numInputChannels == 1)
    {
        // Single input channel - meter takes full height
        if (inputLeftMeter)
            inputLeftMeter->setBounds(sideLArea);
    }
    else if (numInputChannels > 1)
    {
        // Stereo input - split height in half
        auto leftHalf = sideLArea.removeFromLeft(sideLArea.getWidth() / 2);
        if (inputLeftMeter)
            inputLeftMeter->setBounds(leftHalf);
        if (inputRightMeter)
            inputRightMeter->setBounds(sideLArea);
    }
    
    // --- OUTPUT METERS ---
    int numOutputChannels = processorRef.getTotalNumOutputChannels();
    
    if (numOutputChannels == 1)
    {
        // Single output channel - meter takes full height
        if (outputLeftMeter)
            outputLeftMeter->setBounds(sideRArea);
    }
    else if (numOutputChannels > 1)
    {
        // Stereo output - split height in half
        auto leftHalf = sideRArea.removeFromLeft(sideRArea.getWidth() / 2);
        if (outputLeftMeter)
            outputLeftMeter->setBounds(leftHalf);
        if (outputRightMeter)
            outputRightMeter->setBounds(sideRArea);
    }
    
    // --- FOOTER ---
    versionTag.setBounds(footerArea);
    
    // --- HEADER CONTROLS ---
    inputSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    gateSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    outputSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    
    clipperButton.setBounds(headerArea.removeFromRight(80)
                                      .reduced(0, 15)
                            );
    
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
    auto lifterSliderWidth = lifterSliderArea.getWidth() / 3;
    
    lifterRangeSlider.setBounds(lifterSliderArea.removeFromLeft(lifterSliderWidth));
    lifterTimeSlider.setBounds(lifterSliderArea.removeFromLeft(lifterSliderWidth));
    lifterMixSlider.setBounds(lifterSliderArea.removeFromLeft(lifterSliderWidth));
    
    // Position sliders inside comp container
    auto compSliderArea = compArea.reduced(10);
    auto compSliderWidth = compSliderArea.getWidth() / 3;
    
    compThresSlider.setBounds(compSliderArea.removeFromLeft(compSliderWidth));
    compTimeSlider.setBounds(compSliderArea.removeFromLeft(compSliderWidth));
    compMixSlider.setBounds(compSliderArea.removeFromLeft(compSliderWidth));
    
    // --- DISPLAY ---
    displayContainer.setBounds (area.reduced(5));
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));

}
