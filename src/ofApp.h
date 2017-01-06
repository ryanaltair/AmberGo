#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "agmll.h"
#include "ofxDatGui.h"
#include "agplate.h"
#include "agpanel.h"
#include "agSliceManager.h"
#include "agAppPreference.h"


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
    //side window
    void drawSideWindow(ofEventArgs & args);
    //add on
    void loadModel();
		
    // threaded object. Merger
    agSliceManager merger;
    bool bModelLoaded=false;

    
    //palte
    agplate plate;
    
    //GUI
    agpanel panel;
    
    //model load from assimp
    ofxAssimpModelLoader assimpLoader;
    private:
    //
    ofMesh readyModel;

    // test theory
    float layertestZlast=0;
    float layertestZ=0;

    //output layer
    ofPath layertest;
    ofMatrix4x4 layertestmove;

      
    // addons ofxMLL
    agmll mll;
    ofVec3f meshScale;
    
    

    //drag and drop info
    string modelpath;

    
    ///snap
    void drawFBO();
    bool bSnapshot=false;
    ofFbo fbo;
    ofPixels pixelsbuffer;
    
  //  ofImage snapImg;
     
    
    // timer
    float timer;
    
    
    agAppPreference apppreference;
};
