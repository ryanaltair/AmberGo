#pragma once

#include "ofMain.h"
#include "agline.h"
#include "agmllhelper.h"
#include "agfacet.h"
#include "agSliceShell.h"
#include "ofxEditableSVG.h"
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
    void load(ofMesh mesh);
    void prepareModel();
    void cleanmermory();
    ofVec3f getScale();
    float dH=0.01;
    ofPath layerAt(float z);// [new]
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    ofMesh mergedMesh;//the mesh clone
    ofPath supportPolygon;
    ofMesh getMergedMesh(){
        return mergedMesh;
    }
private:
    ofMesh getSliceShell();
    void addSupport();
    ofPath layerCloseLoop(float z,ofIndexType iBegin);
    agModel sliceModel;
    float divdH;// divdH=1/H
    //do in setup
    void addpointlist();
    // prepareModel job
    void addFacet();
    //tools


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
        cout<<"triganel count :"<<trianglecount<<endl;
        if(linecount*2==indexsize){
            cout<<"it seems calc in right way "<<endl;
        }else{
            cout<<"it seems calc in wrong way "<<endl;
        }
    }
    void debuglinelist(ofIndexType index){
        
    }
    int layerCount=0;
    
};

