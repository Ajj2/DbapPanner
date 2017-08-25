/*
  ==============================================================================

    Dbap.h
    Created: 25 Aug 2017 4:22:21pm
    Author:  Alex Jones

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Identifiers.h"
#include "Utility.h"
#include <armadillo>

class Dbap
{
public:
    Dbap();
    ~Dbap();
    
    bool valueTreeToMatrix (const ValueTree& vt, arma::mat& result)
    {
        if (vt.getNumChildren() > 0)
        {
            for (int sp = 0 ; sp < vt.getNumChildren(); sp++)
            {
                ValueTree Sp = vt.getChild(sp);
                
                result (0, sp) = Sp.getProperty(IDX);
                result (1, sp) = Sp.getProperty(IDY);
                result (2, sp) = Sp.getProperty(IDZ);
            }
        }
        else
        {
            result (0, 0) = vt.getProperty(IDX);
            result (1, 0) = vt.getProperty(IDY);
            result (2, 0) = vt.getProperty(IDZ);
        }
        return true;
    }
    
    bool getChannelGains (const ValueTree& speakerPositions, const ValueTree& sourcePosition, arma::mat& result);
    
private:
    float rolloff;
    float spatialBlur;
};