#include "agpanel.h"

agpanel::agpanel(){
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 139.3,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    sliceProgressPercentSlider->setVisible(false);
    sliceReadyLabel=gui->addLabel("slice is ready");
    sliceReadyLabel->setLabel("ready to add model");
    allSliceButton=gui->addButton("all slice!!");
    showAllSliceButton=gui->addButton("show all");
    printPauseButton=gui->addButton("Pause");
    outputToggle=gui->addToggle("output");
    outputToggle->setChecked(false);
    showsliceToggle=gui->addToggle("slice Preview");
    showsliceToggle->setChecked(true);
    layerthicknessSlider=gui->addSlider("layer thickness", 0.02, 0.50, 0.16);
    exposedTimeSlider=gui->addSlider("exposed time:second", 0, 20,10);
    baseExposedTimeSlider=gui->addSlider("baseExposed time:second", 20, 120,80);
    scaleSlider=gui->addSlider("scale:",0.01,2,1);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    layerthicknessSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    exposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    baseExposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    scaleSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    printPauseButton->onButtonEvent(this,&agpanel::onButtonEvent);
    allSliceButton->onButtonEvent(this,&agpanel::onButtonEvent);
    showAllSliceButton->onButtonEvent(this,&agpanel::onButtonEvent);
    outputToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    showsliceToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    
}


void agpanel::setup(){
    
    // GUI start
    // instantiate and position the gui //
    exposedTime=exposedTimeSlider->getValue();
    
    baseExposedTime=baseExposedTimeSlider->getValue();
    sliderBind();
}
void agpanel::update(){
    
}
bool agpanel::isSliceHeightUpdated(){
    if(isLayerTestZChange){
        isLayerTestZChange=false;
        return true;
    }else{
        return false;
    }
 
}
void agpanel::setOutputDone(bool done){
    outputDone=done;
}
void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if(e.target==sliceHeightSlider){
        isLayerTestZChange=true;
    }
    if(e.target==scaleSlider){
        isModelChanged=true;
    }
    
    if(e.target==layerthicknessSlider){
        isLayerThicknessChange=true;
    }
}
void agpanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    
    if(e.target==printPauseButton){
        
        if(isOutput==true){
            bPrint=false;
        }
        if(ShowingAllSlice!=false){
            ShowingAllSlice=false;
        }else{
            ShowingAllSlice=true;
        }
        
    }
    
    if(e.target==allSliceButton){
        needAllSlice=true;
    }
    if(e.target==showAllSliceButton){
        if(outputToggle->getChecked()==true){
            ofFileDialogResult result = ofSystemLoadDialog("fabfiles to Save", true);
            if(result.bSuccess) {
                saveDirectory = result.getPath();
                saveDirectory+="/";
                cout<<"we will save file at"<<saveDirectory<<endl;
                // save your file to `path`
                isSaveDirectoryChanged=true;
                ShowingAllSlice=true;
                iShowAllSliceLayerCount=0;
            }
        }else{
            ShowingAllSlice=true;
            iShowAllSliceLayerCount=0;
            
        }
        
    }
}
bool agpanel::getSaveDirectoryChanged(){
    return isSaveDirectoryChanged;
}
string agpanel::getSaveDirectory(){
    isSaveDirectoryChanged=false;
    return saveDirectory;
}
void agpanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    
    
}
void agpanel::onToggleEvent(ofxDatGuiToggleEvent e)
{
    if(e.target==outputToggle){
        isOutput=outputToggle->getChecked();
        
    }
    if(e.target==showsliceToggle){
        ShowSlice=outputToggle->getChecked();
        
    }
}
void agpanel::sliderBind(){
    
    sliceHeightSlider->bind(layertestZ);
    layerthicknessSlider->bind(layerthickness);
    exposedTimeSlider->bind(exposedTime);
    baseExposedTimeSlider->bind(baseExposedTime);
    scaleSlider->bind(scaleFactor);
    scaleFactor=1;
}

float agpanel::getWidth(){
    float f=gui->getWidth();
    return f;
    
}
void agpanel::setSliceReady(){
    sliceReadyLabel->setLabel("slice is ready");
    
}
void agpanel::setSliceUnready(){
    sliceReadyLabel->setLabel("slice is not ready");
    
}
void agpanel::setSliceDone(){
    sliceReadyLabel->setLabel("ready to show");
}
void agpanel::setSliceShowing(){
 sliceReadyLabel->setLabel("now showing");
}
void agpanel::setSliceOutputDone(){
    sliceReadyLabel->setLabel("output done now");
}
void agpanel::setSlicing(){
 sliceReadyLabel->setLabel("slicing,please wait");
}
