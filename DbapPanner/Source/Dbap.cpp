/*
  ==============================================================================

    Dbap.cpp
    Created: 25 Aug 2017 4:22:21pm
    Author:  Alex Jones

  ==============================================================================
*/

#include "Dbap.h"


Dbap::Dbap()
{
    rollOff = 6.0f / (20.0f * log10(2));
    spatialBlur = 0.1;
}

Dbap::~Dbap()
{
    
}

bool Dbap::getChannelGains (const ValueTree& speakerPositions, const arma::mat& SS, float newRollOff, arma::mat& result)
{
    if (newRollOff != rollOffRef)
    {
        rollOff = newRollOff / (20.0f * log10(2));
        rollOffRef = newRollOff;
    }
    
    int numSpeakersInUse = speakerPositions.getNumChildren();
    
    arma::mat Sp (3, numSpeakersInUse);
    valueTreeToMatrix(speakerPositions, Sp);
    
    float k;
    
    arma::mat dis (Sp.n_cols, 1);
    arma::mat spW (Sp.n_cols, 1);
    
    float rLocal = spatialBlur;
    for ( int i = 0; i < numSpeakersInUse; i++)
    {
        dis(i, 0) = sqrt(arma::accu(arma::pow(Sp.col(i) - SS, 2) + pow(rLocal, 2)));
        spW(i, 0) = 1.f;
    }
    
    double twoA = 2.f * rollOff;
    double sum = 0;
    
    for (int i = 0; i < numSpeakersInUse; i++)
    {
        sum += (pow (spW(i, 0), 2.f)) / (pow (dis(i, 0) , twoA ));
    }
    
    k = 1.f / sqrt (sum);

    float sAmp[numSpeakersInUse];
    
    for ( int i = 0; i < numSpeakersInUse; i++)
    {
        sAmp[i] = k / ( pow (dis(i, 0), rollOff) );
        result(i, 0) = sAmp[i];
    }
    
    return true;
}