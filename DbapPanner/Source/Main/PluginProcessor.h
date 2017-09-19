/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Identifiers.h"
#include "../Dbap.h"
#include <armadillo>

//==============================================================================
/**
*/
class DbapPannerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DbapPannerAudioProcessor();
    ~DbapPannerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    int getNumInputChannels() { return numInputChannels.get(); }
    int getNumOutputChannels() { return numOutputChannels.get(); }
    float getChannel1Gain() { return channel1Gain.get(); }
    
private:

    const static int numChannels = 2;
    AudioProcessorValueTreeState parameters;
    UndoManager undoManager;
    float prevGain[numChannels];
    
    ScopedPointer<Dbap> dbap;
    
    Atomic<int> numInputChannels;
    Atomic<int> numOutputChannels;
    Atomic<float> channel1Gain;
    
    arma::mat myMat;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DbapPannerAudioProcessor)
};
