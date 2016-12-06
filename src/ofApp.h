#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "gkmll.h"
#include "ofxDatGui.h"
#include "gkplate.h"
#include "gkpanel.h"
class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		
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
    void loadModel();
		
		ofImage ofLogo; // the OF logo
		ofLight light; // creates a light and enables lighting
		ofEasyCam cam; // add mouse controls for camera movement
    //palte
    gkplate plate;
    
    //GUI
    gkpanel panel;
        
    private:

    
    //
    ofMesh readyModel;
    
    ofxAssimpModelLoader assimpModel;
    int h=100;
    ofMatrix4x4 mreadyModel;
    unsigned int sliceLayer=200;//
    float sliceLayerThickness=0.4;//
    float sliceHeight; //
    ofBoxPrimitive sliceLayPlane;
    ofMatrix4x4 msliceLayPlane;


    // test theory
    float layertestZlast=0;
    float layertestZ=0;
    int testtri=1;
    int testtrilast=0;
    
    //output layer
    ofPath layertest;
    ofMatrix4x4 layertestmove;
    ofMatrix4x4 mreset;
        int sliceLayPlaneEnable=1;
    // addons ofxMLL
    gkmll mll;
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    
    
    
    //text out
    stringstream screenText;
    
    //drag and drop info
    string modelpath;
    bool isModelChanged=true;
    
};
