#pragma once
#include "ofMain.h"
/**
use for AmberGo
 as plate
 include:
 model(s)
 ground
 outsidebox
 


**/
class gkplate{
public:
    gkplate();
    ~gkplate();
    void setup();
    void drawincamera();
    void drawModel();
    void addModel(ofMesh model);// add a model
    void deleteModel();
    void cleanModel();
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
    
    //models
    vector<ofMesh> models;
    ofMatrix4x4 mreadyModel;

    
    
};
