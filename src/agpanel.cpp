#include "agpanel.h"

agpanel::agpanel(){

}


void agpanel::setup(){
//initial
    isConnect=false;
    
    // GUI start
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    //gui->addBreak();
    //gui->addButton("Click!");
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 1393,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    printStartButton=gui->addButton("start print");
    connectButton=gui->addButton("amber is offline");
   
    sendMessageTextInput=gui->addTextInput("sendMsg:");
     gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    printStartButton->onButtonEvent(this, &agpanel::onButtonEvent);
    connectButton->onButtonEvent(this,&agpanel::onButtonEvent);
    sendMessageTextInput->onTextInputEvent(this,&agpanel::onTextInputEvent);
}
void agpanel::update(){
    serialUpdate();
    
    timeUpdate();
    
    if(workState==statePrintPreparing){
        cout<<"now we are print preparing"<<endl;
        sliceHeight=0.2;
        timerToNextLayer=ofGetElapsedTimef()+5;
        workState=statePrinting;
    }
    if(workState==statePrinting&&outputdone==true){
        
       
        if(isTimeToNextLayer==true){
             cout<<"now we are printing"<<endl;
            if(sliceHeight>1000){
                sliceHeight=-1;
                workState=statePrintFinish;
                }else{
            sliceHeight+=1.35;
                    cout<<"we change slice height at "<< ofGetElapsedTimef()<<" seconds"<<endl;
                }
            cout<<"tiemr before "<<timerToNextLayer;
            timerToNextLayer=ofGetElapsedTimef()+exposeTime;
            
            cout<<" tiemr after "<<timerToNextLayer<<endl;
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
    sliceHeight=e.value;
    //cout << "the new value of the slider = " << e.value << endl;
   //cout << "the new scale of the slider = " << e.scale << endl;
}
void agpanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(e.target==printStartButton){
        workState=statePrintPreparing;
        if(workState==statePrintReady){
            workState=statePrintPreparing;
        }
    
    }
    if(e.target==connectButton){
        tryConnect();
    
    }
    cout << e.target->getLabel() << " was clicked!"  << endl;
}
void agpanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    if(serial.isInitialized()==false){return;}
    for(int i=0;i<e.text.size();i++){
        serial.writeByte(e.text[i]);
    }

}
void agpanel::sliceHeightBind(float sliceheight){
    sliceHeightSlider->bind(sliceHeight);

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

