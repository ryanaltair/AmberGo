#pragma once

#include "ofMain.h"
#include "agmll.h"
#include "agplate.h"
#include "agpanel.h"
#include "agSliceManager.h"
#include "agAppPreference.h"
#include "agOutputManager.h"
class ofApp : public ofBaseApp {
public:
    //gl loop
    void setup();
    void update();
    void draw();
    // events
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
private:
    //side window
    void drawSideWindow(ofEventArgs & args);
    ///snap
    /**
     draw the fbo which hold the slicelayer
     */
    void drawFBO(ofPath pathdraw); 
    void outputLayer();
    /**
     when loadpath was changed,
     it will load model with thread called merger
     */
    void loadModel();
    agSliceManager threadSlice;// threaded object. Merger
    bool bModelLoaded=false;
    agplate plate;//palte
    agpanel panel;    //GUI
    agAppPreference apppreference;// store the prefer
    ofPath layertest;//output layer
    agOutputManager outputManager;
    string modelpath;  //drag and drop info
    
    bool bSnapshot=false;
    ofFbo fbo;
    ofPixels pixelsbuffer;
    ofPixels pixelsbuffervoid;
   //timetest
    float timekeep;
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
};
