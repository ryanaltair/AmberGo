#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
/**
 use for AmberGo
 as panel
 
 **/

class gkpanel{
public:
    gkpanel();
    void setup();
    
    ofxDatGui* gui;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    



};
