/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
	NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
	~NewProjectAudioProcessorEditor() override;

	//==============================================================================
	void paint (juce::Graphics&) override;
	void resized() override;
	
	// Function to handle GPT processing button click
	void processButtonClicked();

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	NewProjectAudioProcessor& audioProcessor;
	
	//filters
	juce::AudioParameterFloat* lowPassCutoffFrequencyParameter;
	juce::Slider lowPassKnob;
	juce::Label lowPassLabel;
	
	juce::AudioParameterFloat* highPassCutoffFrequencyParameter;
	juce::Slider highPassKnob;
	juce::Label highPassLabel;
	
	//eq
	//lowband
	juce::AudioParameterFloat* lowBandFrequencyParameter;
	juce::Slider lowBandSlider;
	juce::Label lowBandLabel;
	juce::AudioParameterFloat* lowBandGainParameter;
	juce::Slider lowBandGainSlider;
	juce::Label lowBandGainLabel;
	//midband
	juce::AudioParameterFloat* midBandFrequencyParameter;
	juce::Slider midBandSlider;
	juce::Label midBandLabel;
	juce::AudioParameterFloat* midBandGainParameter;
	juce::Slider midBandGainSlider;
	juce::Label midBandGainLabel;
	//highband
	juce::AudioParameterFloat* highBandFrequencyParameter;
	juce::Slider highBandSlider;
	juce::Label highBandLabel;
	juce::AudioParameterFloat* highBandGainParameter;
	juce::Slider highBandGainSlider;
	juce::Label highBandGainLabel;

	
	//gain
	//pregain
	juce::Slider preGainSlider;
	juce::AudioParameterFloat* preGainParameter;
	juce::Label preGainLabel;
	//postgain
	juce::Slider postGainSlider;
	juce::AudioParameterFloat* postGainParameter;
	juce::Label postGainLabel;

	//Reverb
	//
	//DryWet
	juce::AudioParameterFloat* dryWetParameter;
	juce::Slider dryWetKnob;
	juce::Label dryWetLabel;

	//Damping
	juce::AudioParameterFloat* dampingParameter;
	juce::Slider dampingKnob;
	juce::Label dampingLabel;
	//RoomSize
	juce::AudioParameterFloat* roomSizeParameter;
	juce::Slider roomSizeKnob;
	juce::Label roomSizeLabel;
	//RoomWidth

	juce::AudioParameterFloat* roomWidthParameter;
	juce::Slider roomWidthKnob;
	juce::Label roomWidthLabel;


	//COMPRESSOR

	//THRESHOLD
	juce::AudioParameterFloat* threshParameter;
	juce::Slider threshKnob;
	juce::Label threshLabel;
	//RAtio
	juce::AudioParameterFloat* ratioParameter;
	juce::Slider ratioKnob;
	juce::Label ratioLabel;
	//attack
	juce::AudioParameterFloat* attackParameter;
	juce::Slider attackKnob;
	juce::Label attackLabel;
	//release
	juce::AudioParameterFloat* releaseParameter;
	juce::Slider releaseKnob;
	juce::Label releaseLabel;

	
	//text box for gpt
	juce::TextEditor userInputTextEditor;
	juce::TextButton processButton;



	//Reverb
	/*
	juce::Slider dryWetSlider;
	juce::Label  dryWetLabel;

	juce::Slider dampingSlider;
	juce::Label  dampingLabel;

	juce::Slider roomSizeSlider;
	juce::Label  roomSizeLabel;

	juce::Slider roomWidthSlider;
	juce::Label  roomWidthLabel;
	*/
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)

public:
	/*
	std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dryWetValue;
	std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dampingValue;
	std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeValue;
	std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> roomWidthValue;
	*/

};
