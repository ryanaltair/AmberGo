#pragma once


#include "ofThread.h"
#include "ofxAssimpModelLoader.h"
#include "agmll.h"
/// This is a simple example of a agSliceManager created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class agSliceManager: public ofThread
{
public:
    /// Create a agSliceManager and initialize the member
    /// variable in an initialization list.
    agSliceManager(): count(0), shouldThrowTestException(false)
    {
    }

    /// Start the thread.
    void start(string path)
    {
        cout<<"-----modelpath:"<<path<<endl;
        assimpmodel.loadModel(path);
        assimpmodel.disableTextures();
        meshmodel=assimpmodel.getMesh(0);
        meshmodel.disableTextures();
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        isThreadEnd=false;
        isModelReadySlice=false;
        needMerge=true;
        startThread();
        ofResetElapsedTimeCounter();
    }
    bool sliceAt(float zheight)
    {
        if(isModelReadySlice==false){
            //if mesh have model and get merged?
            if(meshmodel.getNumIndices()>0&&meshmodel.getNumIndices()>meshmodel.getNumVertices()){
                
                if(isThreadRunning()==false){
                    needLoad=true;
                    cout<<"now we loading"<<endl;
                    startThread();
                }
            }
            return false;
        }else{//if(isModelReadySlice==true){
        isThreadEnd=false;
            isSliceChanged=false;
        needSliceAt=zheight;
        startThread();
            return true;
        }
    }
    
    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
    }

    /// Our implementation of threadedFunction.
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
                stepMerge();
                
                
                //step load:
                //easyLogTime("need we load?");
                stepLoad();
                //
                //easyLogTime("need we update?");
                stepUpdate();
                
                //easyLogTime("need we Slice?");
                stepSliceAt();
                
                //easyLogTime("all we done ?");
                isThreadEnd=true;

                unlock();

               

                if(shouldThrowTestException > 0)
                {
                    shouldThrowTestException = 0;

                    // Throw an exception to test the global ofBaseThreadErrorHandler.
                    // Users that require more specialized exception handling,
                    // should make sure that their threaded objects catch all
                    // exceptions. ofBaseThreadErrorHandler is only used as a
                    // way to provide better debugging / logging information in
                    // the event of an uncaught exception.
                    throw Poco::ApplicationException("We just threw a test exception!");
                }
            }
            else
            {
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }

    /// This drawing function cannot be called from the thread itself because
    /// it includes OpenGL calls (ofDrawBitmapString).
    void draw()
    {
        stringstream ss;

        ss << "I am a slowly increasing thread. " << endl;
        ss << "My current count is: ";

        if(lock())
        {
            // The mutex is now locked and the "count"
            // variable is protected.  Time to read it.
            ss << count;

            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }

        ofDrawBitmapString(ss.str(), 50, 56);
    }

    // Use unique_lock to protect a copy of count while getting a copy.
    int getCount()
    {
        unique_lock<std::mutex> lock(mutex);
        return count;
    }

    void throwTestException()
    {
        shouldThrowTestException = 1;
    }

    ofxAssimpModelLoader assimpmodel;
    ofMesh meshmodel;
    agmll mll;
    ofPath layertest;
    //needing flag
    bool isThreadEnd=false;// true when everything is done
    bool needLoad=false;
    bool needMerge =false;//
    bool needSlice=false;
    
    float needSliceAt=-1;// -1 means no need
    bool isSliceChanged=false;
    float isModelReadySlice=false;
    // work in thread
    
    
    //merge
    void stepMerge(){
    
        if(needMerge==true){
            easyLogTime("merge start");
            meshmodel.mergeDuplicateVertices();
            needMerge=false;
            needLoad=true;
            easyLogTime("merge end");
            
        }
       
    }
    //mll load
    void stepLoad(){
        if(needLoad==true){
            easyLogTime("load model start");
            mll.setup(meshmodel);
            needLoad=false;
            needSlice=true;
            
            easyLogTime("load model end");
        }
        
    }
    //mll update loop
    
    void stepUpdate(){
        if(needSlice==true){
            
            easyLogTime("load model start");
            while(mll.isdXdYlistfilled<100){
                mll.update();
                easyPercent(mll.isdXdYlistfilled);
            }
            isModelReadySlice=true;
            easyLogTime("load model end");
            cout<<"ready for slice"<<endl;
            needSlice=false;
        }
    }
    void stepSliceAt(){
        
        if(needSliceAt>=0){
             cout<<"we are slicing  at"<<needSliceAt<<endl;
            layertest=mll.layertestat(needSliceAt);
            isSliceChanged=true;
        cout<<"we just slice  at"<<needSliceAt<<endl;
        }
        needSliceAt=-1;
    }
protected:
    // A flag to check and see if we should throw a test exception.
    Poco::AtomicCounter shouldThrowTestException;
    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    //
    // Note, if we simply want to count in a thread-safe manner without worrying
    // about mutexes, we might use Poco::AtomicCounter instead.
    int count;
    
    //
    void easyLogTime(string title){
        cout<<"------";

    cout<<title<<":"<<ofToString(ofGetElapsedTimef()) <<" senconds "<<endl;
    }
    void easyPercent(int percent){
        cout<<"------";
        
        cout<<"progress:"<<ofToString(percent) <<"% |" <<ofToString(ofGetElapsedTimef())<<" second"<<endl;
        
    }
    

};
