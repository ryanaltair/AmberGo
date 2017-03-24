#pragma once


#include "ofThread.h"
#include "ofxAssimpModelLoader.h"
#include "agmll.h"
#include "agEasyTimeLog.h"
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
        easyLogTime.from("load model");
        ofMesh meshbuffer;
        for(int i=0;i<assimpmodel.getMeshCount();i++){
            meshbuffer.append(assimpmodel.getMesh(i));
        }
         mll.load(meshbuffer);
        isAllSliceDone=false;
        easyLogTime.to("load model");
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
                isThreadEnd=true;
                unlock();
            }
        }
    }
    
    ofMesh getMergedMesh(){
        return mll.mergedMesh;
    }
    void cleanMesh(){
        assimpmodel.clear();
    }
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
        easyLogTime.from("load model");
        mll.prepareModel();
        isModelReadySlice=true;
        easyLogTime.to("load model");
    }
    /**
     use mll slice at testlayeratZ
     */
    void stepSliceAt(){
        layertest=mll.layerAt(needSliceAt); 
        isSliceChanged=true;
    }
    void stepAllSlice(){
        easyLogTime.from("all slice");
        vector<ofPath> layers;
        
        float z;
        int an=0;
        for(z=allthickness;z<mll.meshScale.z;z+=allthickness){
            cout<<"z"<<z<<endl;//use to check z
            ofPath p=mll.layerAt(z);
            layers.push_back(p);
            alllayertesstsHeight.push_back(z);
            an++;
        }
        alllayertests=layers;
        isAllSliceDone=true;
        cout<<"layers:"<<layers.size()<<endl;
        easyLogTime.to("all slicer");
    }
   
    ofxAssimpModelLoader assimpmodel;
    
     easyLogTimer easyLogTime;
    
};
