/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Dry"
#define DAMPING_ID "daming"
#define DAMPING_NAME "Damping"
#define ROOMSIZE_ID "roomsize"
#define ROOMSIZE_NAME "Roomsize"
#define ROOMWIDTH_ID "roomwidth"
#define ROOMWIDTH_NAME "Roomwidth"

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
	: AudioProcessorEditor (&p), audioProcessor (p), processButton("Process")
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize (820, 780);
	
	//making each slider visible
	addAndMakeVisible(preGainSlider);
	addAndMakeVisible(postGainSlider);
	
	addAndMakeVisible(lowPassKnob);
	addAndMakeVisible(highPassKnob);
	
	addAndMakeVisible(lowBandSlider);
	addAndMakeVisible(lowBandGainSlider);
	addAndMakeVisible(midBandSlider);
	addAndMakeVisible(midBandGainSlider);
	addAndMakeVisible(highBandSlider);
	addAndMakeVisible(highBandGainSlider);
	
	

	addAndMakeVisible(dryWetKnob);
	addAndMakeVisible(dampingKnob);
	addAndMakeVisible(roomSizeKnob);
	addAndMakeVisible(roomWidthKnob);

	addAndMakeVisible(threshKnob);
	addAndMakeVisible(ratioKnob);
	addAndMakeVisible(attackKnob);
	addAndMakeVisible(releaseKnob);
	
	// Set up the button
	addAndMakeVisible(processButton);
	processButton.onClick = [this] { processButtonClicked(); };
	
	//initalization of texteditor
	userInputTextEditor.setMultiLine(true);
	userInputTextEditor.setReturnKeyStartsNewLine(true);
	// Set other properties as needed
	addAndMakeVisible(userInputTextEditor);
	
	
	//attaching text to the GUI components
	//Filters
	lowPassLabel.setText("Low Pass", juce::dontSendNotification);
	lowPassLabel.attachToComponent(&lowPassKnob, false);
	highPassLabel.setText("High Pass", juce::dontSendNotification);
	highPassLabel.attachToComponent(&highPassKnob, false);
	//Eq
	lowBandLabel.setText("Low", juce::dontSendNotification);
	lowBandLabel.attachToComponent(&lowBandSlider, false);
	lowBandGainLabel.setText("Gain", juce::dontSendNotification);
	lowBandGainLabel.attachToComponent(&lowBandGainSlider, false);
	midBandLabel.setText("Mid", juce::dontSendNotification);
	midBandLabel.attachToComponent(&midBandSlider, false);
	midBandGainLabel.setText("Gain", juce::dontSendNotification);
	midBandGainLabel.attachToComponent(&midBandGainSlider, false);
	highBandLabel.setText("High", juce::dontSendNotification);
	highBandLabel.attachToComponent(&highBandSlider, false);
	highBandGainLabel.setText("Gain", juce::dontSendNotification);
	highBandGainLabel.attachToComponent(&highBandGainSlider, false);
	
	//Gain
	preGainLabel.setText("Pre-Gain", juce::dontSendNotification);
	preGainLabel.attachToComponent(&preGainSlider, false);
	postGainLabel.setText("Post-Gain", juce::dontSendNotification);
	postGainLabel.attachToComponent(&postGainSlider, false);
	
	//verb
	dryWetLabel.attachToComponent(&dryWetKnob, false);
	dryWetLabel.setText("Dry/Wet", juce::dontSendNotification);

	dampingLabel.attachToComponent(&dampingKnob, false);
	dampingLabel.setText(DAMPING_NAME, juce::dontSendNotification);

	roomSizeLabel.attachToComponent(&roomSizeKnob, false);
	roomSizeLabel.setText(ROOMSIZE_NAME, juce::dontSendNotification);

	roomWidthLabel.attachToComponent(&roomWidthKnob, false);
	roomWidthLabel.setText(ROOMWIDTH_NAME, juce::dontSendNotification);
	
	//comp
	threshLabel.attachToComponent(&threshKnob, false);
	threshLabel.setText(THRESH_NAME, juce::dontSendNotification);

	ratioLabel.attachToComponent(&ratioKnob, false);
	ratioLabel.setText(RATIO_NAME, juce::dontSendNotification);

	attackLabel.attachToComponent(&attackKnob, false);
	attackLabel.setText(ATTACK_NAME, juce::dontSendNotification);

	releaseLabel.attachToComponent(&releaseKnob, false);
	releaseLabel.setText(RELEASE_NAME, juce::dontSendNotification);

	
	//getting parameters
	auto parameterTree = audioProcessor.getParameters();
	//pregain
	preGainParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(0));
	//filters
	lowPassCutoffFrequencyParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(1));
	highPassCutoffFrequencyParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(2));
	//EQ
	lowBandFrequencyParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(3));
	lowBandGainParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(4));
	midBandFrequencyParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(5));
	midBandGainParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(6));
	highBandFrequencyParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(7));
	highBandGainParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(8));
	//postGain
	postGainParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(9));
	//verb
	dryWetParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(10));
	dampingParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(11));
	roomSizeParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(12));
	roomWidthParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(13));

	//Comp
	threshParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(14));
	ratioParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(15));
	attackParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(16));
	releaseParameter = static_cast<juce::AudioParameterFloat*>(parameterTree.getUnchecked(17));

	
	//setting the style of each GUI component and attaching it to the proper parameters
	lowPassKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	lowPassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	lowPassKnob.setSkewFactor(0.2);
	lowPassKnob.setTextValueSuffix("hz");
	lowPassKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	lowPassKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	lowPassKnob.setRange(lowPassCutoffFrequencyParameter->range.start, lowPassCutoffFrequencyParameter->range.end, .01);
	lowPassKnob.setValue(lowPassCutoffFrequencyParameter->get());
	lowPassKnob.onValueChange = [this]
	{
		*lowPassCutoffFrequencyParameter = lowPassKnob.getValue();
	};
	
	highPassKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	highPassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	highPassKnob.setSkewFactor(0.2);
	highPassKnob.setTextValueSuffix("hz");
	highPassKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	highPassKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	highPassKnob.setRange(highPassCutoffFrequencyParameter->range.start, highPassCutoffFrequencyParameter->range.end, .01);
	highPassKnob.setValue(highPassCutoffFrequencyParameter->get());
	highPassKnob.onValueChange = [this]
	{
		*highPassCutoffFrequencyParameter = highPassKnob.getValue();
	};
	
	lowBandSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	lowBandSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	lowBandSlider.setSkewFactor(1.1);
	lowBandSlider.setTextValueSuffix("hz");
	lowBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	lowBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	lowBandSlider.setRange(lowBandFrequencyParameter->range.start, lowBandFrequencyParameter->range.end, .01);
	lowBandSlider.setValue(lowBandFrequencyParameter->get());
	lowBandSlider.onValueChange = [this]
	{
		*lowBandFrequencyParameter = lowBandSlider.getValue();
	};
	
	lowBandGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	lowBandGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	lowBandGainSlider.setSkewFactor(1.1);
	lowBandGainSlider.setTextValueSuffix(" db");
	lowBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	lowBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	lowBandGainSlider.setRange(lowBandGainParameter->range.start, lowBandGainParameter->range.end, .01);
	lowBandGainSlider.setValue(lowBandGainParameter->get());
	lowBandGainSlider.onValueChange = [this]
	{
		*lowBandGainParameter = lowBandGainSlider.getValue();
	};
	
	midBandSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	midBandSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	midBandSlider.setSkewFactor(1.1);
	midBandSlider.setTextValueSuffix("hz");
	midBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	midBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	midBandSlider.setRange(midBandFrequencyParameter->range.start, midBandFrequencyParameter->range.end, .01);
	midBandSlider.setValue(midBandFrequencyParameter->get());
	midBandSlider.onValueChange = [this]
	{
		*midBandFrequencyParameter = midBandSlider.getValue();
	};

	midBandGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	midBandGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	midBandGainSlider.setSkewFactor(1.1);
	midBandGainSlider.setTextValueSuffix(" db");
	midBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	midBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	midBandGainSlider.setRange(midBandGainParameter->range.start, midBandGainParameter->range.end, .01);
	midBandGainSlider.setValue(midBandGainParameter->get());
	midBandGainSlider.onValueChange = [this]
	{
		*midBandGainParameter = midBandGainSlider.getValue();
	};

	highBandSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	highBandSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	highBandSlider.setSkewFactor(1.1);
	highBandSlider.setTextValueSuffix("hz");
	highBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	highBandSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	highBandSlider.setRange(highBandFrequencyParameter->range.start, highBandFrequencyParameter->range.end, .01);
	highBandSlider.setValue(highBandFrequencyParameter->get());
	highBandSlider.onValueChange = [this]
	{
		*highBandFrequencyParameter = highBandSlider.getValue();
	};

	highBandGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	highBandGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	highBandGainSlider.setSkewFactor(1.1);
	highBandGainSlider.setTextValueSuffix(" db");
	highBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::tan);
	highBandGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::bisque);
	highBandGainSlider.setRange(highBandGainParameter->range.start, highBandGainParameter->range.end, .01);
	highBandGainSlider.setValue(highBandGainParameter->get());
	highBandGainSlider.onValueChange = [this]
	{
		*highBandGainParameter = highBandGainSlider.getValue();
	};

	
	preGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	preGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	preGainSlider.setTextValueSuffix(" db");
	preGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	preGainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	preGainSlider.setRange(preGainParameter->range.start, preGainParameter->range.end, 0.01);
	preGainSlider.setValue(preGainParameter->get());
	preGainSlider.onValueChange = [this]
	{
		*preGainParameter = preGainSlider.getValue();
	};
	
	postGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	postGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	postGainSlider.setTextValueSuffix(" db");
	postGainSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	postGainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	postGainSlider.setRange(postGainParameter->range.start, postGainParameter->range.end, 0.01);
	postGainSlider.setValue(postGainParameter->get());
	postGainSlider.onValueChange = [this]
	{
		*postGainParameter = postGainSlider.getValue();
	};
	
	//verb
	dryWetKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	dryWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	dryWetKnob.setTextValueSuffix(" %");
	dryWetKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	dryWetKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	dryWetKnob.setRange(dryWetParameter->range.start, dryWetParameter->range.end, 0.01);
	dryWetKnob.setValue(dryWetParameter->get());
	dryWetKnob.onValueChange = [this]
	{
		*dryWetParameter = dryWetKnob.getValue();
	};


	dampingKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	dampingKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	dampingKnob.setTextValueSuffix(" %");
	dampingKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	dampingKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	dampingKnob.setRange(dampingParameter->range.start, dampingParameter->range.end, 0.01);
	dampingKnob.setValue(dampingParameter->get());
	dampingKnob.onValueChange = [this]
	{
		*dampingParameter = dampingKnob.getValue();
	};


	roomSizeKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	roomSizeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	roomSizeKnob.setTextValueSuffix(" %");
	roomSizeKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	roomSizeKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	roomSizeKnob.setRange(roomSizeParameter->range.start, roomSizeParameter->range.end, 0.01);
	roomSizeKnob.setValue(roomSizeParameter->get());
	roomSizeKnob.onValueChange = [this]
	{
		*roomSizeParameter = roomSizeKnob.getValue();
	};


	roomWidthKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	roomWidthKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	roomWidthKnob.setTextValueSuffix(" %");
	roomWidthKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::lime);
	roomWidthKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::seagreen);
	roomWidthKnob.setRange(roomWidthParameter->range.start, roomWidthParameter->range.end, 0.01);
	roomWidthKnob.setValue(roomWidthParameter->get());
	roomWidthKnob.onValueChange = [this]
	{
		*roomWidthParameter = roomWidthKnob.getValue();
	};

	// Compressor
	threshKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	threshKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	threshKnob.setTextValueSuffix(" db");
	threshKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::cyan);
	threshKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blueviolet);
	threshKnob.setRange(threshParameter->range.start, threshParameter->range.end, 0.01);
	threshKnob.setValue(threshParameter->get());
	threshKnob.onValueChange = [this]
	{
		*threshParameter = threshKnob.getValue();
	};


	ratioKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	ratioKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	ratioKnob.setTextValueSuffix(" %");
	ratioKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
	ratioKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blueviolet);
	ratioKnob.setRange(ratioParameter->range.start, ratioParameter->range.end, 0.01);
	ratioKnob.setValue(ratioParameter->get());
	ratioKnob.onValueChange = [this]
	{
		*ratioParameter = ratioKnob.getValue();
	};



	attackKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	attackKnob.setTextValueSuffix(" ms");
	attackKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
	attackKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blueviolet);
	attackKnob.setRange(attackParameter->range.start, attackParameter->range.end, 0.01);
	attackKnob.setValue(attackParameter->get());
	attackKnob.onValueChange = [this]
	{
		*attackParameter = attackKnob.getValue();
	};

	releaseKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 120, 18);
	releaseKnob.setTextValueSuffix(" ms");
	releaseKnob.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
	releaseKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blueviolet);
	releaseKnob.setRange(releaseParameter->range.start, releaseParameter->range.end, 0.01);
	releaseKnob.setValue(releaseParameter->get());
	releaseKnob.onValueChange = [this]
	{
		*releaseParameter = releaseKnob.getValue();
	};
	
	processButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
	processButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);



	
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

	g.setColour (juce::Colours::mintcream);
	g.setFont (15.0f);
	//g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}


// Implement the button click handler
void NewProjectAudioProcessorEditor::processButtonClicked()
{
	// Get user input from the TextEditor
	juce::String userInput = userInputTextEditor.getText();

	// Send user input to GPT
	audioProcessor.sendUserInputToGPT(userInput);
}

void NewProjectAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	auto x = getWidth() / 19;
	auto y = getHeight() / 14;
	
	preGainSlider.setBounds(1 * x, 3 * y, 1.5 * x, 7 * y);
	postGainSlider.setBounds(17 * x, 3 * y, 1.5 * x, 7 * y);
	
	lowBandSlider.setBounds(3 * x, .75 * y, 1.5 * x, 2.00 * y);
	lowBandGainSlider.setBounds(3 * x, 3.5 * y, 1 * x, 2.00 * y);
	midBandSlider.setBounds(5 * x, .75 * y, 1.5 * x, 2.00 * y);
	midBandGainSlider.setBounds(5 * x, 3.5 * y, 1 * x, 2.00 * y);
	highBandSlider.setBounds(7 * x, .75 * y, 1.5 * x, 2.00 * y);
	highBandGainSlider.setBounds(7 * x, 3.5 * y, 1 * x, 2.00 * y);
	
	threshKnob.setBounds(3.5 * x, 7 * y, 1.5 * x, 2 * y);
	ratioKnob.setBounds(6.5 * x, 7 * y, 1 * x, 2 * y);
	attackKnob.setBounds(3.5 * x, 9.5 * y, 1.5 * x, 2 * y);
	releaseKnob.setBounds(6.5 * x, 9.5 * y, 1 * x, 2 * y);

	dryWetKnob.setBounds(9 * x, 1 * y, 1.5 * x, 4 * y);
	dampingKnob.setBounds(11 * x, 1 * y, 1.5 * x, 4 * y);
	roomSizeKnob.setBounds(13 * x, 1 * y, 1.5 * x, 4 * y);
	roomWidthKnob.setBounds(15 * x, 1 * y, 1.5 * x, 4 * y);
	lowPassKnob.setBounds(10 * x, 7 * y, 1.5 * x, 4 * y);
	highPassKnob.setBounds(14 * x, 7 * y, 1.5 * x, 4 * y);
	
	//Text box for GPT Queries
	auto textEditorWidth = getWidth() - 2 * getX(); // Use the width of the editor
	auto textEditorHeight = 100; // Set the desired height for the text box
	auto textEditorX = getX();
	auto textEditorY = getHeight() - textEditorHeight;
	userInputTextEditor.setBounds(textEditorX, textEditorY, textEditorWidth, textEditorHeight);
	// Set the bounds for the process button
	processButton.setBounds(16 * x, 10.5 * y, 2 * x, 1.5 * y);

}
