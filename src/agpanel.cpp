#include "agpanel.h"

agpanel::agpanel(){
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 139.3,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    sliceProgressPercentSlider->setVisible(false);
    sliceReadyLabel=gui->addLabel("slice is ready");
    sliceReadyLabel->setLabel(sliceUnready);
    allSliceButton=gui->addButton("all slice!!");
    showAllSliceButton=gui->addButton("show all");
    printPauseButton=gui->addButton("Pause");
    outputToggle=gui->addToggle("output");
    outputToggle->setChecked(false);
    showsliceToggle=gui->addToggle("slice Preview");
    showsliceToggle->setChecked(true);
    layerthicknessSlider=gui->addSlider("layer thickness", 0.02, 0.50, 0.16);
    exposedTimeSlider=gui->addSlider("exposed time:second", 0, 12,3);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    layerthicknessSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    exposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
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
    sliceHeightBind();
}
void agpanel::update(){
    
}

void agpanel::outputDone(bool done){
    outputdone=done;
}
void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    
    if(e.target==exposedTimeSlider){
        float i=e.value;
        exposedTimeSlider->setValue(i);
        exposedTime=e.value;
    }
    
}
void agpanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    
    if(e.target==printPauseButton){
        
        if(isOutput==true){
            bPrint=false;
        }
        if(bShowAllSlice!=false){
            bShowAllSlice=false;
        }else{
            bShowAllSlice=true;
        }
        
    }
    
    if(e.target==allSliceButton){
        bAllSlice=true;
    }
    if(e.target==showAllSliceButton){
        
        bShowAllSlice=true;
        iShowAllSliceLayerCount=0;
    }
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
void agpanel::sliceHeightBind(){
    
    sliceHeightSlider->bind(layertestZ);
    layerthicknessSlider->bind(layerthickness);
    
    
}

float agpanel::getWidth(){
    float f=gui->getWidth();
    return f;
    
}
void agpanel::setSliceReady(){
    sliceReadyLabel->setLabel(sliceReady);
    
}
void agpanel::setSliceUnready(){
    sliceReadyLabel->setLabel(sliceUnready);
    
}
