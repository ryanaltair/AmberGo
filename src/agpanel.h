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
    bool isModelUpdated(){
        if(isModelChanged||isLayerThicknessChange){
            isModelChanged=false;
            isLayerThicknessChange=false;
            return true;
        }else{
            return false;
        }
    }
    void setOutputDone(bool done);
    void sliderBind();
    float getWidth();
    // set slice state
    void setSliceShowing();
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
    void setModelScale(ofVec3f orignScaleInMM){
        modelScale=orignScaleInMM;
        updateModelSize();
        if(orignScaleInMM.x>100){
        positionXSlider->setMax(orignScaleInMM.x);
        positionXSlider->setMin(-orignScaleInMM.x);
        }else{
            positionXSlider->setMax( 100);
            positionXSlider->setMin(-100);
        
        }
        if(orignScaleInMM.y>100){
        positionYSlider->setMax(orignScaleInMM.y);
        positionYSlider->setMin(-orignScaleInMM.y);
        }else{
            positionYSlider->setMax( 100);
            positionYSlider->setMin(-100);
        }
        if(orignScaleInMM.z>100){
        positionZSlider->setMax(orignScaleInMM.z);
        positionZSlider->setMin(-orignScaleInMM.z);
        }else{
            positionZSlider->setMax( 100);
            positionZSlider->setMin(-100);
            
        }
    }
  
    void updateModelSize(){
        ofVec3f realsize=modelScale*scaleFactor;
        // set the slider's range //
        scaleXSlider->setMin(0.01);
        scaleXSlider->setMax(realsize.x*2);
        scaleXSlider->setValue( realsize.x);
        
        scaleYSlider->setMin(0.01);
        scaleYSlider->setMax(realsize.y*2);
        scaleYSlider->setValue( realsize.y);
        
        scaleZSlider->setMin(0.01);
        scaleZSlider->setMax(realsize.z*2);
        scaleZSlider->setValue( realsize.z);
        
    }
    ofVec3f getPositionOffset(){
        return modelPositionOffset;
    }
    ofVec3f getScaleFactor(){
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
    ofxDatGuiFolder* printSettingFolder;
    ofxDatGuiSlider* layerthicknessSlider;
    ofxDatGuiSlider* exposedTimeSlider;
    ofxDatGuiSlider* baseExposedTimeSlider;
    
    ofxDatGuiSlider* sliceHeightSlider;
    
    
    ofxDatGuiFolder* scaleSettingFolder;//scale
    ofxDatGuiSlider* scaleFactorSlider;
    ofxDatGuiSlider* scaleXSlider;
    ofxDatGuiSlider* scaleYSlider;
    ofxDatGuiSlider* scaleZSlider;
    ofxDatGuiFolder* positionSettingFolder;// position
    ofxDatGuiSlider* positionXSlider;
    ofxDatGuiSlider* positionYSlider;
    ofxDatGuiSlider* positionZSlider;
    
    ofxDatGuiSlider* sliceProgressPercentSlider;
    ofxDatGuiButton* printPauseButton;
    ofxDatGuiButton* allSliceButton;
    ofxDatGuiButton* showAllSliceButton;
    ofxDatGuiLabel* sliceReadyLabel;
    ofxDatGuiToggle* outputToggle;
    ofxDatGuiToggle* showsliceToggle;
    
    
    ofVec3f modelScale;
    ofVec3f modelPositionOffset;
    float scaleFactorSize;
    ofVec3f scaleFactor;
    bool isSaveDirectoryChanged=false;
    string saveDirectory;
    float layertestZ=0.1;
    bool isLayerTestZChange=false;
    bool isModelChanged=false;
    bool isLayerThicknessChange=false;
    bool needAllSlice=false;
    
    
};
