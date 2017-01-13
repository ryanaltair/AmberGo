#pragma once


#include "ofThread.h"
#include "ofxAssimpModelLoader.h"
#include "agmll.h"
/// This is the agSliceManager created by extending ofThread.
/// It contains slice that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class agSliceManager: public ofThread
{
public:
    /// Create a agSliceManager and initialize the member
    /// variable in an initialization list.
    agSliceManager(){
    }
    
    /// Start the thread.
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
        mll.setup(assimpmodel.getMesh(0));
        easyLogTime("load model end");
        startThread();
        ofResetElapsedTimeCounter();
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
    float layertestZ;
    void stop()
    {
        stopThread();
    }
    
    /**
     thread contains load or sliceat job
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
                    stepUpdate();
                    needLoad=false;
                    
                }
                
                //easyLogTime("need we Slice?");
                if(needSliceAt>=0){
                    if(isModelReadySlice==true){
                        stepSliceAt();
                        needSliceAt=-1;
                    }
                }
                //easyLogTime("all we done ?");
                isThreadEnd=true;
                
                unlock();
            }
        }
    }
    
  
    ofxAssimpModelLoader assimpmodel;
    
    agmll mll;
    ofPath layertest;
    //needing flag
    bool isThreadEnd=false;// true when everything is done
    
    bool needLoad=false;
    float needSliceAt=-1;// -1 means no need
    bool isSliceChanged=false;
    float isModelReadySlice=false;
    // work in thread
    
    
    
protected:
    //mll load
    void stepLoad(){
            }
    //mll update loop
    
    void stepUpdate(){
        
        float timeLast=ofGetElapsedTimef();
        mll.calcaulateModel();
        float timeWaste=ofGetElapsedTimef()-timeLast;
        isModelReadySlice=true;
        easyLogTime("load model end");
        cout<<"ready for slice"<<endl;
        
        cout<<"linelist.size:"<<mll.linelist.size()<<endl;
        cout<<"pointlist:"<<mll.pointlist.size()<<endl;
        
    }
    void stepSliceAt(){
        
        //easyLogTimeFrom("slice");
        // cout<<"we are slicing from"<<ofToString(ofGetElapsedTimef()) ;
        layertest=mll.layertestat(needSliceAt);
        //  cout<<" to "<<ofToString(ofGetElapsedTimef()) <<endl;
        //easyLogTimeTo("slice");
        
        isSliceChanged=true;
        //cout<<"we just slice  at"<<needSliceAt<<endl;
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
        cout<<title<<"take:"<<ofToString(timetake) <<" senconds "<<endl;
    }
    void easyPercent(int percent){
        cout<<"------";
        
        cout<<"progress:"<<ofToString(percent) <<"% |" <<ofToString(ofGetElapsedTimef())<<" second"<<endl;
        
    }
    float timekeep;
    
};
