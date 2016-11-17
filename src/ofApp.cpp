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
        stringstream ss;
        ss << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
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



ofVec3f ofApp::getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
    ofVec3f pointZ;
    if(pointUp.z>z||pointDown.z<z){
        pointZ.x=pointZ.y=0;
        pointZ.z=z;
        return pointZ;
    }
    if(pointUp.z==z||pointDown.z==z){
        pointZ.z=0;
        pointZ.x=0;
        pointZ.y=0;
        return pointZ;
    }
    float zLength=(pointUp.z-pointDown.z);
    float z1Length=1/zLength;
    float kx=(pointUp.x-pointDown.x)*z1Length;
    pointZ.x=(z-pointUp.z)*kx+pointUp.x;
    float ky=(pointUp.y-pointDown.y)*z1Length;
    pointZ.y=(z-pointUp.z)*ky+pointUp.y;
    pointZ.z=z;
    return pointZ;
}


