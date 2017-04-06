#include "agpanel.h"

agpanel::agpanel(){
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    FrameRateMonitor  = gui->addFRM();
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 139.3,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    sliceProgressPercentSlider->setVisible(false);
        moreOptionToggle=gui->addToggle("TRY drag model in");
   
    allSliceButton=gui->addButton("load stl file");// when setup up it should be load stl file
    showAllSliceButton=gui->addButton("preview and save as ");
    printPauseButton=gui->addButton("pause");
    outputToggle=gui->addToggle("output");
    outputToggle->setChecked(true);
    
    showsliceToggle=gui->addToggle("slice Preview");
    showsliceToggle->setChecked(true);
    printSettingFolder=gui->addFolder("print setting", ofColor::white);
    layerthicknessSlider=printSettingFolder->addSlider("layer thickness", 0.02, 0.50, 0.16);
    exposedTimeSlider=printSettingFolder->addSlider("exposed time:second", 0, 20,10);
    baseExposedTimeSlider=printSettingFolder->addSlider("baseExposed time:second", 20, 120,80);
    
    scaleSettingFolder = gui->addFolder("scale setting", ofColor::white);
    scaleFactorSlider=scaleSettingFolder->addSlider("scale:%",0.01,2,1);
    scaleXSlider=scaleSettingFolder->addSlider("X: mm",0.01,2,1);
    scaleYSlider=scaleSettingFolder->addSlider("Y: mm",0.01,2,1);
    scaleZSlider=scaleSettingFolder->addSlider("Z: mm",0.01,2,1);
    
    positionSettingFolder = gui->addFolder("Position Setting", ofColor::white);
    
    positionXYPad=positionSettingFolder->add2dPad("XY");
    ofRectangle rect;
    rect.setPosition(-50, -25);
    rect.setSize(100,50);
    positionXYPad->setBounds( rect  );
    positionXSlider=positionSettingFolder->addSlider("X: mm",-20,20,0);
    positionYSlider=positionSettingFolder->addSlider("Y: mm",-20,20,0);
    positionZSlider=positionSettingFolder->addSlider("Z: mm",-100,100,0);

    moreOptionToggle->setChecked(false);
    
    gui->setTheme(new ofxDatGuiThemeNewSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    layerthicknessSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    exposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    baseExposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    scaleFactorSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    scaleXSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    scaleYSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    scaleZSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    positionXSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    positionYSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    positionZSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    printPauseButton->onButtonEvent(this,&agpanel::onButtonEvent);
    allSliceButton->onButtonEvent(this,&agpanel::onButtonEvent);
    showAllSliceButton->onButtonEvent(this,&agpanel::onButtonEvent);
    outputToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    showsliceToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    moreOptionToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    positionXYPad->on2dPadEvent(this, &agpanel::on2dPadEvent);
    
    
    moreOptionToggleJob(false);
     sliceProgressBar= ofxProgressBar(10,10,500,20,&sliceProgress,2000);
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
void agpanel::drawProgressBar(){
    sliceProgressBar.draw();


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
void agpanel::on2dPadEvent(ofxDatGui2dPadEvent e){
    cout << "point coordinates have changed to: x=" << e.x << " & y=" << e.y << endl;
    modelPositionOffset.x=e.x;
    modelPositionOffset.y=-e.y;
    
}
void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if(e.target==sliceHeightSlider){
        isLayerTestZChange=true;
    }
    if(e.target==scaleFactorSlider){
        isModelChanged=true;
        scaleFactor=ofVec3f(scaleFactorSize);
        updateModelSize();
    }
    
    if(e.target==layerthicknessSlider){
        isLayerThicknessChange=true;
    }
    if(e.target==scaleXSlider){
        double newfactor=scaleXSlider->getValue()/modelScale.x;
        scaleFactor.x=newfactor;
        isModelChanged=true;
    }
    if(e.target==scaleYSlider){
        double newfactor=scaleYSlider->getValue()/modelScale.y;
        scaleFactor.y=newfactor;
        isModelChanged=true;
        
    }
    if(e.target==scaleZSlider){
        double newfactor=scaleZSlider->getValue()/modelScale.z;
        scaleFactor.z=newfactor;
        isModelChanged=true;
    }
    if(e.target==positionXSlider){
        isModelChanged=true;
    }
    if(e.target==positionYSlider){
        isModelChanged=true;
    }
    if(e.target==positionZSlider){
        isModelChanged=true;
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
        if(isOutput){
            showAllSliceButton->setLabel("preview and save as ");
        
        }else{
            showAllSliceButton->setLabel("preview");
        }
    }
    if(e.target==showsliceToggle){
        ShowSlice=outputToggle->getChecked();
        
    }
    if(e.target==moreOptionToggle){
        moreOptionToggleJob(moreOptionToggle->getChecked());
    }
}
void agpanel::moreOptionToggleJob(bool checked){

    if(checked){
        FrameRateMonitor->setVisible(true);
        sliceHeightSlider->setVisible(true);
        printPauseButton->setVisible(true);
        scaleSettingFolder->setVisible(true);
        printSettingFolder->setVisible(true);
        positionSettingFolder->setVisible(true);
        showsliceToggle->setVisible(true);
        outputToggle->setVisible(true);
    }else{
        scaleSettingFolder->collapse();
        printSettingFolder->collapse();
        positionSettingFolder->collapse();
        FrameRateMonitor->setVisible(false);
        sliceHeightSlider->setVisible(false);
        printPauseButton->setVisible(false);
        scaleSettingFolder->setVisible(false);
        printSettingFolder->setVisible(false);
        positionSettingFolder->setVisible(false);
        showsliceToggle->setVisible(false);
        outputToggle->setVisible(false);
    }
}
void agpanel::sliderBind(){
    
    sliceHeightSlider->bind(layertestZ);
    layerthicknessSlider->bind(layerthickness);
    exposedTimeSlider->bind(exposedTime);
    baseExposedTimeSlider->bind(baseExposedTime);
    scaleFactorSlider->bind(scaleFactorSize);
    scaleFactorSize=1;
    scaleFactor=ofVec3f(scaleFactorSize);
    positionXSlider->bind(modelPositionOffset.x);
    positionYSlider->bind(modelPositionOffset.y);
    positionZSlider->bind(modelPositionOffset.z);
    
}

float agpanel::getWidth(){
    float f=gui->getWidth();
    return f;
    
}
void agpanel::setSliceReady(){
    moreOptionToggle->setLabel("slice is ready");
    
}
void agpanel::setSliceUnready(){
    moreOptionToggle->setLabel("slice is not ready");
    
}
void agpanel::setSliceDone(){
    moreOptionToggle->setLabel("slice is finish");
}
void agpanel::setSliceShowing(){
    moreOptionToggle->setLabel("now preview");
}
void agpanel::setSliceOutputDone(){
    moreOptionToggle->setLabel("output done now");
}
void agpanel::setSlicing(){
    moreOptionToggle->setLabel("slicing,please wait");
}
