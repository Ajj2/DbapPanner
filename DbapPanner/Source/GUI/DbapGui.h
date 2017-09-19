/*
  ==============================================================================

    DbapGui.h
    Created: 19 Sep 2017 4:02:05pm
    Author:  Alex Jones

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DbapGui    : public Component
{
public:
    DbapGui();
    ~DbapGui();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DbapGui)
};
