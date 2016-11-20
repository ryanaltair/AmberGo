#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "gkmll.h"
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
		
		ofImage ofLogo; // the OF logo
		ofLight light; // creates a light and enables lighting
		ofEasyCam cam; // add mouse controls for camera movement
    
    
    private:
    // workplace:
    ofBoxPrimitive playground;
    ofMatrix4x4 mplayground;
    
    ofBoxPrimitive outsideBox;
    ofMatrix4x4 moutsideBox;
    
    ofBoxPrimitive sliceLayPlane;
    ofMatrix4x4 msliceLayPlane;
    
    //
    ofMesh readyModel;
    ofxAssimpModelLoader assimpModel;
    int h=100;
    ofMatrix4x4 mreadyModel;
    unsigned int sliceLayer=200;//
    float sliceLayerThickness=0.4;//
    float sliceHeight; //
    
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    ofIndexType a=-1;
    // test theory
    
    
    // addons ofxMLL
    gkmll mll;
    ofVec3f meshScale;
    ofVec3f meshMax;
    ofVec3f meshMin;
    
    
    
    
    //output layer
    ofPath layertest;
     ofMatrix4x4 layertestmove;
    ofMatrix4x4 mreset;
    float layertestZlast=0;
    float layertestZ=0;
    int testtri=1;
    int testtrilast=0;
    
    //text out
    stringstream screenText;
    
   
    
};
