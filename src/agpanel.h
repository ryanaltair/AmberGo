#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxProgressBar.h"
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
    void drawProgressBar();
    void setProgressBarMax(int max){
       sliceProgressBar.setMax( max);
    }
    void setProgress(int p){
        sliceProgress=p;
    }
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
    void show(){
        gui->setVisible(true);
    }
    void hide(){
        gui->setVisible(false);
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
        ofRectangle rect;
        rect.setPosition(-orignScaleInMM.x, -orignScaleInMM.y);
        rect.setSize(orignScaleInMM.x*2,orignScaleInMM.y*2);
        positionXYPad->setBounds( rect  );
        
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
    void fileLoaded(){
        allSliceButton->setLabel("slice");
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
    int iShowAllSliceLayerCount=0;
    
protected:
    void moreOptionToggleJob(bool checked);
    //events
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    //componment
    ofxDatGuiFRM* FrameRateMonitor;
    ofxDatGuiTextInput* stateTextBar;
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
    
    ofxDatGui2dPad* positionXYPad;
    ofxDatGuiSlider* positionXSlider;
    ofxDatGuiSlider* positionYSlider;
    ofxDatGuiSlider* positionZSlider;
    
    ofxDatGuiSlider* sliceProgressPercentSlider;
    ofxDatGuiButton* printPauseButton;
    ofxDatGuiButton* allSliceButton;
    ofxDatGuiButton* showAllSliceButton;
    ofxDatGuiToggle* outputToggle;
    ofxDatGuiToggle* showsliceToggle;
    ofxDatGuiToggle* moreOptionToggle;
    
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
    ofxProgressBar sliceProgressBar;
    int sliceProgress;
    
};
class ofxDatGuiThemeNewSmoke : public ofxDatGuiTheme{
    
public:
    
    ofxDatGuiThemeNewSmoke(){
        stripe.visible = false;
        color.label = hex(0xF8F3F0);
        color.guiBackground = hex(0x2C3137);
        color.background = hex(0x343B41);
        color.slider.fill = hex(0x60B9ED);
        color.slider.text = hex(0xFFFFFF);
        color.inputAreaBackground = hex(0x434A50);
        color.textInput.text = hex(0xFFF0F8);
        color.textInput.highlight = hex(0x434A50);
        color.textInput.backgroundOnActive = hex(0x2C3137);
        color.backgroundOnMouseOver = hex(0x434A50);
        color.backgroundOnMouseDown = hex(0x2C3137);
        color.matrix.hover.button = hex(0x60B9ED);
        color.matrix.selected.button = hex(0x2C3137);
        color.inputAreaBackground=hex(0x444B51);
        layout.upperCaseLabels=true;
        layout.textInput.forceUpperCase=true;
        init();
    }
};
