/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DbapPannerAudioProcessor::DbapPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("InputMono",  AudioChannelSet::mono(), true)
                       .withInput  ("InputStereo",  AudioChannelSet::stereo(), true)
                       .withInput  ("InputQuad", AudioChannelSet::quadraphonic(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::discreteChannels(numChannels), true)
                     #endif
                       ), parameters(*this, &undoManager), undoManager(30000, 30),
                          myMat (numChannels, 3)
#endif
{
    for (int ch = 0; ch < numChannels; ch++)
    {
        String i = (String)ch;
        parameters.createAndAddParameter(IDgain.toString()+i, IDgain.toString()+(String)i,String(), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr);
    }
    
    parameters.state = ValueTree (IDparameterState);
}

DbapPannerAudioProcessor::~DbapPannerAudioProcessor()
{
}

//==============================================================================
const String DbapPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DbapPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DbapPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double DbapPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DbapPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DbapPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DbapPannerAudioProcessor::setCurrentProgram (int index)
{
}

const String DbapPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void DbapPannerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DbapPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int ch = 0 ; ch < numChannels; ch++)
    {
        prevGain[ch] = *parameters.getRawParameterValue(IDgain+(String)ch);
    }
}

void DbapPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DbapPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    if(layouts.getMainInputChannelSet()==AudioChannelSet::mono())
    {
        if (!layouts.getMainInputChannelSet().isDisabled())
        {
            if (!layouts.getMainOutputChannelSet().isDisabled())
            {
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(16))
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    if (layouts.getMainInputChannelSet()==AudioChannelSet::stereo())
    {
        if (!layouts.getMainInputChannelSet().isDisabled())
        {
            if (!layouts.getMainOutputChannelSet().isDisabled())
            {
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(16))
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    if (layouts.getMainInputChannelSet()==AudioChannelSet::quadraphonic())
    {
        if (!layouts.getMainInputChannelSet().isDisabled())
        {
            if (!layouts.getMainOutputChannelSet().isDisabled())
            {
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(16))
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    
    return false;
#endif
}
#endif

void DbapPannerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    for (int outputCh = getBusesLayout().getMainOutputChannels()-1; outputCh > 0; outputCh--)
    {
        buffer.copyFrom(outputCh, 0, buffer, 0, 0, buffer.getNumSamples());
        
        float currentGain = *parameters.getRawParameterValue(IDgain+(String)outputCh);
        
        if (currentGain == prevGain[outputCh])
        {
            buffer.applyGain(currentGain);
        }
        else
        {
            buffer.applyGainRamp(0, buffer.getNumSamples(), prevGain[outputCh], currentGain);
            prevGain[outputCh] = currentGain;
        }
    }
    buffer.applyGain(0, 0, buffer.getNumSamples(), *parameters.getRawParameterValue(IDgain+"0"));
    
    numInputChannels = getBusesLayout().getMainInputChannels();
    numOutputChannels = getBusesLayout().getMainOutputChannels();
}

//==============================================================================
bool DbapPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DbapPannerAudioProcessor::createEditor()
{
    return new DbapPannerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void DbapPannerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void DbapPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DbapPannerAudioProcessor();
}
