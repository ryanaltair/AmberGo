#pragma once

#include "ofMain.h"
#include "agline.h"

class agmllhelper{
    agmllhelper();
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
        ofVec3f returnPoint;
        ofVec3f zeroPoint;
        
        zeroPoint.x=zeroPoint.y=zeroPoint.z=0;
        // compare the upper point and the down point
        if(pointDown.z>pointUp.z){
            float tempz=pointDown.z;
            pointDown.z=pointUp.z;
            pointUp.z=tempz;
        }
        //check if the line cross the z plane
        if(pointUp.z>z||pointDown.z<z){
            
        }else{
            
            return zeroPoint;
        }
        if(pointUp.x==pointDown.x&&pointUp.y==pointDown.y){
            returnPoint.x=pointDown.x;
            returnPoint.y=pointDown.y;
        }else{
            float zLength=(pointUp.z-pointDown.z);
            float z1Length=1/zLength;
            float kx=(pointUp.x-pointDown.x)*z1Length;
            returnPoint.x=(z-pointUp.z)*kx+pointUp.x;
            float ky=(pointUp.y-pointDown.y)*z1Length;
            returnPoint.y=(z-pointUp.z)*ky+pointUp.y;
        }
        returnPoint.z=z;
        return returnPoint;
    }
    
    
}
