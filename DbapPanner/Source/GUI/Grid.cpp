/*
  ==============================================================================

    Grid.cpp
    Created: 19 Sep 2017 4:00:48pm
    Author:  Alex Jones

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Grid.h"

//==============================================================================
Grid::Grid()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Grid::~Grid()
{
}

void Grid::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Grid", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Grid::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
