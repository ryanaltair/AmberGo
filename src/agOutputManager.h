#pragma once
#include "agImageSequenceRecorder.h"
#include "ofMain.h"
typedef struct {
    string fileName;
    ofPixels image;
} ImageFiles;
typedef struct {
    string fileName;
    ofPath path;
} SVGFiles;

class agOutputManager{
public:
    agOutputManager(){
        threadImageSaver.setPrefix(ofToDataPath("fabfiles/A")); // this directory must already exist
        
        if(usingSVG==true){
            threadImageSaver.setFormat("svg"); // svg is so fast
        }else{
            threadImageSaver.setFormat("png"); // png is really slow but high res, bmp is fast but big, jpg is just right
        }
    }
    const bool usingSVG=true;
    void startOutput(string _saveplace){
        setPrefix(_saveplace);
        uploadEnd=false;
    }
    void init(){
        Annnn=1;
        isBegin=true;
        isFinish=false;
        baseCount=4;
        outputCount=0;
        picSaved=0;
        threadImageSaver.init();
    }
    int getPicSavedCount(){
        picSaved+=threadImageSaver.getSavedCount();
        return picSaved;
    }
    void setPrint(float _exposedSeconds,int _upspeed,int _downspeed,float _baseExposedSeconds,int _baseCount,int _outputCount,float _quickLiftHeight ){
        exposedSeconds=_exposedSeconds;
        upspeed=_upspeed;
        downspeed=_downspeed;
        baseExposedSeconds=_baseExposedSeconds;
        baseCount=_baseCount;
        outputCount=_outputCount;
        quickLiftHeight=_quickLiftHeight;
    }
    void saveSetup(ofPath path){
        //SE
        int ise=exposedSeconds*100;
        addPicSetupToWaiting(path,'E',ise);
        //SU
        addPicSetupToWaiting(path,'U',upspeed);
        //SD
        addPicSetupToWaiting(path,'D',downspeed);
        //SB
        int basemulti=baseExposedSeconds/exposedSeconds;
        cout<<"basemulti"<<basemulti<<"="<<baseExposedSeconds<<":"<<exposedSeconds<<endl;
        addPicSetupToWaiting(path,'B',basemulti);
        //SF
        //        addPicSetupToWaiting(path,'L',outputCount);
        addPicSetupToWaiting(path,'F',3000);
        //SL
        //        addPicSetupToWaiting(path,'L',outputCount);
        addPicSetupToWaiting(path,'L',3000);
        //SV
        int ql=quickLiftHeight*100;
        addPicSetupToWaiting(path,'V',ql);
    }
    void saveSetup(ofPixels pixels){
        //SE
        int ise=exposedSeconds*100;
        addPicSetupToWaiting(pixels,'E',ise);
        //SU
        addPicSetupToWaiting(pixels,'U',upspeed);
        //SD
        addPicSetupToWaiting(pixels,'D',downspeed);
        //SB
        int basemulti=baseExposedSeconds/exposedSeconds;
        addPicSetupToWaiting(pixels,'B',basemulti);
        //SL
        addPicSetupToWaiting(pixels,'L',outputCount);
        //SV
        int ql=quickLiftHeight*100;
        addPicSetupToWaiting(pixels,'V',ql);
    }
    void saveImage(ofPath path,float z){
        
        cout<<"save image layer at :"<<z<<endl;
        
        if(isBegin==true){
            isBegin=false;
            saveSetup(path);
        }
        int intz=z*100;
        string strz=ofToString(intz, 5, '0');
        string picname;
        
        if(baseCount>0){
            baseCount--;
            picname="B"+strz;
        }else if(isFinish==true){
            picname="F"+strz;
        }else{
            picname="0"+strz;
        }
        addPicToWaiting(path,picname);
//        tryAddFrameToThread();
        
    }
    void saveImage(ofPixels pixels,float z){
        cout<<"save image layer at :"<<z<<endl;
        if(isBegin==true){
            isBegin=false;
            saveSetup(pixels);
        }
        int intz=z*100;
        string strz=ofToString(intz, 5, '0');
        string picname;
        
        if(baseCount>0){
            baseCount--;
            picname="B"+strz;
        }else if(isFinish==true){
            picname="F"+strz;
        }else{
            picname="0"+strz;
        }
        addPicToWaiting(pixels,picname);
//        tryAddFrameToThread();
        
    }
    
    
    bool checkEnd(){
        if(threadImageSaver.isThreadRunning()){
            return false;
        }
        
        if(checkWaitingVoid()){
            if(isFinish==true){
                if(needAlert==true){
                    needAlert=false;
             return true;
                }
                return false;
            }
        }else{
            tryAddFrameToThread();
        }
        return false;
        
    }
    
    void end(){
        threadImageSaver.waitForThread();
    }
    
    
    void setLastPic(){
        needAlert=true;
        isFinish=true;
    }
        int Annnn=1;
private:
    vector<ImageFiles> pixelsWaiting;
    
    bool checkWaitingVoid(){
        if(usingSVG==true){
            if(pathToAdd>0){
                return false;
            }
        }else{
            if(pixelsWaiting.size()>0){
                return false;
            }
            
        }
        return true;
    }
    
    
    void setPrefix(string saveplace){
        cout<<"now we get fisaveplace :"<<saveplace<<endl;
        string p=saveplace+"fabfiles/";
        cout<<"now we create file directory:"<<p<<endl;
        ofDirectory::createDirectory(p,false);
        p+="A";
        threadImageSaver.setPrefix(p); // this directory must already exist
    }
    
    bool tryAddFrameToThread(){
        if(threadImageSaver.isThreadRunning()){
            return false;
        }
        if(usingSVG==true){
            int psize=pathToAdd;
            cout<<"try add frame as svg"<<endl;
            for(int i=0; i<psize;i++){
                pathToAdd--;
            }
        }else{
            for(int i=0; i<pixelsWaiting.size();i++){
                cout<<"try add frame as png"<<endl;
                threadImageSaver.addFrame(pixelsWaiting[i].image,Annnn,pixelsWaiting[i].fileName);
                Annnn++;
            }
            
            vector<ImageFiles> pixelsWaitingVoid;
            pixelsWaiting.swap(pixelsWaitingVoid); // clear mem

        }
                threadImageSaver.startThread();
        cout<<"start output thread"<<endl;
        return true;
    }
    
    
    void addPicSetupToWaiting(ofPath path,char cmdType,int cmdSetup){
        addPicToWaiting(path,makeSetupFileName(cmdType,cmdSetup));
    }
    void addPicSetupToWaiting(ofPixels pixels,char cmdType,int cmdSetup){
        addPicToWaiting(pixels,makeSetupFileName(cmdType,cmdSetup));
    }
    void addPicToWaiting(ofPixels pixels,string filename){
        ImageFiles q;
        q.image=pixels;
        q.fileName=filename;
        pixelsWaiting.push_back(q);
    }
    void addPicToWaiting(ofPath _path,string _filename){
        SVGFiles q;
        q.path=_path;
        q.fileName=_filename;
        pathToAdd++;
         threadImageSaver.addFrame(_path,Annnn,_filename);
        Annnn++;
        pathToAdd++;
    }
    string makeSetupFileName(char cmdType,int setup){
        string setupString;
        setupString="S";
        setupString+=cmdType;
        setupString+=ofToString(setup,4,'0');
        return setupString;
    }
    int outputCount=0;
    
    bool isBegin=true;
    bool isFinish=false;

    bool needAlert=false;
    agImageSequenceRecorder threadImageSaver; // use for save image
    
    int pathToAdd=0;
    float exposedSeconds=12;
    int upspeed=6;
    int downspeed=6;
    float baseExposedSeconds=78;
    int baseCount=4;
    float quickLiftHeight=4;
    bool uploadEnd=false;
    int picSaved=0;
};
