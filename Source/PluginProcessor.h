/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Dry"
#define DAMPING_ID "daming"
#define DAMPING_NAME "Damping"
#define ROOMSIZE_ID "roomsize"
#define ROOMSIZE_NAME "Roomsize"
#define ROOMWIDTH_ID "roomwidth"
#define ROOMWIDTH_NAME "Roomwidth"

#define THRESH_ID "threshold"
#define THRESH_NAME "Threshold"
#define RATIO_ID "ratio"
#define RATIO_NAME "Ratio"
#define ATTACK_ID "attack"
#define ATTACK_NAME "Attack"
#define RELEASE_ID "release"
#define RELEASE_NAME "Release"

#include <JuceHeader.h>

//==============================================================================

namespace Adin {

class Conversions
{
public:
	static float linearToDB(const float& sample) {
		return 20*log10f(sample);
		}
		static float DBtoLinear(const float& sample) {
			return powf(10, sample/10);
		}
	};
};


class NewProjectAudioProcessor  : public juce::AudioProcessor
							#if JucePlugin_Enable_ARA
							 , public juce::AudioProcessorARAExtension
							#endif
{
public:
	//==============================================================================
	NewProjectAudioProcessor();
	~NewProjectAudioProcessor() override;

	//==============================================================================
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

	void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram (int index) override;
	const juce::String getProgramName (int index) override;
	void changeProgramName (int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation (juce::MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;
	
	
	// Function to send user input to GPT-4 API
	void sendUserInputToGPT(const juce::String& userInput);

private:
	//filters
	
	//lpf
	juce::dsp::IIR::Filter<float> lowPassFilterL;
	juce::dsp::IIR::Filter<float> lowPassFilterR;
	juce::dsp::ProcessSpec lowPassProcessSpec;
	juce::AudioParameterFloat * lowPassParameter;
	//hpf
	juce::dsp::IIR::Filter<float> highPassFilterL;
	juce::dsp::IIR::Filter<float> highPassFilterR;
	juce::dsp::ProcessSpec highPassProcessSpec;
	juce::AudioParameterFloat * highPassParameter;
	
	//Equalizer
	
	//low band
	juce::dsp::IIR::Filter<float> lowBandEqL;
	juce::dsp::IIR::Filter<float> lowBandEqR;
	juce::dsp::ProcessSpec lowBandProcessSpec;
	juce::AudioParameterFloat * lowBandParameter;
	juce::AudioParameterFloat* lowGainParameter;
	//mid band
	juce::dsp::IIR::Filter<float> midBandEqL;
	juce::dsp::IIR::Filter<float> midBandEqR;
	juce::dsp::ProcessSpec midBandProcessSpec;
	juce::AudioParameterFloat * midBandParameter;
	juce::AudioParameterFloat* midGainParameter;
	//high band
	juce::dsp::IIR::Filter<float> highBandEqL;
	juce::dsp::IIR::Filter<float> highBandEqR;
	juce::dsp::ProcessSpec highBandProcessSpec;
	juce::AudioParameterFloat * highBandParameter;
	juce::AudioParameterFloat* highGainParameter;
	
	//
	juce::dsp::ProcessSpec spec;
	
	//gain
	//pregain
	juce::AudioParameterFloat* preGainParameter;
	//postgain
	juce::AudioParameterFloat* postGainParameter;

	//Reverb
	juce::AudioParameterFloat* dryWetParameter;
	juce::AudioParameterFloat* dampingParameter;
	juce::AudioParameterFloat* roomSizeParameter;
	juce::AudioParameterFloat* roomWidthParameter;

	//Compressor

	juce::AudioParameterFloat* threshParameter;
	juce::AudioParameterFloat* ratioParameter;
	juce::AudioParameterFloat* attackParameter;
	juce::AudioParameterFloat* releaseParameter;




	juce::dsp::Compressor<float> compressor;

	juce:: Reverb cavern;
	juce::Reverb::Parameters cavernParameters;
	
	juce::URL postDataURL;
	
	
	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
