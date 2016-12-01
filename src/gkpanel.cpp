#include "gkpanel.h"

gkpanel::gkpanel(){

}


void gkpanel::setup(){

    // GUI start
    // instantiate and position the gui //
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    
    gui->addFRM();
    //gui->addBreak();
    //gui->addButton("Click!");
    gui->addSlider("slide", 20, 0);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    //GUI end
}
