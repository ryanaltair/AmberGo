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

    bool isSliceHeightUpdated();
 
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
    float getScaleFactor(){
        return scaleFactor;
    }
    bool needOutput(){
        return outputToggle->getChecked();
    }
    void setSliceHeight(float z){
        isLayerTestZChange=true;
        layertestZ=z;
    }
    float getSliceHeight(){
        return layertestZ;
    }
    bool outputDone=false;
    float sliceMin=0.02;
    float sliceMax=0.30;
    float layerthickness=0.05;
   //    string slice
    bool isOutput=false;
    bool bPrint=false;
    int exposedTime=1000;//ms
    int baseExposedTime=60000;//ms
    bool ShowSlice=true;
    bool ShowingAllSlice=false;
    ofIndexType iShowAllSliceLayerCount=0;
    
protected:
    //events
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
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
    ofxDatGuiSlider* scaleSlider;
    
    ofxDatGuiSlider* baseExposedTimeSlider;
    
    float scaleFactor;
    bool isSaveDirectoryChanged=false;
    string saveDirectory;
    float layertestZ=0.1;
    bool isLayerTestZChange=false;
    bool needAllSlice=false;

    
};
