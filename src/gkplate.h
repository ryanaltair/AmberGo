#pragma once
#include "ofMain.h"
/**
use for AmberGo
 as plate


**/
class gkplate{
public:
    gkplate();
    ~gkplate();
    void setup();
    void drawincamera();
    // workplace:
    ofBoxPrimitive playground;
    ofMatrix4x4 mplayground;
    
    ofBoxPrimitive outsideBox;
    ofMatrix4x4 moutsideBox;
    

    ofVec3f size;
    ofVec3f pixelSize;
    float groundheight;
  
    ofMatrix4x4 mreset;
   
  
    // enables
    int outsideBoxEnable=0;

    
};
