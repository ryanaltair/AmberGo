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
 
    void setOutputDone(bool done);
    void sliderBind();
    float getWidth();
    // set slice state
    void setSliceReady();
    void setSliceUnready();
    void setSliceDone();
    void setSliceOutputDone();
    void setSlicing();
    bool getSaveDirectoryChanged();
    string getSaveDirectory();
    bool needAllToSlice(){
        if(needAllSlice){
            needAllSlice=false;
            return true;
        }
        // else
        return false;
        
    }
    bool needOutput(){
        return outputToggle->getChecked();
    
    }
    bool outputDone=false;
    float sliceMin=0.02;
    float sliceMax=0.30;
    int snapcount=1000;
    float layerthickness=0.05;
   //    string slice
    bool isOutput=false;
    bool bPrint=false;
    int exposedTime=1000;//ms
    int baseExposedTime=60000;//ms
    bool ShowSlice=true;
    bool ShowingAllSlice=false;
    ofIndexType iShowAllSliceLayerCount=0;
    float layertestZlast=0.1;
    float layertestZ=0.1;
protected:
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
    
    ofxDatGuiSlider* baseExposedTimeSlider;
    bool isSaveDirectoryChanged=false;
    string saveDirectory;
    
    bool needAllSlice=false;

    
};
