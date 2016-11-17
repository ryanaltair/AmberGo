#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
	 
    playground.set(1280,800,10);
    playground.setPosition(0,0,-5);
    
    outsideBox.set(1280,800,800);
    outsideBox.setPosition(0, 0, 400);
    
    sliceLayPlane.set(1280,800,0.4);
  
   
    cam.setDistance(2000);
    readyModel = ofMesh::box(200.0, 200.0, 200.0);
}

//--------------------------------------------------------------
void ofApp::update(){
    sliceHeight=sliceLayer*sliceLayerThickness;

   }

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
	
	
	cam.begin();
	
    
    // the model
    ofTranslate(0,0,100);
    ofSetColor(ofColor::blueSteel);
    readyModel.draw();
    ofTranslate(0,0,-100);
    // the ground
    ofSetColor(150);
    playground.draw();
    // the outsidebox
    if(outsideBoxEnable==1){
    ofSetColor(20, 20, 20, 50);
    outsideBox.draw();
    }
    if(sliceLayPlaneEnable==1){
        sliceLayPlane.setPosition(0,0,sliceHeight);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();}
	cam.end();
    
    if(1) {
        screenText.str("");
        screenText << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        ofDrawBitmapString(screenText.str().c_str(), 20, 20);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//addons

void fillpointlinelist(ofMesh frommesh){
    vector<ofVec3f> point = frommesh.getVertices();
    ofVec3f a,b,c;
    ofIndexType ia,ib,ic;
    for(ofIndexType i=0;i<frommesh.getNumIndices();i+=3){
        ia=frommesh.getIndex(i);
        a=frommesh.getVertex(ia);
        
    }
}

ofVec3f ofApp::getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
    ofVec3f returnPoint;
    // compare the upper point and the down point
    if(pointDown.z>pointUp.z){
        float tempz=pointDown.z;
        pointDown.z=pointUp.z;
        pointUp.z=tempz;
    }
    //check if the line cross the z plane
    if(pointUp.z>z||pointDown.z<z){
        returnPoint.x=returnPoint.y=0;
        returnPoint.z=-1; //false
        return returnPoint;
    }
    // check if the line parallel to z plane
    if(pointDown.z==pointUp.z){
        returnPoint.x=returnPoint.y=0;
        returnPoint.z=-2; //
    }
    if(pointUp.z==z||pointDown.z==z){
        returnPoint.z=0;
        returnPoint.x=0;
        returnPoint.y=0;
        return returnPoint;
    }
    float zLength=(pointUp.z-pointDown.z);
    float z1Length=1/zLength;
    float kx=(pointUp.x-pointDown.x)*z1Length;
    returnPoint.x=(z-pointUp.z)*kx+pointUp.x;
    float ky=(pointUp.y-pointDown.y)*z1Length;
    returnPoint.y=(z-pointUp.z)*ky+pointUp.y;
    returnPoint.z=z;
    return returnPoint;
}


