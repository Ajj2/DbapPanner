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
    
    String nameX = IDSourcePosition.toString()+IDX;
    parameters.createAndAddParameter(nameX, nameX, nameX, NormalisableRange<float>(-1.0, 1.0), 0.1, nullptr, nullptr);
    String nameY = IDSourcePosition.toString()+IDY;
    parameters.createAndAddParameter(nameY, nameY, nameY, NormalisableRange<float>(-1.0, 1.0), 0.2, nullptr, nullptr);
    String nameZ = IDSourcePosition.toString()+IDZ;
    parameters.createAndAddParameter(nameZ, nameZ, nameZ, NormalisableRange<float>(-1.0, 1.0), 0.3, nullptr, nullptr);
    
    ValueTree SpeakerPositions = ValueTree (IDSpeakerPositions);
    
    arma::mat speakerPosMat = { {-1, 0, 1},
                                {1, 0, 1},};
    
    for (int ch = 0; ch < numChannels; ch++)
    {
        // Create a separate valueTree to store speaker positions in
        ValueTree SpeakerPosition = ValueTree (IDSpeakerPosition);
        SpeakerPosition.setProperty(IDIndex, ch, nullptr);
        SpeakerPosition.setProperty(IDX, speakerPosMat(ch, 0), nullptr);
        SpeakerPosition.setProperty(IDY, speakerPosMat(ch, 1), nullptr);
        SpeakerPosition.setProperty(IDZ, speakerPosMat(ch, 2), nullptr);
        
        String nameX = IDSpeakerPosition.toString()+(String)ch+IDX;
        String nameY = IDSpeakerPosition.toString()+(String)ch+IDY;
        String nameZ = IDSpeakerPosition.toString()+(String)ch+IDZ;
        
        parameters.createAndAddParameter(nameX, nameX, nameX, NormalisableRange<float>(-1.0, 1.0), 0, nullptr, nullptr);
        parameters.createAndAddParameter(nameY, nameY, nameY, NormalisableRange<float>(-1.0, 1.0), 0, nullptr, nullptr);
        parameters.createAndAddParameter(nameZ, nameZ, nameZ, NormalisableRange<float>(-1.0, 1.0), 0, nullptr, nullptr);
        
        SpeakerPositions.getPropertyAsValue(IDX, &undoManager).referTo(parameters.getParameterAsValue(nameX));
        SpeakerPositions.getPropertyAsValue(IDY, &undoManager).referTo(parameters.getParameterAsValue(nameY));
        SpeakerPositions.getPropertyAsValue(IDZ, &undoManager).referTo(parameters.getParameterAsValue(nameZ));
        
        SpeakerPositions.addChild(SpeakerPosition, -1, &undoManager);
    }
    String rolloffName = IDRolloff.toString();
    parameters.createAndAddParameter(rolloffName, rolloffName, rolloffName, NormalisableRange<float> (0, 24), 6, nullptr, nullptr);
    
    parameters.state = ValueTree (IDparameterState);
    
    parameters.state.addChild(SpeakerPositions, -1, &undoManager);
            
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
    ValueTree sourcePosTree = ValueTree ("sourcePosTree");
    
    arma::mat SS (3, 1);
    
    SS (0, 0) = *parameters.getRawParameterValue(IDSourcePosition.toString()+IDX);
    SS (1, 0) = *parameters.getRawParameterValue(IDSourcePosition.toString()+IDY);
    SS (2, 0) = *parameters.getRawParameterValue(IDSourcePosition.toString()+IDZ);
    
    float rolloff = *parameters.getRawParameterValue(IDRolloff);
    dbap->getChannelGains (parameters.state.getChildWithName(IDSpeakerPositions), SS, rolloff, channelGains);
    
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
