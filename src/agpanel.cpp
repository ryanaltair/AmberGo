#include "agpanel.h"

agpanel::agpanel(){

}


void agpanel::setup(){

    // GUI start
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    //gui->addBreak();
    //gui->addButton("Click!");
    sliceHeightSlider=gui->addSlider("sliceHeight", 0, 1393,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    printStartButton=gui->addButton("start print");
    
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    printStartButton->onButtonEvent(this, &agpanel::onButtonEvent);
    
}
void agpanel::update(){

    if(workState==statePrintPreparing){
        sliceHeight=0.02;
    }
    if(workState==statePrinting){
        if(isTimeToNextLayer==true){
            sliceHeight+=0.16;
            isTimeToNextLayer=false;
        }
    }
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
        if(workState==statePrintReady){
            workState=statePrintPreparing;
        }
    
    }
    cout << e.target->getLabel() << " was clicked!"  << endl;
}
void agpanel::sliceHeightBind(float sliceheight){
    sliceHeightSlider->bind(sliceHeight);

}
 
