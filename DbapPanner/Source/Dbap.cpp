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
    rolloff = 18.0f / (20.0f * log10(2));;
    spatialBlur = 0.1;
}

Dbap::~Dbap()
{
    
}

bool Dbap::getChannelGains (const ValueTree& speakerPositions, const arma::mat& SS, arma::mat& result)
{
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
    
    static int counter = 0;
    
    if (counter++ % (int)100 == 0)
    {
        std::cout << "dis : \n" << dis << std::endl;
    }
    
    double twoA = 2.f * rolloff;
    double sum = 0;
    
    for (int i = 0; i < numSpeakersInUse; i++)
    {
        sum += (pow (spW(i, 0), 2.f)) / (pow (dis(i, 0) , twoA ));
    }
    
    k = 1.f / sqrt (sum);

    float sAmp[numSpeakersInUse];
    
    for ( int i = 0; i < numSpeakersInUse; i++)
    {
        sAmp[i] = k / ( pow (dis(i, 0), rolloff) );
        result(i, 0) = sAmp[i];
    }
    
    return true;

//    for (int i = 0; i < getSmoothedGains().size(); i++)
//    {
//        getSmoothedGains()[i]->setValue(sAmp[i]);
//    }
//
//    float hfRoll = getHFRolloff();
//    float cutoff = powf(localDistanceScalar, 3) * 20000.f;
//    
//    cutoff = (cutoff * hfRoll) + ((1.0 - hfRoll) * 20000.f);
//    
//    if (cutoff < 1)
//    {
//        cutoff = 1;
//    }
//    else if (cutoff > 20000)
//    {
//        cutoff = 20000;
//    }
//    
//    for (int i = 0; i< getSpeakerPositions3D().n_cols; i++)
//    {
//        *filterCoeffs[i] = IIRCoefficients::makeLowPass(sR, cutoff);
//        LPFFilters[i]->setCoefficients(*filterCoeffs[i]);
//        
//        float localVerbAmount = getVerbAmount();
//        
//        verbParams.roomSize = getVerbSize();
//        verbParams.damping = ((1.0f - localVerbAmount)*0.8) + 0.2;
//        verbParams.wetLevel = (1.0 - localDistanceScalar) * localVerbAmount;
//        verbParams.dryLevel = localDistanceScalar;
//        verbParams.width = 0.3;
//        verbParams.freezeMode = 0;
//        
//        verb1[i].setParameters(verbParams);
//    }
//    
//    for (int sP = 0; sP < numSpeakersInUse; sP++)
//    {
//        int numSamples = bufferToFill.numSamples;
//        float* outP = bufferToFill.buffer->getWritePointer(sP);
//        float* outPV = bufferToFill.buffer->getWritePointer(sP);
//        while (numSamples--)
//        {
//            *outP *= getSmoothedGains()[sP]->getNextValue() * getDistanceScalar();
//            *outP = LPFFilters[sP]->processSingleSampleRaw(*outP);
//            outP++;
//        }
//        verb1[sP].processMono(outPV, bufferToFill.numSamples);
//    }
//    
//    if (numAvailableChannels > numSpeakersInUse)
//    {
//        for (int ch = numSpeakersInUse; ch < numAvailableChannels; ch++)
//        {
//            bufferToFill.buffer->applyGain(ch, 0, bufferToFill.numSamples, 0);
//        }
//    }
}