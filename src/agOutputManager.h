#pragma once
#include "ofxImageSequenceRecorder.h"
#include "ofMain.h"
class agOutputManager{
public:
    agOutputManager(){
        threadImageSaver.setPrefix(ofToDataPath("fabfiles/A")); // this directory must already exist
        threadImageSaver.setFormat("png"); // png is really slow but high res, bmp is fast but big, jpg is just right
        
    }
    void init(){
        Annnn=1;
         isBegin=true;
         isFinish=false;
          baseCount=4;
    }
    void saveSetup(ofPixels pixels){
        string se="SE1300";// for 13s
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(se);
        
        string su="SU0006";
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(su);
        
        string sd="SD0008";
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(sd);
        
        string sb="SB0008";
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(sb);
        
        string sl="SL1000";
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(sl);
        
        string sf="SF0056";
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(sf);
        
        string sv="SV0400";//04.00mm
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(sv);
    }
    void saveImage(ofPixels pixels,float z){
        cout<<"save image"<<endl;
        int intz=z*100;
        string strz=ofToString(intz, 5, '0');
        string picname;
        if(isBegin==true){
            isBegin=false;
            saveSetup(pixels);
        }
        if(baseCount>0){
            baseCount--;
            picname="B"+strz;
        }else if(isFinish==true){
            picname="F"+strz;
        }else{
            picname="0"+strz;
        }
        pixelsWaiting.push_back(pixels);
        strWaiting.push_back(picname);
        addFrameToThread();
        
    }
    
    bool check(){
        if(threadImageSaver.isThreadRunning()){
            return false;
        }
        if(pixelsWaiting.size()>0){
            addFrameToThread();
            return false;
        }
        return true;
    }
    
    void end(){
        threadImageSaver.waitForThread();
    }
    
 
    void setLastPic(){
    isFinish=true;
    }
private:
    vector<ofPixels> pixelsWaiting;
    vector<string> strWaiting;
    
    void addFrameToThread(){
        if(threadImageSaver.isThreadRunning()){
            return;
        }
        for(int i=0; i<pixelsWaiting.size();i++){
            threadImageSaver.addFrame(pixelsWaiting[i],Annnn,strWaiting[i]);
            Annnn++;
        }
        vector<ofPixels> pixelsWaitingVoid;
        vector<string> strWaitingVoid;
        pixelsWaiting.swap(pixelsWaitingVoid); // clear mem
        strWaiting.swap(strWaitingVoid);// clear mem
        threadImageSaver.startThread();
    }
    int baseCount=4;
    bool isBegin=true;
    bool isFinish=false;
    int Annnn=1;
    ofxImageSequenceRecorder threadImageSaver; // use for save image
};
