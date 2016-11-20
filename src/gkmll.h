#pragma once

#include "ofMain.h"

class gkmll{
public:
    gkmll();
    void setup(ofMesh mesh);
   
    void draw();
    void update(ofMesh mesh);
    void brake();
    void accellerate();
    void steer(float dir);
    float dH=0.01;
    vector<ofVec3f> pointlist;
    vector<ofIndexType> linelist;// {p0 p1} the index point to point list
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline work with adddXdY
    vector<ofIndexType> nearpointlist;//{pa pb} the index point to point list
    vector<float> dXdYlist;// add with addnewline work with adddXdY
    ofPath layertestat(ofMesh mesh,float z,int tri=1);
    
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    ofIndexType linecount=0;
    int islinelistfilled=0;//100 means filled
    int isdXdYlistfilled=0;//100 means filled
    
    // main job
    void addface(ofMesh mesh,ofIndexType i);
    void adddXdY(ofIndexType i);
private:
    //do in setup
    void addpointlist(ofMesh mesh);
    ofVec3f getScale(ofMesh mesh);

    //do once
    
    //do loop
    
    // tools
    void addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipa,ofIndexType ipb);
    void addoldline(ofIndexType ipl,ofIndexType ipn);
    ofIndexType searchline(ofIndexType ip0,ofIndexType ip1);
    ofPath addPointToPath(ofPath path,float x,float y,ofIndexType i);
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    
    // progress flag
    size_t linelistloaded=0;
    size_t dxdylistloaded=0;
    
};
