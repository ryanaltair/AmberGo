#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
/**
 use for AmberGo
 as panel
 and serial work 
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
 
    //componment
    ofxDatGuiSlider* sliceHeightSlider;
    ofxDatGuiSlider* sliceProgressPercentSlider;
    ofxDatGuiButton* printStartButton;
    ofxDatGuiButton* printStopButton;
    ofxDatGuiButton* connectButton;
    ofxDatGuiTextInput* sendMessageTextInput;
    ofxDatGuiLabel* sliceReadyLabel;
    ofxDatGuiToggle* outputToggle;
    ofxDatGuiToggle* showsliceToggle;
    ofxDatGuiSlider* layerthicknessSlider;
    ofxDatGuiSlider* exposedTimeSlider;
   // float sliceHeight=0.02;
    void timeUpdate();
    void outputDone(bool done);
    void sliceHeightBind();
    void tryConnect();
    void serialUpdate();
    bool isTimeToNextLayer=false;
    int workState=0;
    float getWidth();
    void setSliceReady();
    void setSliceUnready();
    const int stateModelPrepare=1;
    const int stateModelReady=2;
    const int statePrintReady=3;
    const int statePrintPreparing=4;
    const int statePrinting=5;
    const int statePrintFinish=6;
    const int statePrepare=7;
    const int stateSleep=0;
    float timerReset=0;
    float timerToNextLayer=0;
    
    bool outputdone=false;
    bool timerResetTriger =false ;
    bool timerToNextLayerTrigger= false ;
    float exposeTime=0.0001;
    ofSerial serial;
    string teststring;// used for serial test
    bool isConnect=false;
    float sliceMin=0.02;
    float sliceMax=1;
    int snapcount=1000;
    float layerthickness=0.02;
    string sliceReady="Slice is ready";
    string sliceUnready="Slice is not ready";
    
    bool isOutput=false;
    bool bPrint=false;
    
    int exposedTime=1000;
        bool ShowSlice=true;
    
    float layertestZlast=0.1;
    float layertestZ=0.1;

};
