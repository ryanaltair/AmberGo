#pragma once

#include "ofMain.h"

class agmll{
public:
    agmll();
    void setup(ofMesh mesh);
   
    void draw();
    void update();
    void brake();
    void accellerate();
    void steer(float dir);
    float dH=0.01;
    vector<ofVec3f> pointlist;
    vector<ofIndexType> linelist;// {p0 p1} the index point to point list
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline work with adddXdY
    vector<ofIndexType> nearpointlist;//{pa pb} the index point to point list
    vector<float> dXdYlist;// add with addnewline work with adddXdY
    ofPath layertestat(float z);
    ofPath layertest;
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    ofIndexType linecount=0;
    //process flag
    int ismeshMerged=0;//100 means done
    int islinelistfilled=0;//100 means filled
    int isdXdYlistfilled=0;//100 means filled
    int isAllReady=0;//100 means all done
    //the mesh clone
    ofMesh mergedMesh;
    // main job
    void addface(ofMesh mesh,ofIndexType i);
    void adddXdY(ofIndexType i);
     ofVec3f getScale();
    //static
    ofVec3f getScale(ofMesh mesh);
private:
    //do in setup
    void addpointlist(ofMesh mesh);
    
    //do once
    
    //do loop
    
    // tools
    ofVec3f getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z);
    
    void addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn);
    void addoldline(ofIndexType ipl,ofIndexType ipn);
    ofIndexType searchline(ofIndexType ip0,ofIndexType ip1);
    ofPath addPointToPath(ofPath path,float x,float y,ofIndexType i);
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    
    // progress flag
    size_t linelistloaded=0;
    size_t dxdylistloaded=0;
    
};
