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
    
   ofPath layertestat(float z);// return a whole layer path
    ofPath layerAt(float z);// [new]
    ofPath layertestcloseloop(float z,ofIndexType iBegin);// return a single closed loop path
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
    void adddXdY();
    void addlinetype(ofIndexType i,int linetype,int riseorfall);
    void addtouchedlist(ofIndexType i,float isTouchedOrNot,float ZMax);
    agline zsortline(agline line);
    // theory
    ofVec3f getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float divdH,float z);
    ofIndexType searchline(agline line);
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    ofIndexType findcrosspointat(ofIndexType startflag,float z);// a new return cross
   bool isPointPlaneCross(ofVec3f pointHigher,ofVec3f pointLower,float planeatz);
    
    
    const int horizonline=1;
    const int verticalline=2;
    const int bevelline=3;
    int riseline=1;
    int fallline=2;
    int evenline=3;//just even z1=z2
    int unknown=0;
    
    float isTouched=1;
    float isUntouched=-1;
    
    
    
    //layertest only
    void justtouch(ofIndexType i);
    //layertest end
    
    
    //debug only
    ofIndexType indexsize;
    ofIndexType counter0=0;
    ofIndexType counter1=0;
    void printlineandpoint(){
        cout<<"sliceModel.linelist "<<sliceModel.linelist.size()/2<<":"<<endl;
        for(ofIndexType i=0;i<sliceModel.linelist.size();i+=2){
            cout<<"line \t"<<i/2+1<<"\t"<<sliceModel.linelist[i]<<"\t"<<sliceModel.linelist[i+1];
            if(sliceModel.pointlist[sliceModel.linelist[i]].z==sliceModel.pointlist[sliceModel.linelist[i+1]].z){
                cout<<"\t -"<<endl;
            }else{
                cout<<endl;
            }
        }
        cout<<"sliceModel.pointlist "<<sliceModel.pointlist.size()<<":"<<endl;
        for(ofIndexType i=0;i<sliceModel.pointlist.size();i+=1){
            cout<<"point \t "<<i<<"\t"<<sliceModel.pointlist[i]<<endl;
        }
        
    }
    void printsize(){
        ofIndexType linecount=sliceModel.linelist.size();
        ofIndexType trianglecount=indexsize/3;
        cout<<"line count :"<<linecount<<endl;
        cout<<"point count :"<<sliceModel.pointlist.size()<<endl;
        cout<<"indices size:"<<indexsize<<endl;
        if(linecount==indexsize){
            cout<<"it seems calc in right way "<<endl;
        }else{
            cout<<"it seems calc in wrong way "<<endl;
            
        }
    }
    void debuglinelist(ofIndexType index){
        cout<<"index0,index1:"<<index<<","<<index+1<<"   ";
        if(index+1>sliceModel.linelist.size()){
            cout<<"waring:ip>sliceModel.linelist.size"<<endl;
            return;
        }else{
            cout<<"sliceModel.linelist.size:"<<sliceModel.linelist.size()<<"   ";
        }
        ofIndexType ip0=sliceModel.linelist[index];
        ofIndexType ip1=sliceModel.linelist[index+1];
        cout<<"point index is:"<<ip0<<":"<<ip1<<"   ";
        if(ip0>sliceModel.pointlist.size()||ip1>sliceModel.pointlist.size()){
            cout<<"waring:index of point > sliceModel.pointlist.size"<<endl;
            return;
        }else{
            cout<<"sliceModel.pointlist.size:"<<sliceModel.pointlist.size()<<"   ";
        }
        ofPoint point0,point1;
        point0=sliceModel.pointlist[ip0];
        point1=sliceModel.pointlist[ip1];
        cout<<"line from:"<<ofToString(point0)<<" to "<<ofToString(point1)<<"   ";
        if(point0.x==point1.x&&point0.y==point1.y&&point0.z==point1.z){
            cout<<"this is a singlepoint;"<<endl;
        }else{
            cout<<"this is a line;"<<endl;
        }
        
    }

    
};

