#pragma once

#include "ofMain.h"
#include "agline.h"

/**
 work flow:
 get the mesh
 merge it
 calculate:
 1. get the point list
 2. use mesh data to output linelist
 
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
    
    
    // main data
    vector<ofVec3f> pointlist;// get the real point p=pointlist[ip]
    vector<ofIndexType> linelist;// hlod {ipL ipH} the index point to point list add with addnewline() and pL will be lower
    map<agline,ofIndexType> linecopymap;// use for searchline , for great speed
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline() work with adddXdY
    vector<ofIndexType> nearpointlist;//{ipa ipb} the index refer point list add with addnewline() and addoldline
    vector<float> dXdYlist;// init with addFacet() work with adddXdY()
    vector<int> linetypelist;//init with addFacet() work with adddXdY()
    vector<float> touchedlist;//init with addFacet() work with adddXdY()
    vector<float> horizonFacetHeightlist;// hold every height that get horizong facet/triangle
    ofPath layertestat(float z);// return a whole layer path
    ofPath layertestcloseloop(float z,ofIndexType iBegin);// return a single closed loop path
    ofPath layertest;// store the output path
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    
    ofMesh mergedMesh;//the mesh clone
    
private:
    float divdH;// divdH=1/H
    //do in setup
    void addpointlist();
    // calcaulateModel job
    void addFacet();
    void adddXdY();
    void addlinetype(ofIndexType i,int linetype,int riseorfall);
    void addtouchedlist(ofIndexType i,float isTouchedOrNot,float ZMax);
    agline zsortline(agline line);
    // theory
    ofVec3f getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float divdH,float z);
    ofIndexType searchline(agline line);
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    ofIndexType findcrosspointat(ofIndexType startflag,float z);// a new return cross
    bool isPointPlaneCross(ofIndexType indexpoint0,ofIndexType indexpoint1,int riseorfall,float planeatz);
    bool isPointPlaneCross(ofVec3f pointHigher,ofVec3f pointLower,float planeatz);
    
    
    int horizonline=1;
    int verticalline=2;
    int bevelline=3;
    int riseline=1;
    int fallline=2;
    int evenline=3;//just even z1=z2
    int unknown=0;
    
    float isTouched=1;
    float isUntouched=-1;
    
    ofIndexType indexsize;
    
    //layertest only
    
    void justtouch(ofIndexType i);
    float testatZoffset=0.00001;
    
    
    //layertest end
    
    
    //debug only
    void debuglinelist(ofIndexType index);
    ofIndexType counter0=0;
    ofIndexType counter1=0;
    void printlineandpoint(){
        cout<<"linelist "<<linelist.size()/2<<":"<<endl;
        for(ofIndexType i=0;i<linelist.size();i+=2){
            cout<<"line \t"<<i/2+1<<"\t"<<linelist[i]<<"\t"<<linelist[i+1];
            if(pointlist[linelist[i]].z==pointlist[linelist[i+1]].z){
                cout<<"\t -"<<endl;
            }else{
                cout<<endl;
            }
            
        }
        cout<<"pointlist "<<pointlist.size()<<":"<<endl;
        for(ofIndexType i=0;i<pointlist.size();i+=1){
            cout<<"point \t "<<i<<"\t"<<pointlist[i]<<endl;
        }
        
    }
    void printsize(){
        ofIndexType linecount=linelist.size();
        ofIndexType trianglecount=indexsize/3;
        cout<<"line count :"<<linecount<<endl;
        cout<<"point count :"<<pointlist.size()<<endl;
        cout<<"indices size:"<<indexsize<<endl;
        if(linecount==indexsize){
            cout<<"it seems calc in right way "<<endl;
        }else{
            cout<<"it seems calc in wrong way "<<endl;
            
        }
    }
    
};

