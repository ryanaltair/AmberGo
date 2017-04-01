#pragma once
#include "ofMain.h"
#include "agSliceShell.h" //for quick test
/**
 use for AmberGo
 NOTICE:all model in this plate are using mm as unit for stl
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
    void setMinPosition(ofVec3f min){
        modelMin=min;
        updatePosition();
    }
    void setScaleFactor(ofVec3f sf){
        if(scaleFactor!=sf){
            scaleFactor=sf;
        }
         updatePosition();
    }
    void updatePosition(){
        ofVec3f newpostion=modelMin;
        newpostion.x-=modelSize.x/2*scaleFactor.x;//-plate.modelSize.x/2;
        newpostion.y-=modelSize.y/2*scaleFactor.y;//-plate.modelSize.y/2;
        if(nodemodels.size()>0){
            nodemodels[0].setPosition(newpostion);
//            cout<<"position"<<ofToString(newpostion);
        }
    }
     
    // workplace:
    ofBoxPrimitive playground;    // ground
    ofBoxPrimitive outsideBox;    // box
    ofBoxPrimitive sliceLayPlane;    // slicelayer
    
    ofVec3f modelMin;
    ofVec3f modelSize;
    ofVec3f boxSize;// the box size
    ofVec3f pixelSize;
    ofVec3f scaleFactor;
    float groundheight;
    int drawmode=0;//drawmode set 0 for meshes 1 for assimps
    
    vector<of3dPrimitive> nodemodels;    //models
    ofEasyCam cam;
 private:
    float slicelayerZ=0;
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    vector<ofLight> aroundLight;
    agSliceShell sliceShell;
    bool btest=false;
};
