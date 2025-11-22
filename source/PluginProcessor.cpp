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
    
    // Range (Upward)
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::rangeId,
                                                                   Parameters::rangeName,
                                                                   juce::NormalisableRange<float>(0.0f, 24.0f, 0.1f),
                                                                   Parameters::rangeDefault
                                                                   )
                       
                       );
    
    // Compressor Threshold (dB) -> Gets turned to linear in the updater
    dynGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
                                                                   Parameters::compThresId,
                                                                   Parameters::compThresName,
                                                                   juce::NormalisableRange<float>(-18.0f, 0.0f, 0.1f),
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
    
    const float outLeveldB = apvts.getRawParameterValue(Parameters::gateId)->load();
    gateThres = juce::Decibels::decibelsToGain(outLeveldB);
    
    const float gateThres = apvts.getRawParameterValue(Parameters::outId)->load();
    outGain = juce::Decibels::decibelsToGain(gateThres);
    
    const float mixPercent = apvts.getRawParameterValue(Parameters::mixId)->load();
    wetMix = mixPercent / 100.0f;
    
    // --- 2. OTT
    double sampleRate = getSampleRate();
    const float thresLin = apvts.getRawParameterValue(Parameters::compThresId)->load();
    const float attackMS = apvts.getRawParameterValue(Parameters::attackId)->load();
    const float releaseMS = apvts.getRawParameterValue(Parameters::releaseId)->load();
    
    // 1-pole filter coefficient calculation (alpha = e^(-1 / (sampleRate * time_in_seconds)))
    // We use -1.0f/tau as the exponent for the exponential smoothing factor.
    auto attCoeff = std::exp(-1.0f / (sampleRate * (attackMS / 1000.0f)));
    auto relCoeff = std::exp(-1.0f / (sampleRate * (releaseMS / 1000.0f)));
    
    compressor.update(thresLin, attCoeff, relCoeff);
    
    // // TODO: Fix when Lifter is ready
    // const float rangedB = apvts.getRawParameterValue(Parameters::rangeId)->load();
    // currentParameters.rangeLinear = juce::Decibels::decibelsToGain(rangedB);
}

void PunkOTTProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate);
    
    // gate.prepare(sampleRate, samplesPerBlock);
    // lifter.prepare(sampleRate, samplesPerBlock);
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
    // TODO: NOISE GATE HERE
    
    //2. OTT
    compressor.process(compressedBuffer);
    
    // 3. UTILITIES - POST-OTT
    compressedBuffer.applyGain(wetMix);
    buffer.applyGain(1.f - wetMix);
    for (auto i = 0; i < buffer.getNumChannels(); ++i)
        buffer.addFrom(i, 0, compressedBuffer, i, 0, buffer.getNumSamples());
    
    buffer.applyGain(outGain);
    
    // // Core processing
    // for (int channel = 0; channel < totalNumInputChannels; ++channel)
    // {
    //     auto* channelData = buffer.getWritePointer (channel);
    //     float* compressedData = compressedBuffer.getWritePointer(channel);
    //     auto currentEnvelope = envelope[channel];
    //
    //     for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    //     {
    //         float inSample = channelData[sample];
    //
    //         // 1. UTILITIES - PRE-OTT
    //         float processedSample = inSample * inGain;
    //
    //         // TODO: Implement better noise gate
    //         if (std::abs(processedSample) < gateThres)
    //             processedSample = 0.0f;
            
    //         /*
    //         // Store the processed sample back temporarily (it will be overwritten later)
    //         // We use the temporary buffer for the WET signal calculation:
    //         float magnitude = std::abs (processedSample);
    //
    //         // 2. OTT
    //         float G_up = 1.0f;    // Upward Gain Component
    //         float G_down = 1.0f;  // Downward Gain Component
    //
    //         // 1. UPWARD COMPRESSION LOGIC (Boosting quiet signals)
    //         if (magnitude < rangeLinear)
    //         {
    //             // Gain to push the signal up to the Range threshold
    //             G_up = rangeLinear / magnitude;
    //         }
    //
    //         // 2. DOWNWARD COMPRESSION LOGIC (Limiting loud signals)
    //         if (magnitude > downThreshold)
    //         {
    //             // Calculate the amount signal is above threshold (in linear)
    //             float excursion = magnitude - downThreshold;
    //
    //             // G = Threshold + (Excursion / Ratio) -> Attenuation needed for peak
    //             // The linear gain reduction factor:
    //             // G_down is 1.0 (no reduction) if ratio=1, and smaller if ratio > 1
    //             G_down = downThreshold + (excursion / ratio);
    //             G_down = downThreshold / G_down; // Convert magnitude scaling back to VCA gain
    //         }
            
    //         // 3. COMBINED TARGET GAIN & SMOOTHING
    //         const float targetGain = G_up * G_down;
    //
    //         // Attack/Release decision logic
    //         float alpha = (targetGain > currentEnvelope) ? attackCoeff : releaseCoeff;
    //
    //         // Simple 1-pole filter to smooth the applied gain
    //         currentEnvelope = (alpha * currentEnvelope) + ((1.0f - alpha) * targetGain);
    //
    //         // Apply smoothed gain to the sample
    //         compressedData[sample] = processedSample * currentEnvelope;
    //          */
    //
    //         // 3. UTILITIES - POST-OTT
    //         channelData[sample] = (processedSample * wetMix + inSample * (1.f - wetMix)) * outGain;
    //     }
    // }
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
