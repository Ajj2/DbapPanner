/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../Identifiers.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class DbapPannerAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Timer
{
public:
    DbapPannerAudioProcessorEditor (DbapPannerAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~DbapPannerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void timerCallback () override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioProcessorValueTreeState& valueTreeState;
    DbapPannerAudioProcessor& processor;
    
    OwnedArray<SliderAttachment> sourcePosAttachments;
    OwnedArray<Slider> sourcePosSliders;
    OwnedArray<Label> sourcePosLabels;
    
    TextEditor display;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DbapPannerAudioProcessorEditor)
};
