#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //panel.setup();

	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
    
    loadModel();
   
    cam.setDistance(4000);
    
    layertestmove.glTranslate(200, 200, 0);
    sliceLayPlane.set(1280,800,0.4);
    plate.setup();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast||testtri!=testtrilast){
        msliceLayPlane.setTranslation(0, 0, layertestZ);
        layertestZlast=layertestZ;
        testtrilast=testtri;
        
        if(mll.isdXdYlistfilled==100){
            cout<<"we just got to here to try layertest"<<"\n";
            layertest=mll.layertestat0(layertestZ);
            
        }
    }
    mll.update();
    if(sliceLayPlaneEnable==1){
        ofMultMatrix(msliceLayPlane);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();
        
        ofMultMatrix(mreset);
        
    }
   }

//--------------------------------------------------------------
void ofApp::draw(){
  
    ofBackground(ofColor::gray);
    
    layertest.draw(200,200);
     cam.begin();
    plate.drawincamera();
    
    
    
    // the model
    ofSetColor(ofColor::blueSteel);
    ofMultMatrix(mreadyModel);
    
    readyModel.draw();
    ofMultMatrix(mreset);
    
   	cam.end();
    
    if(0) {
        screenText.str("");
        screenText << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        screenText << "layertestat:"<<ofToString(layertestZ)<<"\n";
        screenText << "testtri"<<testtri<<"\n";
        screenText << "assimpmodel:"<<meshScale.z<<"\n";
        screenText << "vertexCount:"<<ofToString(readyModel.getNumVertices())<<"/"<<ofToString(readyModel.getNumIndices())<<"\n";
        screenText << "line:point:dxdy "<<ofToString(mll.linelist.size()/2)<<":"<<ofToString(mll.pointlist.size())<<":"<<ofToString(mll.dXdYlist.size())<<"\n";
       //screenText << "pointlist"
        //screenText << ofToString(layertest.);
        ofDrawBitmapString(screenText.str().c_str(), 20, 20);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            layertestZ++;
            break;
        case OF_KEY_DOWN:
            layertestZ--;
            break;
        case OF_KEY_LEFT:
            testtri-=100;
            break;
        case OF_KEY_RIGHT:
            testtri+=100;
            break;
    }
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

void ofApp::loadModel(){

    
    //assimp model load
    assimpModel.loadModel("testcube.stl");
    
    //assimpModel.calculateDimensions();
    //readyModel = ofMesh::box(300, 200, h);//cone(200.0, 200.0);
    readyModel=assimpModel.getMesh(0);
    readyModel.mergeDuplicateVertices();
    cout<<readyModel.getNumVertices()<<"\n";
    mll.setup(readyModel);
    meshScale=mll.meshScale;
    meshMin=mll.meshMin;
    
    h=meshMin.z;
    

}




