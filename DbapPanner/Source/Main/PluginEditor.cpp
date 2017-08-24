/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DbapPannerAudioProcessorEditor::DbapPannerAudioProcessorEditor (DbapPannerAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState(vts), processor (p)
{
    for (int i = 0; i < processor.getNumParameters(); i++)
    {
        Slider* aSlider;
        gainSliders.add (aSlider = new Slider);
        addAndMakeVisible(aSlider);
        aSlider->setSliderStyle(Slider::LinearBarVertical);
        
        SliderAttachment* aSliderAttachment;
        gainAttachments.add ( aSliderAttachment = new SliderAttachment (valueTreeState, IDgain.toString()+(String)i, *aSlider));
    }
    
    //test = new SliderAttachment (valueTreeState, IDgain.toString()+"0", *gainSliders[0]);
    
    addAndMakeVisible(display);
    display.setFont(Font(20.0));
    display.setMultiLine(true);
    
    setSize (500, 400);
    
    startTimer(1000);
}

DbapPannerAudioProcessorEditor::~DbapPannerAudioProcessorEditor()
{
}

//==============================================================================
void DbapPannerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::darkgrey.interpolatedWith(Colours::mediumpurple, 0.5));
}

void DbapPannerAudioProcessorEditor::resized()
{
    Rectangle<int> r = getLocalBounds();
    Rectangle<int> sliderArea = r.removeFromTop(r.getHeight()*0.75);
    float sliderWidth =  sliderArea.getWidth() / (float)gainSliders.size();
    
    for (int slider = 0; slider < gainSliders.size(); slider++)
    {
        gainSliders[slider]->setBounds(sliderArea.removeFromLeft(sliderWidth));
    }
    display.setBounds(r);
}

void DbapPannerAudioProcessorEditor::timerCallback ()
{
    display.clear();
    String displayText = "Inputs: " + (String)processor.getNumInputChannels() + "\nOutputs: " + (String)processor.getNumOutputChannels() + "\nCh1 Gain: " + (String)processor.getChannel1Gain();
    display.setText(displayText);
}