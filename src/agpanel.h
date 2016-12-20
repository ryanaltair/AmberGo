#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
/**
 use for AmberGo
 as panel
 
 **/

class agpanel{
public:
    agpanel();
    void setup();
    void update();
    ofxDatGui* gui;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    ofxDatGuiSlider* sliceHeightSlider;
    ofxDatGuiSlider* sliceProgressPercentSlider;
    ofxDatGuiButton* printStartButton;
    float sliceHeight=0;
    void sliceHeightBind(float sliceheight);
    
    bool isTimeToNextLayer=false;
    int workState=0;
    
    const int stateModelPrepare=1;
    const int stateModelReady=2;
    const int statePrintReady=3;
    const int statePrintPreparing=4;
    const int statePrinting=5;
    const int statePrintFinish=6;
    const int statePrepare=7;
    const int stateSleep=0;
};
