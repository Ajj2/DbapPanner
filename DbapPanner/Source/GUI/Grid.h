/*
  ==============================================================================

    Grid.h
    Created: 19 Sep 2017 4:00:48pm
    Author:  Alex Jones

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Grid    : public Component
{
public:
    Grid();
    ~Grid();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Grid)
};
