#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

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
    void drawModel();
    void addModel(ofMesh model);// add a model
    void addModel(string modelpath);//add a model with assimp
    void deleteModel();
    void cleanModel();
    void setPosition(ofVec3f newpostion);
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
    ofVec3f modelMoce;
    ofVec3f boxSize;// the box size
    ofVec3f pixelSize;
    float groundheight;
  
    ofMatrix4x4 mreset;
   
  //drawmode set
    int drawmode=0;//0 for meshes 1 for assimps
    
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    
    //models
    vector<ofMesh> models;
    ofxAssimpModelLoader assimploader;
    of3dPrimitive nodemodel;
    ofEasyCam cam;
    //ofRectangle view=ofRectangle(0, 0, 320, 320);
    
};
