#pragma once

#include "ofMain.h"
class agTriangle{
public:
    agTriangle(){
    }
    agTriangle(ofVec3f _pL,ofVec3f _pM,ofVec3f _pH){
        p0.x=_pL.x;
        p0.y=_pL.y;
        p1.x=_pM.x;
        p1.y=_pM.y;
        p2.x=_pH.x;
        p2.y=_pH.y;
    }
    ofVec2f p0,p1,p2;
};
class agfacet{
public:
    agfacet(){
    }
    void setFromTri(ofVec3f _pL,ofVec3f _pM,ofVec3f _pH){
        pL=_pL;
        pM=_pM;
        pH=_pH;
    }
    agTriangle getTriangle(){
        return agTriangle(pL,pM,pH);
    }
    ofPath getPath(){
        ofPath path;
        path.moveTo(pL.x,pL.y);
        path.lineTo(pM.x,pM.y);
        path.lineTo(pH.x,pH.y);
        path.close();
        return path;
    }
    void setNormal(ofVec3f nL,ofVec3f nM,ofVec3f nH){
        normalL=nL;
        normalM=nM;
        normalH=nH;
    }
    ofVec3f getFaceNormal(){
        return normalF;
    }
    float getGradiant(){
        return normalF.z;
    }
    void setFaceNormal(ofVec3f nf){
        normalF=nf;
    }
    float getGradientX(){
        ofVec3f p=(normalL+normalM+normalH)/3;
        return p.x;
    }
    float getGradientY(){
        ofVec3f p=(normalL+normalM+normalH)/3;
        return p.y;
    }
    float getZmin(){
        float zmin=pL.z;
        if(zmin<pM.z){
            zmin=pM.z;
        }
        if(zmin<pH.z){
            zmin=pH.z;
        }
        return zmin;
    }
    ofVec3f pL,pM,pH;
    bool operator < ( const agfacet& other) const
    {
        if ( pL.z == other.pL.z ) {
            if ( pM.z == other.pM.z ) {
                
                return pH.z < other.pH.z;
            }
   
            return pM.z < other.pM.z;
        }
        
        return  pL.z < other.pL.z;
    }
    
protected:
    
    ofVec3f normalL,normalM,normalH,normalF;
};
