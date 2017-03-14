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
        vector<ofIndexType>().swap(linelist);// use for really clean memory
        vector<ofVec3f>().swap(pointlist);
        vector<bool>().swap(linehorizonlist);// use for really clean memory
        vector<ofIndexType>().swap(nearpointlist);
        vector<float>().swap(dXdYlist);
        horizonFacetHeightlist.clear();
        linelist.clear();
        pointlist.clear();
        linehorizonlist.clear();// is this line horizon true horizon // add with addnewline work with adddXdY
        linehorizonlist.clear();//{pa pb} the index point to point list
        dXdYlist.clear();// add with addnewline work with adddXdY
        linecopymap.clear();
        //
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
            multilinklinelist[findindex].addNearPoint(ipnear);
            return false;
        }else{
            agmultilinkline newmultiline(sortline);
            newmultiline.addNearPoint(ipnear);
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
    
    ofVec3f getXY(agmultilinkline mlline,float divdH,float z){
        ofVec3f pH;
        ofVec3f pL;
        pL= pointlist[mlline.ip0];
        pH= pointlist[mlline.ip1];
        
        ofVec3f returnpoint;
        float h=z-pL.z;
        float divdHxh=divdH*h;
        returnpoint.x=pL.x+mlline.dx*divdHxh;
        returnpoint.y=pL.y+mlline.dy*divdHxh;
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
        for(int i=startflag;i<multilinklinelist.size();i+=1){
            // we don't touch anything that already touched
            if(multilinklinelist[i].isTouched()){
                // cout<<"it was touched"<<endl;
                continue;
            }else{
                // cout<<"it untouched "<<ip0<<endl;
            }
            multilinklinelist[i].touch();
            // we don't touch when z>zmax
            if(z>multilinklinelist[i].getZmax()){
                continue;
            }
            if( isPointPlaneCross(pointlist[multilinklinelist[i].ip0], pointlist[multilinklinelist[i].ip1], z)==true){
                return i;
            }
        }
        //  cout<<"we find nothing any more so we quit"<<endl;
        return multilinklinelist.size();
    }
    vector<ofVec3f> pointlist;// get the real point p=pointlist[ip]
    vector<agmultilinkline> multilinklinelist;// to replace linelist and nearpoint list
    map<agline,ofIndexType> mllcopymap;// use for searchline , for great speed
    vector<ofIndexType> linelist;// hlod {ipL ipH} the index point to point list add with addnewline() and pL will be lower
    map<agline,ofIndexType> linecopymap;// use for searchline , for great speed
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline() work with adddXdY
    vector<ofIndexType> nearpointlist;//{ipa ipb} the index refer point list add with addnewline() and addoldline
    vector<float> dXdYlist;// init with addFacet() work with adddXdY()
    vector<int> linetypelist;//init with addFacet() work with adddXdY()
    vector<float> touchedlist;//init with addFacet() work with adddXdY()
    vector<float> horizonFacetHeightlist;// hold every height that get horizong facet/triangle
    ofVec3f scale;
    ofVec3f scaleMax;
    ofVec3f scaleMin;
    float dH=0.01;
protected:
    bool isPointPlaneCross(ofVec3f pointLower, ofVec3f  pointHigher, float planeatz){
        if(pointLower.z<planeatz&&pointHigher.z>planeatz){
            // cout<<"get the cross line: "<<ofToString(pointHigher)<<","<<ofToString(pointLower)<<endl;
            return true;
        }
        return false;
    }
    int zero=0;
};
