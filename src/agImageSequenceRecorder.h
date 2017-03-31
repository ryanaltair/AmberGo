/*
 
 Based on code by Memo from this thread:
 http://forum.openframeworks.cc/index.php?topic=1687.0
 
 */
#pragma once
#include "ofMain.h"
#include "ofxEditableSVG.h"
typedef struct {
    string fileName;
    ofPixels image;
} QueuedImage;
typedef struct {
    string fileName;
    ofPath path;
} QueuedSVG;

class agImageSequenceRecorder : public ofThread {
public:
    int counter;
    queue<QueuedImage> q;
    
    queue<QueuedSVG> qSVG;
    string prefix;
    string format;
    int numberWidth;
    agImageSequenceRecorder(){
        counter=1000;
        numberWidth=4;
        
    }
    void init(){
        //        counter=1000;
    }
    void end(){
        counter=1000;
        
    }
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
        if(fmt=="svg"){
            cout<<"now we using svg"<<endl;
            usingSVG=true;
        }else{
        usingSVG=false;
        }
    }
    
    void setCounter(int count){
        counter = count;
    }
    
    void setNumberWidth(int nbwidth){
        numberWidth = nbwidth;
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            if(usingSVG==false){
                if(!q.empty()){
                    QueuedImage i = q.front();
                    cout<<"thread-add image:"<<i.fileName<<endl;
                    ofSaveImage(i.image, i.fileName);
                    q.pop();
                    if(q.empty()){
                        cout<<"thread empty:" <<endl;
                    }
                }
            }else{
                if(!qSVG.empty()){
                    QueuedSVG i = qSVG.front();
                    cout<<"thread-save svg:"<<i.fileName<<endl;
                    ofxEditableSVG svg;
                    svg.setSize(1280, 768,"px");
                    svg.setViewbox(0, 0, 1280, 768);
                    svg.setFilled(true);
                    svg.addPath(i.path);
                    svg.save(i.fileName);
                    qSVG.pop();
                    if(qSVG.empty()){
                        cout<<"thread svg empty:" <<endl;
                    }
                }
            }
        }
        
        
        
    }
    
    void addFrame(ofImage &img){
        addFrame(img.getPixels());
    }
    
    
    
    void addFrame(ofPixels imageToSave) {
        //char fileName[100];
        //snprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());
        string fileName = "output3/A" + ofToString(counter, numberWidth, '0') + "xx3000." + format;
        counter++;
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;
        qImage.image = imageToSave;
        q.push(qImage);
        
    }
    
    void addFrame(ofPixels imageToSave, int Annnn,string Emmmmmm) {
        string fileName = prefix + ofToString(Annnn, numberWidth, '0') + Emmmmmm +"."+ format;
        counter++;
        QueuedImage qImage;
        qImage.fileName = fileName;
        cout<<"main-add pic:"<<fileName<<endl;
        qImage.image = imageToSave;
        q.push(qImage);
        
    }
    void addFrame(ofPath pathToSave,int Annnn,string Emmmmmm ){
        string fileName = prefix + ofToString(Annnn, numberWidth, '0') + Emmmmmm +"."+ format;
        counter++;
        QueuedSVG quenedSVG;
        quenedSVG.fileName = fileName;
        cout<<"main-add svg:"<<fileName<<endl;
        quenedSVG.path = pathToSave;
        qSVG.push(quenedSVG);
    }
     bool usingSVG=true;
};
