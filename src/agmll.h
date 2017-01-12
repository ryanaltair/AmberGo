#pragma once

#include "ofMain.h"
#include "agline.h"
class agmll{
public:
    agmll();
    ~agmll();
    void setup(ofMesh mesh);
    void draw();
    void update();
    void cleanmermory();
    float dH=0.01;
    
    
    
    vector<ofVec3f> pointlist;// get the real point p=pointlist[ip]
    vector<ofIndexType> linelist;// {ip0 ip1} the index point to point list add with addnewline()
    map<agline,ofIndexType> linecopymap;// use for searchline , for great speed
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline() work with adddXdY
    vector<ofIndexType> nearpointlist;//{ipa ipb} the index refer point list add with addnewline() and addoldline
    vector<float> dXdYlist;// init with addnewline() work with adddXdY()
    vector<int> linetypelist;//init with addnewline() work with adddXdY()
    vector<float> touchedlist;//init with addnewline() work with adddXdY()
    ofPath layertestat(float z);
    ofPath layertestcloseloop(float z,ofIndexType ipbegin);
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
    void addface();
    void adddXdY();
    void addlinetype(ofIndexType i,int linetype,int riseorfall);
    void addtouchedlist(ofIndexType i,float isTouchedOrNot,float ZMax);
     ofVec3f getScale();
    //static
    ofVec3f getScale(ofMesh mesh);
private:
    //do in setup
    void addpointlist(ofMesh mesh);
    
    //do once
    
    //do loop
    
    // theory
    ofVec3f getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z);
    void addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn);
    void addoldline(ofIndexType ipl,ofIndexType ipn);
    ofIndexType searchline(ofIndexType ip0,ofIndexType ip1);
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    ofIndexType findcrosspointat(float z);// return ip0
    bool isPointPlaneCross(ofIndexType indexpoint0,ofIndexType indexpoint1,int riseorfall,float planeatz);
    
    
    
    // progress flag
    size_t linelistloaded=0;
    size_t dxdylistloaded=0;
    
    int horizonline=1;
    int verticalline=2;
    int bevelline=3;
    int riseline=1;
    int fallline=2;
    int evenline=3;//just even z1=z2
    int unknown=0;
    
    float isTouched=1;
    float isUntouched=0;
    
    
    //layertest only
    void stepreset(); 
    void addPointToPath(float x,float y,ofIndexType i);
    void alluntouched();
    void justtouch(ofIndexType ip);
    ofIndexType continueflag=0;
    float testatZoffset=-1;
    //dX=dXdYlist[ip0]
    //dY=dXdYlist[ip1]
    // how to refer a point
    // point =pointlist[iplp0]
    // point =pointlist[linelist[ip0]]
    // point =pointlist[nearpointlist[ip0]]
    // point =pointlist[ipa]
    

    
    //layertest end 
    
    
    //debug only
    void debuglinelist(ofIndexType ip);
    ofIndexType counter0=0;
    ofIndexType counter1=0;
};

