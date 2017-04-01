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
    bool cleanOldLayers(){
        vector<float>().swap(alllayertesstsHeight);
        
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
  
    vector<ofPath> alllayertests;
    vector<float> alllayertesstsHeight;
    //needing flag
    bool isThreadEnd=false;// true when everything is done
    bool needLoad=false;
    bool needAllSlice=false;

    float allthickness=0.06;
    bool isSliceChanged=false;
    bool isModelReadySlice=false;
    // work in thread
    bool isAllSliced(){
        return isAllSliceDone;
    }
    void setScaleFactor(ofVec3f sf){
        mll.setScaleFactor(sf);
    }
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
    void stepAllSlice(){
        easyLogTime.from("all slice");
        vector<ofPath> layers;
        vector<float> layerHeights;
        float slicez;
        float outputZ=0;
        int an=0;
        float slicethickness=mll.getRealthickness(allthickness);
        float outputthickness=mll.getRealZ(allthickness);
        for(slicez=slicethickness;slicez<mll.meshScale.z;slicez+=slicethickness){
           
            
            ofPath p=mll.layerAt(slicez);
            layers.push_back(p);
             outputZ+=allthickness;
            
            layerHeights.push_back( outputZ);
//            cout<<"z slice at :"<<slicez<<" new : "<<outputZ<<endl;//use to check z
            an++;
            
        }
        alllayertests.swap(layers);
        alllayertesstsHeight.swap(layerHeights);
        isAllSliceDone=true;
        cout<<"layers:"<<layers.size()<<":"<<an<<endl;
        easyLogTime.to("all slicer");
    }
     ofPath layertest; //the output layer path
    ofxAssimpModelLoader assimpmodel;
    
     easyLogTimer easyLogTime;
    bool isAllSliceDone=false;  
};
