#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
/**
 use for AmberGo
 as panel 
 and output work
 **/

class agpanel{
public:
    agpanel();
    void setup();
    void update();
    ofxDatGui* gui;
    //events
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    bool isSliceHeightChange();
    //componment
    ofxDatGuiSlider* sliceHeightSlider;
    ofxDatGuiSlider* sliceProgressPercentSlider;
    ofxDatGuiButton* printPauseButton;
    ofxDatGuiButton* allSliceButton;
    ofxDatGuiButton* showAllSliceButton;
     ofxDatGuiLabel* sliceReadyLabel;
    ofxDatGuiToggle* outputToggle;
    ofxDatGuiToggle* showsliceToggle;
    ofxDatGuiSlider* layerthicknessSlider;
    ofxDatGuiSlider* exposedTimeSlider;
     
    void setOutputDone(bool done);
    void sliderBind();
    float getWidth();
    void setSliceReady();
    void setSliceUnready();
    
    void setSliceDone();
    bool outputDone=false;
    float sliceMin=0.02;
    float sliceMax=0.30;
    int snapcount=1000;
    float layerthickness=0.02;
    string sliceReady="Slice is ready";
    string sliceUnready="Slice is not ready";
    string sliceDone="Slice is finish";
//    string slice
    bool isOutput=false;
    bool bPrint=false;
    bool bAllSlice=false;
    int exposedTime=1000;
    bool ShowSlice=true;
    bool bShowAllSlice=false;
    ofIndexType iShowAllSliceLayerCount=0;
    float layertestZlast=0.1;
    float layertestZ=0.1;
private:
    
};
