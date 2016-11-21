#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
    
    //assimp model load
    assimpModel.loadModel("testcube.stl");
    
    assimpModel.calculateDimensions();
    //readyModel = ofMesh::box(300, 200, h);//cone(200.0, 200.0);
    readyModel=assimpModel.getMesh(0);
    readyModel.mergeDuplicateVertices();
    cout<<readyModel.getNumVertices()<<"\n";
    mll.setup(readyModel);
    meshScale=mll.meshScale;
    meshMin=mll.meshMin;
    
    h=meshMin.z;
    //playground
    playground.set(1280,800,10);
    mplayground.setTranslation(0, 0, meshMin.z-5);
    
    outsideBox.set(1280,800,800);
    moutsideBox.setTranslation(0, 0, meshMin.z-400);
    
    sliceLayPlane.set(1280,800,0.4);
    cam.setDistance(2000);
    
    layertest=mll.layertestat(readyModel, layertestZ,testtri);// to do
    
    
    layertestmove.glTranslate(200, 200, 0);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast||testtri!=testtrilast){
        //layertest=mll.layertestat(readyModel, layertestZ,testtri);// to do
        
        msliceLayPlane.setTranslation(0, 0, layertestZ);
        layertestZlast=layertestZ;
        testtrilast=testtri;
        
        if(mll.isdXdYlistfilled==100){
            cout<<"we just got to here to try layertest"<<"\n";
            layertest=mll.layertestat0(layertestZ);
            
        }
    }
    mll.update(readyModel);
   }

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
    
    layertest.draw(400,400);
     cam.begin();
    
    
    
    
    // the model
    ofSetColor(ofColor::blueSteel);
    ofMultMatrix(mreadyModel);
    
    readyModel.draw();
    ofMultMatrix(mreset);
    
    // the ground
    ofSetColor(150);
    ofMultMatrix(mplayground);
    playground.drawWireframe();
    ofMultMatrix(mreset);
    
    // the outsidebox
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        ofMultMatrix(moutsideBox);
        outsideBox.draw();
        ofMultMatrix(mreset);
        
    }
    if(sliceLayPlaneEnable==1){
        ofMultMatrix(msliceLayPlane);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();
        
        ofMultMatrix(mreset);
        
    }
	cam.end();
    
    if(1) {
        screenText.str("");
        screenText << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        screenText << "layertestat:"<<ofToString(layertestZ)<<"\n";
        screenText << "testtri"<<testtri<<"\n";
        screenText << "assimpmodel:"<<meshScale.z<<"\n";
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






