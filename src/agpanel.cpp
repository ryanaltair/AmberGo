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
    ofxDatGuiSlider* sliceHeightSlider=gui->addSlider("slide", 0, 100,0);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
    sliceHeightSlider->onSliderEvent(this, &agpanel::onSliderEvent);
    
}


void agpanel::onSliderEvent(ofxDatGuiSliderEvent e)
{
    sliceHeight=e.value;
    cout << "the new value of the slider = " << e.value << endl;
    cout << "the new scale of the slider = " << e.scale << endl;
}
