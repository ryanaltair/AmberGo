#pragma once

#include "ofMain.h"
#include "agline.h"
class agmll{
public:
    agmll();
    void setup(ofMesh mesh);
   
    void draw();
    void update();
    float dH=0.01;
    vector<ofVec3f> pointlist;
    vector<ofIndexType> linelist;// {p0 p1} the index point to point list add with addnewline()
    map<agline,ofIndexType> linecopymap;// use for searchline , for great speed
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline() work with adddXdY
    vector<ofIndexType> nearpointlist;//{pa pb} the index point to point list add with addnewline() and addoldline
    vector<float> dXdYlist;// init with addnewline() work with adddXdY()
    vector<int> linetypelist;//init with addnewline() work with adddXdY()
    vector<float> touchedlist;//init with addnewline() work with adddXdY()
   // vector<ofIndexType> linehashlist;//init with addnewline() work with searchline() but it is slow
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
    
    // tools
    ofVec3f getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z);
    
    void addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn);
    void addoldline(ofIndexType ipl,ofIndexType ipn);
    ofIndexType searchline(ofIndexType ip0,ofIndexType ip1);
    ofPath addPointToPath(ofPath path,float x,float y,ofIndexType i);
    void addPointLineToPath(ofVec3f addpoint);
    void addPointMoveToPath(ofVec3f addpoint);
    
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
    void stepstart(ofIndexType i);
    void steploop();
    void stepHorizon();
    void checkpnextZ();
    void getipHipLfrom(ofIndexType indexpoint0,ofIndexType indexpoint1);
    void addPointToPath(float x,float y,ofIndexType i);
    void alluntouched();
    void justtouch(ofIndexType ip);
    ofIndexType continueflag=0;
    ofIndexType findnextline(ofIndexType lineip0,ofIndexType lineip1);
    ofPath layertestpath;
    ofIndexType ipstartL=0;
    ofIndexType ipstartH=0; // ipstart0<ipstart1 always
    ofIndexType ipstarta=0;// we never use ipa as next point until we find it
    ////cout<<"ipstart:"<<ipstartL<<":"<<ipstartH<<"\n";
    ofIndexType ipnext=0;
    ofIndexType ipused=0;
    
    ofIndexType ip0=0;//linelist[ip0]
    ofIndexType ip1=0;//linelist[ip1]
    ofIndexType iplp0=0;//pointlist[iplp0] nextline's ip0
    ofIndexType iplp1=0;//pointlist[iplp1] nexeline's ip1
    
    ofIndexType ipH=0;
    ofIndexType ipL=0;
    ofIndexType ipa=0;//ipa=nearpointlist[ip0]
    ofIndexType ipb=0;//ipb=nearpointlist[ip1]
    float testatZ;
    float testatZoffset=0;
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

