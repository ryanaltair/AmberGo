#pragma once

#include "ofMain.h"

class agline{
public:
    agline(){
    }
    ofIndexType ip0,ip1;
    
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

class agmultilinkline : public agline{
public:
    agmultilinkline(){
        
    }
    agmultilinkline(agline line){
        if(line.getzsort()){
            ip0=line.ip0;
            ip1=line.ip1;
        }else{
            ip0=line.ip1;
            ip1=line.ip0;
        }
        markZSortTrue();
    }
    void set(ofIndexType ip0Value,ofIndexType ip1Value,ofIndexType ipaValue,float znear){
        ip0=ip0Value;
        ip1=ip1Value;
        addNearPoint(ipaValue,znear);
    }
    void set(ofIndexType ip0Value,ofIndexType ip1Value,ofIndexType ipaValue,ofIndexType ipbValue,float znearA,float znearB){
        ip0=ip0Value;
        ip1=ip1Value;
        addNearPoint(ipaValue,znearA);
        addNearPoint(ipbValue,znearB);
    }
    ofIndexType ipa,ipb;
    float dx,dy;
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
        if(isVoid ==true){
            ipa=ip;
            ipb=ip;
            za=nearZ;
            zb=nearZ;
            isVoid=false;
            return true;//
        }else{
            if(ipa==ipb&&ipb!=ip){
                ipb=ip;
                zb=nearZ;
                return true;
            }else{
                return false;
            }
        }
    }
    bool isFilled(){
        if(ipa==ipb){
            cout<<"[WRONG LINE]ip0,ip1: "<<ip0<<":"<<ip1<<"ipa,ipb:"<<ipa<<","<<ipb<<" zmax,min"<<zmax<<":"<<zmin<<" za:zb"<<za<<":"<<zb<<endl;
            return false;
        }else{
            return true;
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
    bool nextLineUsingUpperPoint(bool isSideNearA,float z){
        ofIndexType inear;// near point that next line use
        ofIndexType iline;// line point that next line use
        bool nextLineUseUpperSide;
        float nextpointZ;
        if(isSideNearA==true){
        nextpointZ=za;
            inear=ipa;
        }else{
        nextpointZ=zb;
            inear=ipb;
        }
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
protected:
    void setdXdY(float dX,float dY){
        dx=dX;
        dy=dY;
    }
    bool isVoid=true;
    bool isTouch=false;
    bool isHorizon=false;
    bool isVertical=false;
    float zmax;
    float zmin;
    float za,zb;
};
