#pragma once

#include "ofMain.h"

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
    int h=100;
    ofMatrix4x4 mreadyModel;
    unsigned int sliceLayer=200;//
    float sliceLayerThickness=0.4;//
    float sliceHeight; //
    
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    
    // test theory
    
    
    // addons ofxMLL
    vector<ofVec3f> pointlist;
    vector<ofIndexType> linelist;// {p0 p1} the index point to point list
    vector<ofIndexType> nearpointlist;//{pa pb} the index point to point list
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    void fillpointlinelist(ofMesh);
    
    //output layer
    ofPath layertest;
    ofPath layertestat(ofMesh mesh,float z,int tri=1);
    ofMatrix4x4 layertestmove;
    ofMatrix4x4 mreset;
    
    float layertestZ=0;
    int testtri=1;
    //text out
    stringstream screenText;
    
    //vec3fFalseDefine
    float isnotvalid=-1;
    
};
