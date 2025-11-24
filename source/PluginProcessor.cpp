#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkOTTProcessor::PunkOTTProcessor()
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ), apvts(*this, nullptr, "Parameters", createParams())
{
}

PunkOTTProcessor::~PunkOTTProcessor()
{
}

//==============================================================================
const juce::String PunkOTTProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PunkOTTProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PunkOTTProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PunkOTTProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PunkOTTProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PunkOTTProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PunkOTTProcessor::getCurrentProgram()
{
    return 0;
}

void PunkOTTProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PunkOTTProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PunkOTTProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
// TODO: Fine tune OTT parameter ranges
juce::AudioProcessorValueTreeState::ParameterLayout PunkOTTProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // --- Utility Parameters Group ---
    auto utilsGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                           "utils",       // Group ID (must be unique string)
                                                                           "Utilities",   // Group Name (displayed in host)
                                                                           " | "          // Separator string for hosts that flatten the hierarchy
                                                                           );
    
    // Input Level
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::inId,
                                                                     Parameters::inName,
                                                                     juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
                                                                     Parameters::inDefault
                                                                     )
                         );
    
    // Input Noise Gate
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::gateId,
                                                                     Parameters::gateName,
                                                                     juce::NormalisableRange<float>(-90.0f, 0.0f, 0.1f),
                                                                     Parameters::gateDefault
                                                                     )
                         );
    
    // Mix (Wet/Dry)
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::mixId,
                                                                     Parameters::mixName,
                                                                     juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
                                                                     Parameters::mixDefault
                                                                     )
                         );
    
    // Output Level
    utilsGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                     Parameters::outId,
                                                                     Parameters::outName,
                                                                     juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
                                                                     Parameters::outDefault
                                                                     )
                         );
    
    layout.add(std::move(utilsGroup));
    
    // --- Compressor Parameters Group ---
    
    auto dynGroup = std::make_unique<juce::AudioProcessorParameterGroup>(
                                                                         "dyn",         // Group ID (must be unique string)
                                                                         "Dynamics",    // Group Name (displayed in host)
                                                                         " | "          // Separator string for hosts that flatten the hierarchy
                                                                         );
    
    // Range (Upward) -> TODO: Not yet developed, range should be adjusted
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::rangeId,
                                                                   Parameters::rangeName,
                                                                   juce::NormalisableRange<float>(-60.0f, -20.0f, 0.1f),
                                                                   Parameters::rangeDefault
                                                                   )
                       
                       );
    
    // Compressor Threshold (dB)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::compThresId,
                                                                   Parameters::compThresName,
                                                                   juce::NormalisableRange<float>(-24.0f, 0.0f, 0.1f),
                                                                   Parameters::compThresDefault
                                                                   )
                       
                       );
    
    // Attack Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::attackId,
                                                                   Parameters::attackName,
                                                                   juce::NormalisableRange<float>(0.1f, 500.0f, 0.1f, 0.5f, false),
                                                                   Parameters::attackDefault
                                                                   )
                       
                       );
    
    // Release Time
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::releaseId,
                                                                   Parameters::releaseName,
                                                                   juce::NormalisableRange<float>(10.0f, 2000.0f, 1.0f, 0.5f, false),
                                                                   Parameters::releaseDefault
                                                                   )
                       
                       );
    
    // Clipper
    dynGroup->addChild(std::make_unique<juce::AudioParameterBool>(Parameters::clipperId,
                                                                 Parameters::clipperName,
                                                                 Parameters::clipperDefault
                                                                 )
                       );
    
    layout.add(std::move(dynGroup));
    
    return layout;
}

//==============================================================================
void PunkOTTProcessor::updateParameters()
{
    // TODO: For final version, this function should only be called when parameters actually move -> APVTS::addParameterListener
    // --- 1. Utilities
    const float inLeveldB = apvts.getRawParameterValue(Parameters::inId)->load();
    inGain = juce::Decibels::decibelsToGain(inLeveldB);
    
    const float gatedB = apvts.getRawParameterValue(Parameters::gateId)->load();
    gate.updateThres(gatedB);
    
    const float mixPercent = apvts.getRawParameterValue(Parameters::mixId)->load();
    wetMix = mixPercent / 100.0f;
    
    const float outdB = apvts.getRawParameterValue(Parameters::outId)->load();
    outGain = juce::Decibels::decibelsToGain(outdB);
    
    // --- 2. OTT
    float sampleRate = (float) getSampleRate();
    
    const float rangedB = apvts.getRawParameterValue(Parameters::rangeId)->load();
    const float thresdB = apvts.getRawParameterValue(Parameters::compThresId)->load();
    
    const float attackMS = apvts.getRawParameterValue(Parameters::attackId)->load();
    const float releaseMS = apvts.getRawParameterValue(Parameters::releaseId)->load();
    
    lifter.updateRange(rangedB);
    lifter.updateAttack(sampleRate, attackMS);
    lifter.updateRelease(sampleRate, releaseMS);
    compressor.updateThres(thresdB);
    compressor.updateAttack(sampleRate, attackMS);
    compressor.updateRelease(sampleRate, releaseMS);
    
    clipperState = apvts.getRawParameterValue(Parameters::clipperId)->load();
}

void PunkOTTProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::ignoreUnused(sampleRate);
    
    gate.prepare(sampleRate, samplesPerBlock);
    gate.updateAttack((float) sampleRate, 100.f);
    gate.updateRelease((float) sampleRate, 30.f);
    lifter.prepare(sampleRate, samplesPerBlock);
    compressor.prepare(sampleRate, samplesPerBlock);
    
    compressedBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);
    
    updateParameters();
}

void PunkOTTProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PunkOTTProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}

void PunkOTTProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear any unused output channel
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Safety check: compressedBuffer is ready
    if (compressedBuffer.getNumSamples() != buffer.getNumSamples())
        compressedBuffer.setSize(totalNumOutputChannels, buffer.getNumSamples());
    
    compressedBuffer.clear();
    
    // Update params - TODO: this has to be triggered by a listener in the final version
    updateParameters();
    
    // 0. Buffer copy
    compressedBuffer.makeCopyOf(buffer);
    
    // 1. UTILITIES - PRE-OTT
    compressedBuffer.applyGain(inGain);
    gate.process(compressedBuffer);
    
    //2. OTT
    lifter.process(compressedBuffer);
    compressor.processFF(compressedBuffer);
    if (clipperState) {
        clipper.applySoftClipper(compressedBuffer);
    }
    
    // 3. UTILITIES - POST-OTT
    compressedBuffer.applyGain(wetMix);
    buffer.applyGain(1.f - wetMix);
    for (auto i = 0; i < buffer.getNumChannels(); ++i)
        buffer.addFrom(i, 0, compressedBuffer, i, 0, buffer.getNumSamples());
    
    buffer.applyGain(outGain);
}

//==============================================================================
bool PunkOTTProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PunkOTTProcessor::createEditor()
{
    // return new PluginEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void PunkOTTProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PunkOTTProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PunkOTTProcessor();
}
