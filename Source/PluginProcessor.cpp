/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	 : AudioProcessor (BusesProperties()
					 #if ! JucePlugin_IsMidiEffect
					  #if ! JucePlugin_IsSynth
					   .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
					  #endif
					   .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
					 #endif
					   )

#endif



{
	//pregain parameters
	preGainParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 1", 1}, "Gain", -24.0f, 12.0f, 0.0f);
	addParameter(preGainParameter);
	
	//filter parameters
	lowPassParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 2", 1}, "Low Pass", 20, 20000, 20000);
	addParameter(lowPassParameter);
	highPassParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 3", 1}, "High pass", 20, 20000, 20);
	addParameter(highPassParameter);
	
	//eq parameters
	lowBandParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 4", 1}, "Low Band", 20, 500, 120);
	addParameter(lowBandParameter);
	lowGainParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 5", 1}, "Low Gain", -24.0f, 12.0f, 0.0f);
	addParameter(lowGainParameter);
	
	midBandParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 6", 1}, "Mid Band", 120, 3000, 800);
	addParameter(midBandParameter);
	midGainParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 7", 1}, "Mid Gain", -24.0f, 12.0f, 0.0f);
	addParameter(midGainParameter);
	
	highBandParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 8", 1}, "High Band", 500, 20000, 6000);
	addParameter(highBandParameter);
	highGainParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 9", 1}, "High Gain", -24.0f, 12.0f, 0.0f);
	addParameter(highGainParameter);
	
	
	//postgain parameters
	postGainParameter = new juce::AudioParameterFloat(juce::ParameterID{"Parameter 10", 1}, "Gain", -24.0f, 12.0f, 0.0f);
	addParameter(postGainParameter);

	//Reverb Parameters

	dryWetParameter = new juce::AudioParameterFloat(juce::ParameterID{ DRYWET_ID, 1 }, DRYWET_NAME, 0.0f, 1.0f, 0.5);
	addParameter(dryWetParameter);

	dampingParameter = new juce::AudioParameterFloat(juce::ParameterID{ DAMPING_ID, 1 }, DAMPING_NAME, 0.0f, 1.0f, 0.1);
	addParameter(dampingParameter);

	roomSizeParameter = new juce::AudioParameterFloat(juce::ParameterID{ ROOMSIZE_ID, 1 }, ROOMSIZE_NAME, 0.0f, 1.0f, 0.5);
	addParameter(roomSizeParameter);

	roomWidthParameter = new juce::AudioParameterFloat(juce::ParameterID{ ROOMWIDTH_ID, 1 }, ROOMWIDTH_NAME, 0.0f, 1.0f, 0.5);
	addParameter(roomWidthParameter);

	//Compressor Parameters
	threshParameter = new juce::AudioParameterFloat(juce::ParameterID{ THRESH_ID, 1 }, THRESH_ID, -60.0f, 10.0f, 0.0f);
	addParameter(threshParameter);

	ratioParameter = new juce::AudioParameterFloat(juce::ParameterID{ RATIO_ID, 1 }, RATIO_NAME, 1.0f, 20.0f, 1.0);
	addParameter(ratioParameter);

	attackParameter = new juce::AudioParameterFloat(juce::ParameterID{ ATTACK_ID, 1 }, ATTACK_NAME, 0.0f, 200.0f, 50);
	addParameter(attackParameter);

	releaseParameter = new juce::AudioParameterFloat(juce::ParameterID{ RELEASE_ID, 1 }, RELEASE_NAME, 5.0f, 5000.0f, 5.0f);
	addParameter(releaseParameter);



   

}


void NewProjectAudioProcessor::sendUserInputToGPT(const juce::String& userInput)
{
	juce::URL url("https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
	juce::StringPairArray headers;
	headers.set("Content-Type", "application/json");
	juce::String apiKey = "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";
	headers.set("Authorization", "Bearer " + apiKey);
	DBG("API Key: " << apiKey);
	DBG("Constructed URL: " << url.toString(true));

	// Construct the request body with the conversation format
	juce::String requestBody = R"({
		"model": "gpt-3.5-turbo",
		"messages": [
			{"role": "system", "content": "You are a helpful assistant."},
			{"role": "user", "content": ")" + userInput + R"("}
		]
	})";
	DBG("Request Body: " << requestBody);

	// Create InputStreamOptions and set other options
	juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inPostData);

	DBG("Constructed URL: " << url.toString(true));
	DBG("Constructed Headers: " << headers.getDescription());
	DBG("Request Body: " << requestBody);

	// Make the HTTP request
	auto response = url.createInputStream(options);

	// Set POST data separately if using WebInputStream
	if (auto* stream = dynamic_cast<juce::WebInputStream*>(response.get()))
	{
		stream->withExtraHeaders(headers.getDescription() + "\r\n").withConnectionTimeout(5000).connect(nullptr); // This may be optional depending on your needs

		stream->setPosition(0); // Reset position to the start

		// Send POST data in chunks
		int totalBytes = requestBody.getNumBytesAsUTF8();
		int bytesSent = 0;
		int chunkSize = 4096;
		while (bytesSent < totalBytes)
		{
			int bytesToSend = std::min(chunkSize, totalBytes - bytesSent);
			stream->read(const_cast<char*>(requestBody.toRawUTF8()) + bytesSent, bytesToSend);
			bytesSent += bytesToSend;
		}
	}

	juce::String jsonResponse = response->readEntireStreamAsString();
	DBG("GPT API Response: " << jsonResponse);
	// Implement the logic to extract parameter values from jsonResponse
	// Set parameter values accordingly
}








//juce::StringPairArray headers;
//
//	headers.set("Content-Type", "application/json");
//
//	headers.set("Authorization", "Bearer " + apiKey);
//
//
//
//	// Construct the request body with the conversation format
//
//	juce::String requestBody = R"({
//
//		"model": "gpt-3.5-turbo",
//
//		"messages": [
//
//			{"role": "system", "content": "You are a helpful assistant."},
//
//			{"role": "user", "content": "give me cool EQ settings"}
//
//		]
//
//	})";
//
//
//
//	// Make the HTTP request
//
//	juce::URL::OpenStreamProgressCallback* progressCallback = nullptr;
//
//	auto response = url.withPOSTData(requestBody).withExtraHeaders(headers).createInputStream(true, nullptr, nullptr, {}, 5000, progressCallback);
//
//juce::String jsonResponse = response->readEntireStreamAsString();












NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
	if(preGainParameter != nullptr)
		delete preGainParameter;
	if(postGainParameter != nullptr)
		delete postGainParameter;
}


//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
	return true;
   #else
	return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
	return true;
   #else
	return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
	return true;
   #else
	return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
	return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
	return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	// Initializing filters
	
	//lpf
	lowPassProcessSpec.sampleRate = sampleRate;
	lowPassProcessSpec.maximumBlockSize = samplesPerBlock;
	lowPassProcessSpec.numChannels = getTotalNumOutputChannels();
	lowPassFilterL.prepare(lowPassProcessSpec);
	lowPassFilterR.prepare(lowPassProcessSpec);
	spec.sampleRate = sampleRate;
	//hpf
	highPassProcessSpec.sampleRate = sampleRate;
	highPassProcessSpec.maximumBlockSize = samplesPerBlock;
	highPassProcessSpec.numChannels = getTotalNumOutputChannels();
	highPassFilterL.prepare(highPassProcessSpec);
	highPassFilterR.prepare(highPassProcessSpec);
	spec.sampleRate = sampleRate;
	
	//lowband
	lowBandProcessSpec.sampleRate = sampleRate;
	lowBandProcessSpec.maximumBlockSize = samplesPerBlock;
	lowBandProcessSpec.numChannels = getTotalNumOutputChannels();
	lowBandEqL.prepare(lowBandProcessSpec);
	lowBandEqR.prepare(lowBandProcessSpec);
	spec.sampleRate = sampleRate;
	//midband
	midBandProcessSpec.sampleRate = sampleRate;
	midBandProcessSpec.maximumBlockSize = samplesPerBlock;
	midBandProcessSpec.numChannels = getTotalNumOutputChannels();
	midBandEqL.prepare(midBandProcessSpec);
	midBandEqR.prepare(midBandProcessSpec);
	spec.sampleRate = sampleRate;
	//highband
	highBandProcessSpec.sampleRate = sampleRate;
	highBandProcessSpec.maximumBlockSize = samplesPerBlock;
	highBandProcessSpec.numChannels = getTotalNumOutputChannels();
	highBandEqL.prepare(highBandProcessSpec);
	highBandEqR.prepare(highBandProcessSpec);
	spec.sampleRate = sampleRate;
	//verb
	cavern.setSampleRate(sampleRate);
	//comressor
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = getTotalNumOutputChannels();
	compressor.prepare(spec);
		

}

void NewProjectAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
	juce::ignoreUnused (layouts);
	return true;
  #else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
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
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels  = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.

	//******************************VERB***********************************
	
	//Get current values
	const float lastDryWet = dryWetParameter->get();
	const float lastRoomSize = roomSizeParameter->get();
	const float lastDamping = dampingParameter->get();
	const float lastWidth = roomWidthParameter->get();
	

	//*******comp******************
	


   
	
	


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear (i, 0, buffer.getNumSamples());


	const auto numChannels = juce::jmin(totalNumInputChannels, totalNumOutputChannels);

	compressor.setThreshold(threshParameter->get());
	compressor.setRatio(ratioParameter->get());
	compressor.setAttack(attackParameter->get());
	compressor.setRelease(releaseParameter->get());
	
	
	
	//Update Parameters, Merge Dry/Wet Levels
	cavernParameters.dryLevel = 1 - lastDryWet;
	cavernParameters.wetLevel = lastDryWet;
	cavernParameters.roomSize = lastRoomSize;
	cavernParameters.damping = lastDamping;
	cavernParameters.width = lastWidth;



	//Set Parameters
	cavern.setParameters(cavernParameters);

	




	//******************************VERB***********************************
	
	lowPassFilterL.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, *lowPassParameter);
	lowPassFilterR.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, *lowPassParameter);
	
	highPassFilterL.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, *highPassParameter);
	highPassFilterR.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, *highPassParameter);
	
	float lowGain = Adin::Conversions::DBtoLinear(*lowGainParameter);
	float midGain = Adin::Conversions::DBtoLinear(*midGainParameter);
	float highGain = Adin::Conversions::DBtoLinear(*highGainParameter);
	
	lowBandEqL.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *lowBandParameter, 2.00f, lowGain);
	lowBandEqR.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *lowBandParameter, 2.00f, lowGain);
	
	midBandEqL.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *midBandParameter, 2.00f, midGain);
	midBandEqR.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *midBandParameter, 2.00f, midGain);
	
	highBandEqL.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *highBandParameter, 2.00f, highGain);
	highBandEqR.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(spec.sampleRate, *highBandParameter, 2.00f, highGain);
	
	float preGain = Adin::Conversions::DBtoLinear(*preGainParameter);
	float postGain = Adin::Conversions::DBtoLinear(*postGainParameter);
	

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.

	for(int i=0; i<buffer.getNumChannels(); i++) {
		for(int j=0; j<buffer.getNumSamples(); j++) {
			float sample = buffer.getSample(i, j);
			sample = (sample * preGain);

			if(i==0) buffer.setSample(i, j, lowPassFilterL.processSample(sample));
			else buffer.setSample(i, j, lowPassFilterR.processSample(sample));

			sample = buffer.getSample(i, j);

			if(i==0) buffer.setSample(i, j, highPassFilterL.processSample(sample));
			else buffer.setSample(i, j, highPassFilterR.processSample(sample));

			sample = buffer.getSample(i, j);

			if(i==0) buffer.setSample(i, j, lowBandEqL.processSample(sample));
			else buffer.setSample(i, j, lowBandEqR.processSample(sample));

			sample = buffer.getSample(i, j);

			if(i==0) buffer.setSample(i, j, midBandEqL.processSample(sample));
			else buffer.setSample(i, j, midBandEqR.processSample(sample));

			sample = buffer.getSample(i, j);

			if(i==0) buffer.setSample(i, j, highBandEqL.processSample(sample));
			else buffer.setSample(i, j, highBandEqR.processSample(sample));

			sample = buffer.getSample(i, j);

			sample = (sample * postGain);
			buffer.setSample(i, j, sample);
		}
	}


	juce::dsp::AudioBlock<float> block{ buffer };
	compressor.process(juce::dsp::ProcessContextReplacing<float>(block));


	if (numChannels == 1)
		//Compressor processing

		//Reverb processing
		cavern.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
	else if (numChannels == 2)
		cavern.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
	
	
	
	
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
	return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new NewProjectAudioProcessor();
}
	






