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
    void update();
    void drawincamera();
    void drawModel();
    void addModel(ofMesh model);// add a model
    void deleteModel();
    void cleanModel();
    // workplace:
    // ground
    ofBoxPrimitive playground;
    ofMatrix4x4 mplayground;
    // box
    ofBoxPrimitive outsideBox;
    ofMatrix4x4 moutsideBox;
    // slicelayer
    ofBoxPrimitive sliceLayPlane;
    ofMatrix4x4 msliceLayPlane;
    float layertestZ=0;
    ofVec3f modelSize;
    ofVec3f boxSize;// the box size
    ofVec3f pixelSize;
    float groundheight;
  
    ofMatrix4x4 mreset;
   
  
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    
    //models
    vector<ofMesh> models;
    ofMatrix4x4 mreadyModel;

    
    
};
