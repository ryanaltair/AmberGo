#pragma once

#include "ofMain.h"
#include "agline.h"
#include "agmllhelper.h"
/**
 work flow:
 get the mesh
 merge it
 calculate:
 1. get the point list
 2. use mesh data to output linelist
 3. use linelist to get the polygon
 output data use sliceat
 
 **/
class agmll{
public:
    agmll();
    ~agmll();
    void setup(ofMesh mesh);
    void calcaulateModel();
    void cleanmermory();
    ofVec3f getScale();
    float dH=0.01;
    ofPath layerAt(float z);// [new]
    ofPath layerCloseLoop(float z,ofIndexType iBegin);
    ofPath layertest;// store the output path
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    ofMesh mergedMesh;//the mesh clone
    
private:
    agModel sliceModel;
    float divdH;// divdH=1/H
    //do in setup
    void addpointlist();
    // calcaulateModel job
    void addFacet();
    agline zsortline(agline line);

    //debug only
    ofIndexType indexsize;
    ofIndexType counter0=0;
    ofIndexType counter1=0;
  
    void printsize(){
        ofIndexType linecount=sliceModel.multilinklinelist.size();
        
        ofIndexType trianglecount=indexsize/3;
        cout<<"indices size:"<<indexsize<<endl;
        cout<<"line count :"<<linecount<<endl;
        cout<<"point count :"<<sliceModel.pointlist.size()<<endl;
        if(linecount*2==indexsize){
            cout<<"it seems calc in right way "<<endl;
        }else{
            cout<<"it seems calc in wrong way "<<endl;
            
        }
    }
    void debuglinelist(ofIndexType index){
        
    }

    
};

