#pragma once

#include "ofMain.h"
#include "agline.h"

class agmllhelper{
    agmllhelper();
    
    
};
class agModel{
public:
    agModel(){
        
    }
    /**
     use swap to clean the vector
     */
    void cleanmermory(){
        // let us do the clean
        vector<ofVec3f>().swap(pointlist);
        horizonFacetHeightlist.clear();
        pointlist.clear();
        multilinklinelist.clear();
        mllcopymap.clear();
        scaleMax.set(0,0,0);
        scaleMin.set(0,0,0);
        //
    }
    bool printAllWrongLine(){
        bool r=true;
        for(auto &linkline:multilinklinelist){
            if(linkline.isFilled()==false){
                r=false;
            }
        }
        if(r){
            cout<<"it gets all right"<<endl;
        }
        return r;
    
    }
    void tryFix(){
        cout<<"now we try fix"<<endl;
        vector<ofIndexType> wronglinelist;
     vector<agmultilinkline> bufferlist;// to replace linelist and nearpoint list
        for(int i=0;i<multilinklinelist.size();i++){
            agmultilinkline linkline=multilinklinelist[i];
            if(linkline.isFilled()==false){
                bufferlist.push_back(linkline);
                wronglinelist.push_back(i);
            }
            
        }
    
        for(int i=0;i<wronglinelist.size();i++){
            agmultilinkline line=multilinklinelist[wronglinelist[i]];
            ofIndexType ip0,ip1;
            ip0=line.ip0;
            ip1=line.ip1;
            agline oldline;
            oldline.set(ip0,ip1);
            for(int j=0;j<bufferlist.size();j++){
                agmultilinkline compareLine=bufferlist[j];
                if(ip0==compareLine.ip0||ip1==compareLine.ip0){
                    agline newline=zsortline(oldline);
                    addLine(newline,compareLine.ip1);//add line with dxdy
                    break;
                }
                if(ip0==compareLine.ip1||ip1==compareLine.ip1){
                    agline newline=zsortline(oldline);
                    addLine(newline,compareLine.ip0);//add line with dxdy
                    break;
                }
            }
        }
    }
    bool addLine(agline sortline,ofIndexType ipnear){
        bool isFind=false;
        ofIndexType findindex;
        if(1){
            auto it=mllcopymap.find(sortline);
            if(it==mllcopymap.end()){
                //not found
                isFind=false;
                zero++;
            }else{
                //found
                isFind=true;
                findindex=it->second;
                zero--;
            }
        }
        if(isFind){
            if(multilinklinelist[findindex].isFilled()==true){
                cout<<"[WRONG]"<<endl;
                wrongIndexList.push_back(findindex);
            }else{
            
            multilinklinelist[findindex].addNearPoint(ipnear,pointlist[ipnear].z);
            }
            return false;
        }else{
            agmultilinkline newmultiline(sortline);
            newmultiline.addNearPoint(ipnear,pointlist[ipnear].z);
            mllcopymap[sortline]=multilinklinelist.size();
            ofVec3f p0=pointlist[newmultiline.ip0];
            ofVec3f p1=pointlist[newmultiline.ip1];
            checkScale(p0);
            checkScale(p1);
            newmultiline.adddXdY(p0,p1,dH);// add dx dy and zmax and untouched
            multilinklinelist.push_back(newmultiline);
            return true;
        }
//        printZero();
    }
    
    void printZero(){
        if(zero==0){
            cout<<"is add ok"<<endl;
        }else{
            cout<<"is add failed "<<endl;
            
        }
    }
    bool find(agline sortline){
        auto it=mllcopymap.find(sortline);
        if(it==mllcopymap.end()){
            //not found
            return false;
        }else{
            //found
            return true;
        }
    }
    ofIndexType searchLine(agline sortline){
        auto it=mllcopymap.find(sortline);
        if(it==mllcopymap.end()){
            //not found
            return multilinklinelist.size();
        }else{
            //found
            return it->second;
        }
    }
    void alluntouched(){
        for(ofIndexType i=0;i<multilinklinelist.size();i=i+1){
            multilinklinelist[i].untouch();
        }
    
    }
    void refresh(){
        for(ofIndexType i=0;i<multilinklinelist.size();i=i+1){
            multilinklinelist[i].refresh();
        }
    
    }
    
    
    
    float getNoHorizonHappenZ(float z){
        float zNew;
        size_t findzhorizon=ofFind(horizonFacetHeightlist, z);
        if(findzhorizon<horizonFacetHeightlist.size()){// means find it is horizon facet
            if(findzhorizon+1!=horizonFacetHeightlist.size()){
                float zmoveup=horizonFacetHeightlist[findzhorizon+1]-z;
                if(zmoveup>dH){
                    zNew=z+0.001;// now we safa to slice at z
                }else{
                    zNew=z+zmoveup/2;// now we safa to slice at z
                }
            }else{
                zNew=z+0.001;
            }
        }else{
            zNew=z;
        }
        return zNew;
    }
    
    ofVec3f getXY(agmultilinkline mlline,float divdH,float z,float xstart,float ystart){
        //TO DO:
        ofVec3f pH;
        ofVec3f pL;
        const float xm=1280/126.72;
        const float ym=768/74.88;
        pL= pointlist[mlline.ip0];
        pH= pointlist[mlline.ip1];
        
        ofVec3f returnpoint;
        float h=z-pL.z;
        float divdHxh=divdH*h;
        returnpoint.x=pL.x+mlline.dx*divdHxh;
        returnpoint.x*=xm;
        
        returnpoint.x+=xstart;
        returnpoint.y=pL.y+mlline.dy*divdHxh;
        returnpoint.y*=ym;
        
        returnpoint.y+=ystart;
        returnpoint.z=z;
        return returnpoint;
    }
    
    void checkScale(ofVec3f p){
        if(p.x>=scaleMax.x){
            scaleMax.x=p.x;
        }
        if(p.y>=scaleMax.y){
            scaleMax.y=p.y;
        }
        if(p.z>=scaleMax.z){
            scaleMax.z=p.z;
        }
        
        //b
        if(p.x<=scaleMin.x){
            scaleMin.x=p.x;
        }
        if(p.y<=scaleMin.y){
            scaleMin.y=p.y;
        }
        if(p.z<=scaleMin.z){
            scaleMin.z=p.z;
        }
    }
    ofIndexType findcrosspointat(ofIndexType startflag,float z){
        // find a cross point
        for(int i=startflag;i<multilinklinelist.size();++i){
            // we don't touch anything that already touched
            if(multilinklinelist[i].isTouched()){
                continue;// cout<<"it was touched"<<endl;
            }else{
                multilinklinelist[i].touch();// cout<<"it's not touched so we touch it "<<ip0<<endl;
            }
            // we will skip when z not in zmax and zmin
            if(multilinklinelist[i].isCross(z)){
                return i;
            }else{
                continue;
            }
        }
        //  cout<<"we find nothing any more so we quit"<<endl;
        return multilinklinelist.size();
    }
    ofVec3f getScale(){
        return scaleMax-scaleMin;
    }
    vector<ofVec3f> pointlist;// get the real point p=pointlist[ip]
    vector<agmultilinkline> multilinklinelist;// to replace linelist and nearpoint list
    map<agline,ofIndexType> mllcopymap;// use for searchline , for great speed
    vector<float> horizonFacetHeightlist;// hold every height that get horizong facet/triangle
    vector<ofIndexType> wrongIndexList;
    ofVec3f scaleMax;
    ofVec3f scaleMin;
    float dH=0.01;
    agline zsortline(agline line){
        ofIndexType ip0,ip1,ipa,ipb;
        ip0=line.ip0;
        ip1=line.ip1;
        if(ip0>pointlist.size()||ip1>pointlist.size()){
            cout<<"we get a bigggggg ip "<<ip0<<"\t"<<ip1<<"\t"<<pointlist.size()<<endl;
        }
        agline newline;
        if(pointlist[ip0].z<pointlist[ip1].z){
            newline.set(ip0,ip1);
        }else if(pointlist[ip0].z>pointlist[ip1].z){// ip1<ip0
            newline.set(ip1,ip0);
        }else{   // if they the same      // make sure ip0<ip1
            if(ip0<=ip1){
                newline.set(ip0,ip1);
            }else{
                newline.set(ip1,ip0);
            }
        }
        newline.markZSortTrue();// mark zsort that we can use it to search or add to linelist correctly
        return newline;
    }
protected:
    
    int zero=0;
};
