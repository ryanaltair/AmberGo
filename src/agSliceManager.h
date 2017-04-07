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
        isThreadEnd=false;
        isModelReadySlice=false;
        needLoad=true;
        easyLogTime.from("load model");

        ofxAssimpModelLoader assimpmodel;
        assimpmodel.clear();
        cout<<"-----modelpath:"<<path<<endl;
        assimpmodel.loadModel(path);
        cout<<"the mesh count:"<<assimpmodel.getMeshCount()<<endl;
        assimpmodel.disableTextures();
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        ofMesh meshbuffer;
        for(int i=0;i<assimpmodel.getMeshCount();i++){
            meshbuffer.append(assimpmodel.getMesh(i));
        }
          assimpmodel.clear();
         mll.load(meshbuffer);
        isAllSliceDone=false;
        easyLogTime.to("load model");
        progressZ=0;
        startThread();
        alllayertests.clear();
    }
    bool allSlice(float layerthickness){
        if(isThreadRunning()){
            return false;
        }
        if(isModelReadySlice==false){
            //if mesh have model and get merged?
            
                needLoad=true;
                cout<<"now we loading"<<endl;
                startThread();
            
            
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
    float getSliceProgress(){
       
        float pz;
        float mz;
        lock();
        pz=progressZ;
        mz=maxZ;
        unlock();
        if(pz==0){
            return 0;
        }
        
        if(mz-pz<5){
        return 1;
        }
        
        float progress= pz / mz;
        return progress;
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
               
                unlock();
            }
            
                if(needAllSlice==true){
                    if(isModelReadySlice==true){
                        
                        
                        stepAllSlice();
                        needAllSlice=false;
                        
                    }
                }
                isThreadEnd=true;
            
                
        }
    }
    
    ofMesh getMergedMesh(){
        cout<<"mergedMesh"<<mll.mergedMesh.getIndices().size()<<":"<<mll.mergedMesh.getVertices().size()<<endl;
        ofMesh returnMesh=mll.mergedMesh;
        ofMesh meshVoid;
//        mll.mergedMesh=meshVoid;
        return returnMesh;
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
    void setModelOffset(ofVec3f mo){
        mll.setPositionOffset(mo);
        startZ=mo.z;
    }
protected:
    //mll load
    void stepLoad(){
        easyLogTime.from("prepare model");
        mll.prepareModel();
        isModelReadySlice=true;
        easyLogTime.to("prepare model");
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
        float slicethickness=mll.getSliceZ(allthickness);
        float sliceStartZ=mll.meshMin.z+mll.getSliceZ(allthickness-startZ);
        float realZmax=mll.getRealZ(mll.meshScale.z)+startZ;
        cout<<"slice Z min max scale "<< mll.meshMin.z<<":"<<mll.meshMax.z<<":"<<mll.meshScale.z<<endl;
               slicez=sliceStartZ;
        cout<<"slice Z for real min max scale "<<mll.getRealZ(mll.meshMin.z)<<":"<<mll.getRealZ(mll.meshMax.z)<<":"<<mll.getRealZ(mll.meshScale.z)<<endl;
        slicez=sliceStartZ;
        cout<<"real z from   "<<slicez<<" to "<<realZmax <<endl;
        lock();
        maxZ=realZmax/allthickness;
        int calloutMax=maxZ/1000;
        int lastCallOut=0;
        unlock();
        if(allthickness<realZmax){
                for(outputZ=allthickness ;outputZ<realZmax; outputZ+=allthickness){
                    
                    slicez+=slicethickness;
                    ofPath p=mll.layerAt(slicez);
                    layers.push_back(p);
                    layerHeights.push_back( outputZ);
                    if(layers.size()-lastCallOut>calloutMax){
                      lock();
                    progressZ=layers.size();
                        lastCallOut=progressZ;
                    unlock();
                    }
                    lock();
                    progressZ=layers.size();
                    lastCallOut=progressZ;
                    unlock();
        //            cout<<"z slice at :"<<slicez<<" new : "<<outputZ<<endl;//use to check z
                    an++;
                    
                }
        }else{
            ofPath p;
            layers.push_back(p);
            layerHeights.push_back(0);
        }
        
        
        alllayertests.swap(layers);
        alllayertesstsHeight.swap(layerHeights);
        isAllSliceDone=true;
        cout<<"layers:"<<layers.size()<<":"<<an<<endl;
        easyLogTime.to("all slicer");
    }
     ofPath layertest; //the output layer path

    float startZ=10;//allSliceFromHere;
     easyLogTimer easyLogTime;
    bool isAllSliceDone=false;
    int progressZ=0;
    int maxZ=1;
};
