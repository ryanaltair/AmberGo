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
    sliceHeightSlider=gui->addSlider("slide", 0, 100,0);
    sliceProgressPercentSlider=gui->addSlider("progress", 0, 100, 0);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    
}
void agpanel::update(){

   // sliceHeightSlider->setValue((double)sliceHeight);

}

void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    sliceHeight=e.value;
    cout << "the new value of the slider = " << e.value << endl;
    cout << "the new scale of the slider = " << e.scale << endl;
}

void agpanel::sliceHeightBind(float sliceheight){
    sliceHeightSlider->bind(sliceHeight);

}
 
