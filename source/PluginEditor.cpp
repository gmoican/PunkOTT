#include "PluginEditor.h"

PluginEditor::PluginEditor (PunkOTTProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
      // inLeftMeter(p),
      // inRightMeter(p),
      // outLeftMeter(p),
      // outRightMeter(p)
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
    
    sidebarLeft.setColour (juce::TextButton::buttonColourId, juce::Colours::red.withAlpha(0.25f));
    sidebarLeft.setEnabled(false);
    // sidebarLeft.setButtonText ("Sidebar L");
    addAndMakeVisible (sidebarLeft);
    
    sidebarRight.setColour (juce::TextButton::buttonColourId, juce::Colours::red.withAlpha(0.25f));
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
    
    // Mix knob
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mixSlider.setRange(Parameters::mixMin, Parameters::mixMax, 0.1);
    mixSlider.setValue(Parameters::mixDefault);
    mixSlider.setName(Parameters::mixName);
    addAndMakeVisible(mixSlider);
    
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::mixId, mixSlider);
    
    // Output knob
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputSlider.setRange(Parameters::outMin, Parameters::outMax, 0.1);
    outputSlider.setValue(Parameters::outDefault);
    outputSlider.setName("Out");
    addAndMakeVisible(outputSlider);
    
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::outId, outputSlider);

    // --- DYNAMIC PARAMETERS ---
    // Lifter-range knob
    lifterRangeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterRangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterRangeSlider.setRange(Parameters::lifterThresMin, Parameters::lifterThresMax, 0.1);
    lifterRangeSlider.setValue(Parameters::lifterThresDefault);
    lifterRangeSlider.setName(Parameters::lifterThresName);
    addAndMakeVisible(lifterRangeSlider);
    
    lifterRangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterThresId, lifterRangeSlider);

    // Lifter-time knob
    lifterTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lifterTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lifterTimeSlider.setRange(Parameters::lifterTimeMin, Parameters::lifterTimeMax, 0.1);
    lifterTimeSlider.setValue(Parameters::lifterTimeDefault);
    lifterTimeSlider.setName(u8"β");
    addAndMakeVisible(lifterTimeSlider);
    
    lifterTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::lifterTimeId, lifterTimeSlider);

    // Comp-thres knob
    compThresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compThresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compThresSlider.setRange(Parameters::compThresMin, Parameters::compThresMax, 0.1);
    compThresSlider.setValue(Parameters::compThresDefault);
    compThresSlider.setName(Parameters::compThresName);
    addAndMakeVisible(compThresSlider);
    
    compThresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compThresId, compThresSlider);
    
    // Comp-time knob
    compTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    compTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    compTimeSlider.setRange(Parameters::compTimeMin, Parameters::compTimeMax, 0.1);
    compTimeSlider.setValue(Parameters::compTimeDefault);
    compTimeSlider.setName(u8"α");
    addAndMakeVisible(compTimeSlider);
    
    compTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::compTimeId, compTimeSlider);
    
    // Clipper button
    clipperButton.setClickingTogglesState(true);
    addAndMakeVisible(clipperButton);
    
    clipperAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::clipperId, clipperButton);
    
    // Level Meters
    // addAndMakeVisible(inLeftMeter);
    // addAndMakeVisible(inRightMeter);
    // addAndMakeVisible(outLeftMeter);
    // addAndMakeVisible(outRightMeter);
    
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

    // Timer in Hertz
    startTimerHz(30);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (520, 410);
}

PluginEditor::~PluginEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::timerCallback()
{
    // inLeftMeter.setLevel(processorRef.levelMeters.rmsInputLeft.load());
    // inRightMeter.setLevel(processorRef.levelMeters.rmsInputRight.load());
    // outLeftMeter.setLevel(processorRef.levelMeters.rmsOutputLeft.load());
    // outRightMeter.setLevel(processorRef.levelMeters.rmsOutputRight.load());
    
    // TODO: FIX
    // inLeftMeter.repaint();
    // inRightMeter.repaint();
    // outLeftMeter.repaint();
    // outRightMeter.repaint();
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
    // area.removeFromBottom(50);
    // inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop(50);
    auto sideLArea = area.removeFromLeft(50);
    auto sideRArea = area.removeFromRight(50);
    auto footerArea = area.removeFromBottom(30);
    
    header.setBounds(headerArea);
    footer.setBounds(footerArea);
    sidebarLeft.setBounds(sideLArea);
    sidebarRight.setBounds(sideRArea);
    
    // // --- LEVEL METERS --- TODO: FIX
    // inLeftMeter.setBounds(sideLArea.removeFromLeft(sideLArea.getHeight()));
    // inRightMeter.setBounds(sideLArea.removeFromLeft(sideLArea.getHeight()));
    // outLeftMeter.setBounds(sideRArea.removeFromLeft(sideRArea.getHeight()));
    // outRightMeter.setBounds(sideRArea.removeFromLeft(sideRArea.getHeight()));
    
    // --- FOOTER ---
    versionTag.setBounds(footerArea);
    
    // --- HEADER CONTROLS ---
    inputSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    gateSlider.setBounds(headerArea.removeFromLeft(headerArea.getHeight()));
    
    outputSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    
    mixSlider.setBounds(headerArea.removeFromRight(headerArea.getHeight()));
    
    clipperButton.setBounds(headerArea.removeFromRight(60)
                                      .reduced(10)
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
    lifterRangeSlider.setBounds(lifterSliderArea.removeFromLeft(lifterSliderArea.getWidth() / 2));
    lifterTimeSlider.setBounds(lifterSliderArea);
    
    // Position sliders inside comp container
    auto compSliderArea = compArea.reduced(10);
    compThresSlider.setBounds(compSliderArea.removeFromLeft(compSliderArea.getWidth() / 2));
    compTimeSlider.setBounds(compSliderArea);
    
    // --- DISPLAY ---
    displayContainer.setBounds (area.reduced(5));
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));

}
