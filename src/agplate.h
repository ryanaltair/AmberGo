#pragma once
#include "ofMain.h"

/**
 use for AmberGo
 as plate
 include:
 model(s)
 ground
 outsidebox
 
 unit: mm
 
 **/
class agplate{
public:
    agplate();
    ~agplate();
    void setup();
    void update();
    void sliceAt(float Z);
    void drawincamera(ofRectangle view);
    void drawModels();
    void addModel(ofMesh model);// add a model
    void deleteModel();
    void cleanModel();
    void setPosition(ofVec3f newpostion);
    // workplace:
    ofBoxPrimitive playground;    // ground
    ofBoxPrimitive outsideBox;    // box
    ofBoxPrimitive sliceLayPlane;    // slicelayer
    
    float layertestZ=0;
    ofVec3f modelSize;
    ofVec3f boxSize;// the box size
    ofVec3f pixelSize;
    float groundheight;
    int drawmode=0;//drawmode set 0 for meshes 1 for assimps
    
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    
    vector<of3dPrimitive> nodemodels;    //models
    ofEasyCam cam;
    
};
