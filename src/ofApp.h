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
    ofBoxPrimitive outsideBox;
    ofBoxPrimitive sliceLayPlane;
    //
    ofMesh readyModel;
    unsigned int sliceLayer=200;//
    float sliceLayerThickness=0.4;//
    float sliceHeight; //
    
    // enables
    int outsideBoxEnable=0;
    int sliceLayPlaneEnable=1;
    
    // addons ofxMLL
    vector<ofVec3f> pointlist;
    vector<ofIndexType> pointlinelist;// {p0 p1} the index point to point list
    vector<ofIndexType> nearpointlist;//{pa pb} the index point to point list
    ofVec3f getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z);
    void fillpointlinelist(ofMesh);
    //text out
    stringstream screenText;
    
    //vec3ffalsedefine
    float isnotvalid=-1;
    
};
