#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "dsp/Gate.h"
#include "dsp/Lifter.h"
#include "dsp/Compressor.h"
#include "dsp/Clipper.h"

#if (MSVC)
#include "ipps.h"
#endif

namespace Parameters
{
    // ======= UTILITY PARAMETERS ========
    constexpr auto inId = "in_gain";
    constexpr auto inName = "Input Gain (dB)";
    constexpr auto inDefault = 0.0f;
    constexpr auto inMin = -24.0f;
    constexpr auto inMax = 24.0f;

    constexpr auto gateId = "gate_thresh";
    constexpr auto gateName = "Input Gate";
    constexpr auto gateDefault = -80.f;
    constexpr auto gateMin = -90.0f;
    constexpr auto gateMax = 0.0f;

    constexpr auto mixId = "mix";
    constexpr auto mixName = "Mix";
    constexpr auto mixDefault = 100.f;
    constexpr auto mixMin = 0.0f;
    constexpr auto mixMax = 100.0f;

    constexpr auto outId = "out_gain";
    constexpr auto outName = "Output Gain (dB)";
    constexpr auto outDefault = 0.f;
    constexpr auto outMin = -24.0f;
    constexpr auto outMax = 24.0f;

    // ========== OTT PARAMETERS ===========
    // Comp: Determines the ceiling (in dB) for high-level signals to be compressed down
    constexpr auto compThresId = "comp";
    constexpr auto compThresName = "Comp Threshold (dB)";
    constexpr auto compThresDefault = -12.0f;
    constexpr auto compThresMin = -24.0f;
    constexpr auto compThresMax = 0.0f;

    // Comp time control
    constexpr auto compTimeId = "compressorTime";
    constexpr auto compTimeName = "Compressor Time Control";
    constexpr auto compTimeDefault = 0.5f;
    constexpr auto compTimeMin = 0.0f;
    constexpr auto compTimeMax = 1.0f;

    // Lifter: Determines the ceiling (in dB) for low-level signals to be lifted up
    constexpr auto lifterThresId = "lifter";
    constexpr auto lifterThresName = "Lifter Thres (dB)";
    constexpr auto lifterThresDefault = -40.0f;
    constexpr auto lifterThresMin = -80.0f;
    constexpr auto lifterThresMax = 0.0f;

    // Lifter time control
    constexpr auto lifterTimeId = "lifterTime";
    constexpr auto lifterTimeName = "Lifter Time Control";
    constexpr auto lifterTimeDefault = 0.5f;
    constexpr auto lifterTimeMin = 0.0f;
    constexpr auto lifterTimeMax = 1.0f;

    // Clipper: Applies a soft-clipping function after the dynamics processing
    constexpr auto clipperId = "clipper";
    constexpr auto clipperName = "Clipper";
    constexpr auto clipperDefault = false;
}

class PunkOTTProcessor : public juce::AudioProcessor
{
public:
    PunkOTTProcessor();
    ~PunkOTTProcessor() override;

    // DEFAULT STUFF ===============================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // MY STUFF ====================================================================
    juce::AudioProcessorValueTreeState apvts;
    
    void updateParameters();

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    // --- INTERNAL PARAMETER HANDLING ---
    // Utilities
    float inGain = 1.0f;
    float wetMix = 1.0f;
    float outGain = 1.0f;
    Gate gate;
    
    // --- OTT PROCESSORS ---
    Lifter lifter;
    Compressor compressor;
    
    Clipper clipper;
    bool clipperState = false;

    // Temporary storage for the wet (compressed) signal.
    // Needed to calculate the wet/dry mix using the dry signal from the input buffer.
    juce::AudioBuffer<float> compressedBuffer;
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkOTTProcessor)
};
