#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "gkmll.h"
#include "ofxDatGui.h"
#include "gkplate.h"
#include "gkpanel.h"
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
    //add on
    void loadModel();
		
	
    
    //palte
    gkplate plate;
    
    //GUI
    gkpanel panel;
        
    private:
    //
    ofMesh readyModel;
    ofxAssimpModelLoader assimpModel;

    // test theory
    float layertestZlast=0;
    float layertestZ=0;

    //output layer
    ofPath layertest;
    ofMatrix4x4 layertestmove;

      
    // addons ofxMLL
    gkmll mll;
    ofVec3f meshScale;
    
    

    //drag and drop info
    string modelpath;
    bool isModelChanged=true;
    
};
