#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
// #include <juce_dsp/juce_dsp.h>

#if (MSVC)
#include "ipps.h"
#endif

namespace Parameters
{
    // ======= UTILITIES PARAMETERS ========
    constexpr auto inId = "in_gain";
    constexpr auto inName = "Input Gain (dB)";
    constexpr auto inDefault = 0.f;

    constexpr auto gateId = "gate_thresh";
    constexpr auto gateName = "Input Gate";
    constexpr auto gateDefault = -80.f;

    constexpr auto mixId = "mix";
    constexpr auto mixName = "Mix";
    constexpr auto mixDefault = 100.f;

    constexpr auto outId = "out_gain";
    constexpr auto outName = "Output Gain (dB)";
    constexpr auto outDefault = 0.f;

    // ========== OTT PARAMETERS ===========
    // Comp: Determines the ceiling (in dB) for high-level signals to be compressed down
    // Amount:
    constexpr auto amountId = "comp";
    constexpr auto amountName = "Comp (dB)";
    constexpr auto amountDefault = 0.5f;

    // Range: Determines the ceiling (in dB) for low-level signals to be lifted up
    constexpr auto rangeId = "range";
    constexpr auto rangeName = "Range (dB)";
    constexpr auto rangeDefault = 12.f;

    // Attack/Release: Time constants for the dynamics
    constexpr auto attackId = "attack";
    constexpr auto attackName = "Attack (ms)";
    constexpr auto attackDefault = 10.f;


    constexpr auto releaseId = "release";
    constexpr auto releaseName = "Release (ms)";
    constexpr auto releaseDefault = 100.f;
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
    // Public method to access the parameter state for the Editor (GUI)
    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }
    
    // Current DSP-ready parameter values
    struct PluginParameters
    {
        // Utilities
        float inGain = 1.0f;
        float gateThres = 0.0f;
        float wetMix = 1.0f;
        float outGain = 1.0f;
        
        // OTT
        float rangeLinear = 1.0f;
        float downAmount = 0.5f;
        float attackCoeff = 0.0f;
        float releaseCoeff = 0.0f;
    };
    
    PluginParameters currentParameters;
    
    void updateParameters();

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    // using Gain = juce::dsp::Gain<float>;
    
    // Gain inLevel, outLevel;
    
    static float softClipper(float sample)
    {
        return 2.f * sample / (abs(sample) + 1.f);
    }
    
    // 2. State Variables for DSP (must be reset in prepareToPlay)

    // Envelope for smoothing the applied gain (one per channel).
    // This retains the gain value across processBlock calls.
    std::array<float, 2> envelope { 1.0f, 1.0f }; // Initialized to 1.0 (0 dB gain)

    // Temporary storage for the wet (compressed) signal.
    // Needed to calculate the wet/dry mix using the dry signal from the input buffer.
    juce::AudioBuffer<float> compressedBuffer;
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkOTTProcessor)
};
