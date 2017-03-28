#pragma once

#include "ofMain.h"

class agline{
public:
    agline(){
    }
    ofIndexType ip0,ip1;//    ipLow is ip0 after sorted  ipHigh is ip1 after sorted
    void set(ofIndexType ip0Value,ofIndexType ip1Value){
        ip0=ip0Value;
        ip1=ip1Value;
    }
    void markZSortTrue(){
        zsort=true;
    };
    bool getzsort(){
        return zsort;
    };
    bool operator < ( const agline& other) const
    {
        if ( ip0 == other.ip0 ) {
            return ip1 < other.ip1;
        }
        return ip0 < other.ip0;
    }
    
protected:
    bool zsort=false;
    
};
class nearPointLink{
public:
    nearPointLink(){
    }

    nearPointLink(ofIndexType pointIndex,float _z){
        setLink(pointIndex);
        setZ(_z);
    }
    void setLink(ofIndexType pointIndex){
        link=pointIndex;
    }
    
    ofIndexType getLink(){
        return link;
    }
    void setZ(float _z){
        z=_z;
    }
    float getZ(){
        return z;
    }
    //touch
    bool isTouched(){
        return touched;
    }
    
    void touch(){
        touched=true;
    }
    void untouch(){
        touched=false;
    }
protected:
    bool touched=false;
    ofIndexType link;
    float z;
};

class agmultilinkline : public agline{
public:
    agmultilinkline(){
        
    }
    agmultilinkline(agline line){
        if(line.getzsort()){
            ip0=line.ip0;
            ip1=line.ip1;
            markZSortTrue();
        }else{
            ip0=line.ip1;
            ip1=line.ip0;
        }
   
    }
    
    float dx,dy;
   
    float getZmax(){
        return zmax;
    }
    float getZmin(){
    }
    bool isTouched(){
        return isTouch;
    }
    void touch(){
        isTouch=true;
    }
    void untouch(){
        isTouch=false;
    }
    bool addNearPoint(ofIndexType ip,float nearZ){
        if(nearPointLinks.size()==0||(nearPointLinks.size()%2==1 )){// 0 or odd
            
//        if(nearPointLinks.size()==0||(nearPointLinks.size() &1 )){// 0 or odd
            nearPointLink pointlink(ip,nearZ);
            nearPointLinks.push_back(pointlink);
            return true;
        }else{
            return false;
        }
        
        
        
    }
    bool isFilled(){//-1 for not filled
        if( nearPointLinks.size()==2 ){
           
            return true;
        }else{
            return false;
             //            cout<<"[WRONG LINE]ip0,ip1: "<<ip0<<":"<<ip1<<"ipa,ipb:"<<ipa<<","<<ipb<<" zmax,min"<<zmax<<":"<<zmin<<" za:zb"<<za<<":"<<zb<<endl;
        }
        
    }
    /**
     add dx dy thru sorted vec3f point always notice p0.z <= p1.z
     @param p0 lower vec3f point
     @param p1 higher vec3f point
     @return false when it's horizong line because the dxdy is no right()
     */
    bool adddXdY(ofVec3f p0,ofVec3f p1,float dH){
        setZmax(p1.z);//p1 always higherso set zmax first
        setZmin(p0.z);
        // for horizon
        if(p0.z==p1.z){
            setHorizon();
            setdXdY(p1.x-p0.x,p1.y-p1.y);
            return false;
        }
        if(p0.x==p1.x&&p0.y==p1.y){
            setVertical();
            setdXdY(0,0);
        }else{
            //get the high point and low point
            // there is no fall line any more so p1.z always higher than p0.z
            
            float H,divH;
            H=p1.z-p0.z;
            divH=1/H;
            if(p0.x==p1.x){
                setdXdY(0,(p1.y-p0.y)*divH*dH);
            }
            if(p0.y==p1.y){
                setdXdY((p1.x-p0.x)*divH*dH,0);
            }
            setdXdY((p1.x-p0.x)*divH*dH,(p1.y-p0.y)*divH*dH);
        }
        return true;
        
    }
    bool isCross( float planeatz){
        if(zmin<planeatz&&zmax>planeatz){
            // cout<<"get the cross line: "<<ofToString(pointHigher)<<","<<ofToString(pointLower)<<endl;
            return true;
        }
        return false;
    }
    agline getNextLine(float z){
        agline nextline;
        if(nextLineUsingUpperSide(z)){
            nextline.ip0= ip1;// use upper point to make next line
            nextline.ip1=getNextLink(ip0);
        }else{
            nextline.ip0= ip0;// use down side point to make next line
            nextline.ip1=getNextLink(ip1);
        }
        return nextline;
    }
        void refresh(){
        for(auto &npl:nearPointLinks){
            if(npl.isTouched()){

                npl.untouch();
             
            }
        }
    }
   protected:
    void setHorizon(){
        isHorizon=true;
    }
    void setVertical(){
        isVertical=true;
    }
    
    void setZmax(float z){
        zmax=z;
    }
    void setZmin(float z){
        zmin=z;
    }
    bool nextLineUsingUpperSide(float z){
        ofIndexType inear;// near point that next line use
        ofIndexType iline;// line point that next line use
        bool nextLineUseUpperSide;
        float nextpointZ=usingLinkZ;
        if(nextpointZ>=zmax){
            nextLineUseUpperSide=false;
        }else if(nextpointZ<=zmin){
            nextLineUseUpperSide=true;
        }else{
            if(nextpointZ<=z){
                nextLineUseUpperSide=true;
            }else{
                nextLineUseUpperSide=false;
            }
        }
        return nextLineUseUpperSide;
    }

    ofIndexType getNextLink(ofIndexType _linkFrom){
        touchUsedLink(_linkFrom);
        return getUntouchedLink();
    }
    ofIndexType getUntouchedLink(){
        for(auto &npl:nearPointLinks){
            if(npl.isTouched()==false){
                usingLinkZ=npl.getZ();
                npl.touch();
                return npl.getLink();
            }
        }
    }
    void touchUsedLink(ofIndexType linkFrom){
        for(auto &npl:nearPointLinks){
            if(npl.isTouched()==false){
                if(npl.getLink()==linkFrom);
                npl.touch();
                return;
            }
        }
        
    }

    void setdXdY(float dX,float dY){
        dx=dX;
        dy=dY;
    }
    vector<nearPointLink> nearPointLinks;
    //    vector<nearPointPair> nearPointPairs;
    float usingLinkZ;
    bool isTouch=false;
    bool isHorizon=false;
    bool isVertical=false;
    float zmax;
    float zmin;
    float za,zb;
};
