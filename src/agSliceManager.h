#pragma once


#include "ofThread.h"
#include "ofxAssimpModelLoader.h"
#include "agmll.h"

class agSliceManager: public ofThread
{
public:
   
    agSliceManager(){
    }
    /// which Start the thread.
    void loadModel(string path)
    {
        assimpmodel.clear();
        cout<<"-----modelpath:"<<path<<endl;
        assimpmodel.loadModel(path);
        cout<<"the mesh count:"<<assimpmodel.getMeshCount()<<endl;
        assimpmodel.disableTextures();
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        isThreadEnd=false;
        isModelReadySlice=false;
        needLoad=true;
        easyLogTime("load model start");
        ofMesh meshbuffer;
        for(int i=0;i<assimpmodel.getMeshCount();i++){
            meshbuffer.append(assimpmodel.getMesh(i));
        }
         mll.setup(meshbuffer);
        isAllSliceDone=false;
        easyLogTime("load model end");
        startThread();
        ofResetElapsedTimeCounter();
        alllayertests.clear();
    }
    bool sliceAt(float zheight)
    {
        if(isModelReadySlice==false){
            //if mesh have model and get merged?
            
            if(isThreadRunning()==false){
                needLoad=true;
                cout<<"now we loading"<<endl;
                startThread();
            }
            
            return false;
        }else{//if(isModelReadySlice==true){
            isThreadEnd=false;
            isSliceChanged=false;
            layertestZ=zheight;
            needSliceAt=zheight;
            startThread();
            return true;
        }
    }
    bool allSlice(float layerthickness){
        if(isModelReadySlice==false){
            //if mesh have model and get merged?
            if(isThreadRunning()==false){
                needLoad=true;
                cout<<"now we loading"<<endl;
                startThread();
            }
            
            return false;
        }else{//if(isModelReadySlice==true){
            isThreadEnd=false;
            isSliceChanged=false;
            allthickness=layerthickness;
            needAllSlice=true;
            startThread();
            return true;
        }
    }
    float layertestZ;
    /**
     stop the thread mamually
     */
    void stop()
    {
        stopThread();
    }
    
    /**
     thread contains load or sliceAt job
     */
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            if(isThreadEnd==true){
                stopThread();
            }
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                // step merge: the mesh
                if(needLoad==true){
                    stepLoad();
                    needLoad=false;
                }
                //easyLogTime("need we Slice?");
                if(needSliceAt>=0){
                    if(isModelReadySlice==true){
                        stepSliceAt();
                        needSliceAt=-1;
                    }
                }
                if(needAllSlice==true){
                    if(isModelReadySlice==true){
                        stepAllSlice();
                        needAllSlice=false;
                    }
                }
                //easyLogTime("all we done ?");
                isThreadEnd=true;
                unlock();
            }
        }
    }
    
    
    ofxAssimpModelLoader assimpmodel;
    
    agmll mll;// the work slicer
    ofPath layertest; //the output layer path
    vector<ofPath> alllayertests;
    vector<float> alllayertesstsHeight;
    //needing flag
    bool isThreadEnd=false;// true when everything is done
    
    bool needLoad=false;
    float needSliceAt=-1;// -1 means no need
    bool needAllSlice=false;
    bool isAllSliceDone=false;
    float allthickness=0.06;
    bool isSliceChanged=false;
    bool isModelReadySlice=false;
    // work in thread
protected:
    //mll load
    void stepLoad(){
        easyLogTimeFrom("load model");
        mll.calcaulateModel();
        isModelReadySlice=true;
        easyLogTimeTo("load model");
    }
    /**
     use mll slice at testlayeratZ
     */
    void stepSliceAt(){
        layertest=mll.layerAt(needSliceAt); 
        isSliceChanged=true;
    }
    void stepAllSlice(){
        easyLogTimeFrom("all slice");
        vector<ofPath> layers;
        
        float z;
        for(z=allthickness;z<mll.meshScale.z;z+=allthickness){
            layers.push_back(mll.layerAt(z)); 
            alllayertesstsHeight.push_back(z);
        }
        alllayertests=layers;
        isAllSliceDone=true;
        cout<<"layers:"<<layers.size()<<endl;
        easyLogTimeTo("all slicer");
    }
    void easyLogTime(string title){
        cout<<"------";
        cout<<title<<":"<<ofToString(ofGetElapsedTimef()) <<" senconds "<<endl;
    }
    void easyLogTimeFrom(string title){
        cout<<"------";
        timekeep=ofGetElapsedTimef();
        cout<<title<<" from:"<<ofToString(timekeep) <<" senconds "<<endl;
    }
    void easyLogTimeTo(string title){
        cout<<"------";
        float timetake=ofGetElapsedTimef()-timekeep;
        timekeep=ofGetElapsedTimef();
        cout<<title<<"  to:"<<ofToString(timekeep) <<" senconds "<<endl;
        cout<<"------";
        cout<<title<<"take:"<<ofToString(timetake) <<" senconds "<<endl;
    }
    void easyPercent(int percent){
        cout<<"------";
        cout<<"progress:"<<ofToString(percent) <<"% |" <<ofToString(ofGetElapsedTimef())<<" second"<<endl;
        
    }
    float timekeep;
    
};
