#include "agpanel.h"

agpanel::agpanel(){
    
}


void agpanel::setup(){
    //initial
    isConnect=false;
    
    // GUI start
    // instantiate and position the gui //
    // gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    //gui->addBreak();
    //gui->addButton("Click!");
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 139.3,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    sliceProgressPercentSlider->setVisible(false);
    sliceReadyLabel=gui->addLabel("slice is ready");
    sliceReadyLabel->setLabel(sliceUnready);
    printStartButton=gui->addButton("start print");
    printStopButton=gui->addButton("stop print");
    outputToggle=gui->addToggle("output");
    outputToggle->setChecked(false);
    showsliceToggle=gui->addToggle("slice Preview");
    showsliceToggle->setChecked(true);
    layerthicknessSlider=gui->addSlider("layer thickness", 0.02, 3.30, 0.16);
    exposedTimeSlider=gui->addSlider("exposed time:second", 0, 12,3);
    connectButton=gui->addButton("amber is offline");
    connectButton->setVisible(false);
    sendMessageTextInput=gui->addTextInput("sendMsg:");
    sendMessageTextInput->setVisible(false);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    layerthicknessSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    exposedTimeSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    printStartButton->onButtonEvent(this, &agpanel::onButtonEvent);
    printStopButton->onButtonEvent(this,&agpanel::onButtonEvent);
    connectButton->onButtonEvent(this,&agpanel::onButtonEvent);
    sendMessageTextInput->onTextInputEvent(this,&agpanel::onTextInputEvent);
    outputToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    showsliceToggle->onToggleEvent(this,&agpanel::onToggleEvent);
    exposedTime=exposedTimeSlider->getValue();
    sliceHeightBind();
}
void agpanel::update(){
    serialUpdate();
    
    timeUpdate();
    
    if(workState==statePrintPreparing){
        cout<<"now we are print preparing"<<endl;
        layertestZ=0.02;//the first layer
        timerToNextLayer=ofGetElapsedTimef()+5;
        workState=statePrinting;
    }
    if(workState==statePrinting&&outputdone==true){
        
        
        if(isTimeToNextLayer==true){
            //    cout<<"now we are printing"<<endl;
            if(layertestZ>sliceMax-layerthickness){
                //layertestZ=-1;
                workState=statePrintFinish;
            }else{
                layertestZ+=layerthickness;
                // cout<<"we change slice height at "<< ofGetElapsedTimef()<<" seconds"<<endl;
            }
            // cout<<"tiemr before "<<timerToNextLayer;
            timerToNextLayer=ofGetElapsedTimef()+exposeTime;
            
            //  cout<<" tiemr after "<<timerToNextLayer<<endl;
            isTimeToNextLayer=false;
            outputdone=false;
            
        }
    }
}
void agpanel::timeUpdate(){
    if(timerReset<ofGetElapsedTimef()){
        timerResetTriger=true;
    }
    if(timerToNextLayer<ofGetElapsedTimef()){
        timerToNextLayerTrigger=true;
        isTimeToNextLayer=true;
    }
}
void agpanel::outputDone(bool done){
    outputdone=done;
}
void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if(e.target==sliceHeightSlider){
        //layertestZ=e.value;
    }
    if(e.target==layerthicknessSlider){
        // layerthickness=e.value;
    }
    if(e.target==exposedTimeSlider){
        float i=e.value;
        exposedTimeSlider->setValue(i);
        exposedTime=e.value;
    }
    //cout << "the new value of the slider = " << e.value << endl;
    //cout << "the new scale of the slider = " << e.scale << endl;
}
void agpanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target==printStartButton){
        workState=statePrintPreparing;
        if(isOutput==true){
            bPrint=true;
        }
        if(workState==statePrintReady){
            workState=statePrintPreparing;
        }
        
    }
    if(e.target==printStopButton){
        if(workState!=statePrintFinish){
            workState=statePrintFinish;
        }
        if(isOutput==true){
            bPrint=false;
        }
        
        
    }
    if(e.target==connectButton){
        tryConnect();
        
    }
    
}
void agpanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    if(serial.isInitialized()==false){return;}
    for(int i=0;i<e.text.size();i++){
        serial.writeByte(e.text[i]);
    }
    
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
    //sliceHeightSlider->bind(sliceHeight);
    sliceHeightSlider->bind(layertestZ);
    layerthicknessSlider->bind(layerthickness);
    
    
}

void agpanel::tryConnect(){
    if(serial.isInitialized()==false){
        serial.setup(0,115200);
    }else{
        serial.close();
        serial.setup(0,115200);
    }
    
}

void agpanel::serialUpdate(){
    if(serial.isInitialized()==false){
        return;
    }
    if(isConnect==true){
        connectButton->setLabel("amber is ON");
    }
    
    teststring="Serial_\n";
    int myByte = 0;
    while(serial.available()>0){
        isConnect=true;
        myByte = serial.readByte();
        if ( myByte == OF_SERIAL_NO_DATA ){
            cout<<"no data was read"<<endl;
        }
        else if ( myByte == OF_SERIAL_ERROR ){
            cout<<"an error occurred"<<endl;
        }
        else{
            teststring+=myByte;
            cout<<teststring<<endl;
        }
    }
    
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
