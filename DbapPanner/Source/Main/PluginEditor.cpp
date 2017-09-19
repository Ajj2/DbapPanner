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
        String propertyName = processor.getParameterName(i);
        
        if (propertyName.contains(IDSourcePosition))
        {
            Slider* aSlider;
            sourcePosSliders.add (aSlider = new Slider);
            addAndMakeVisible(aSlider);
            aSlider->setSliderStyle(Slider::LinearVertical);
            aSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
            
            Label* aLabel;
            sourcePosLabels.add (aLabel = new Label (propertyName));
            addAndMakeVisible(aLabel);
            aLabel->setText(propertyName, dontSendNotification);
            aLabel->setJustificationType(Justification::centred);
            
            SliderAttachment* aSliderAttachment;
            sourcePosAttachments.add ( aSliderAttachment = new SliderAttachment (valueTreeState, propertyName, *aSlider));
        }
        else if (propertyName.contains(IDRolloff))
        {
            rolloffSlider = new Slider;
            addAndMakeVisible (rolloffSlider);
            
            rolloffSlider->setSliderStyle(Slider::LinearVertical);
            rolloffSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
            
            rolloffLabel = new Label (propertyName);
            addAndMakeVisible(rolloffLabel);
            rolloffLabel->setText(propertyName, dontSendNotification);
            rolloffLabel->setJustificationType(Justification::centred);
        
            rolloffSliderAttachment = new SliderAttachment (valueTreeState, propertyName, *rolloffSlider);
        }
    }
    
    addAndMakeVisible(display);
    display.setFont(Font(20.0));
    display.setMultiLine(true);
    
    tabbedComponent = new TabbedComponent(TabbedButtonBar::Orientation::TabsAtBottom);
    addAndMakeVisible(tabbedComponent);
    
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
    
    Rectangle<int> sourcePosSliderArea = r.removeFromTop(r.getHeight()*0.75);
    Rectangle<int> sourcePosLabelArea = sourcePosSliderArea.removeFromBottom(sourcePosSliderArea.getHeight()*0.2);
    
    Rectangle<int> rolloffSliderArea = sourcePosSliderArea.removeFromRight(sourcePosSliderArea.getWidth()*0.2);
    Rectangle<int> rolloffLabelArea  = sourcePosLabelArea.removeFromRight(sourcePosSliderArea.getWidth()*0.2);
    
    float sliderWidth =  sourcePosSliderArea.getWidth() / (float)sourcePosSliders.size();
    
    for (int slider = 0; slider < sourcePosSliders.size(); slider++)
    {
        sourcePosSliders[slider]->setBounds(sourcePosSliderArea.removeFromLeft(sliderWidth));
        sourcePosLabels[slider]->setBounds(sourcePosLabelArea.removeFromLeft(sliderWidth));
    }
    
    rolloffSlider->setBounds(rolloffSliderArea);
    rolloffLabel->setBounds(rolloffLabelArea);
    
    display.setBounds(r);
}

void DbapPannerAudioProcessorEditor::timerCallback ()
{
    display.clear();
    String displayText = "Inputs: " + (String)processor.getNumInputChannels() + "\nOutputs: " + (String)processor.getNumOutputChannels() + "\nCh1 Gain: " + (String)processor.getChannel1Gain();
    display.setText(displayText);
}