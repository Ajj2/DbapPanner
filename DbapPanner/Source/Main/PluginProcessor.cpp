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
        parameters.createAndAddParameter(IDchannelGain.toString()+i, IDchannelGain.toString()+(String)i,String(), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr);
    }
    
    parameters.state = ValueTree (IDparameterState);
    
    ValueTree SpeakerPositions = ValueTree (IDSpeakerPositions);
    for (int ch = 0; ch < numChannels; ch++)
    {
        ValueTree SpeakerPosition = ValueTree (IDSpeakerPosition);
        SpeakerPosition.setProperty(IDIndex, ch, nullptr);
        SpeakerPosition.setProperty(IDX, 1+ch, nullptr);
        SpeakerPosition.setProperty(IDY, 2+ch, nullptr);
        SpeakerPosition.setProperty(IDZ, 3+ch, nullptr);
        
        SpeakerPositions.addChild(SpeakerPosition, -1, &undoManager);
    }
    parameters.state.addChild(SpeakerPositions, -1, &undoManager);
    
    ValueTree SourcePosition = ValueTree(IDSourcePosition);
    SourcePosition.setProperty(IDX, 1.5, nullptr);
    SourcePosition.setProperty(IDY, 2.5, nullptr);
    SourcePosition.setProperty(IDZ, 3.5, nullptr);
    
    parameters.state.addChild(SourcePosition, -1, &undoManager);
    
    dbap = new Dbap();
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
        prevGain[ch] = *parameters.getRawParameterValue(IDchannelGain+(String)ch);
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
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(numChannels))
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
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(numChannels))
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
                if (layouts.getMainOutputChannelSet()==AudioChannelSet::discreteChannels(numChannels))
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
    }
    
    arma::mat channelGains (numChannels, 1);
    dbap->getChannelGains (parameters.state.getChildWithName(IDSpeakerPositions), parameters.state.getChildWithName(IDSourcePosition), channelGains);
    
    static int counter = 0;
    if (counter++ % (int)100 == 0)
    {
        std::cout << "channelGains: \n" << channelGains << std::endl;
    }
    
    for (int ch = 0 ; ch < getBusesLayout().getMainOutputChannels(); ch++)
    {
        parameters.getParameterAsValue(IDchannelGain+(String)ch).setValue(channelGains(ch, 0));
        float currentGain = *parameters.getRawParameterValue(IDchannelGain+(String)ch);
        if (currentGain == prevGain[ch])
        {
            buffer.applyGain(ch, 0, buffer.getNumSamples(), currentGain);
        }
        else
        {
            buffer.applyGainRamp(ch, 0, buffer.getNumSamples(), prevGain[ch], currentGain);
            prevGain[ch] = currentGain;
        }
    }
    
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
