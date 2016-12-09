#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    panel.setup();

	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	
    
    loadModel();
   
    layertestmove.glTranslate(200, 200, 0);

    plate.setup();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    loadModel();
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast){
        plate.layertestZ=layertestZ;
        layertestZlast=layertestZ;
      
        if(mll.isdXdYlistfilled==100){
            
            cout<<"we just got to here to try layertest"<<"\n";
            layertest=mll.layertestat(layertestZ);
            
        }
    }
    mll.update();
    
   }

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    ofBackground(ofColor::gray);
    layertest.draw(200,200);
    plate.drawincamera();
    ofDisableDepthTest();
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
    if( dragInfo.files.size() > 0 ){
        //dragPt = dragInfo.position;
        isModelChanged=true;
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            modelpath=dragInfo.files[k];
            cout<<modelpath<<endl;
        }
    }
}

//addons

void ofApp::loadModel(){
    if(isModelChanged==false){
        return;
      
    }
    
    //assimp model load
    if(modelpath.size()>0){
        merger.start(modelpath);
        bLoaded=false;
        modelpath.clear();
    }
    
    // when loading
    if(merger.isThreadRunning()==true){
        return;
    }
    //when loaded
    if(bLoaded==false){
        readyModel=merger.meshmodel;
        bLoaded=true;
        return;
    }
    

    mll.setup(readyModel);
    plate.modelSize=mll.getScale();
    cout<<"modelsize"<<plate.modelSize<<endl;
    plate.addModel(readyModel);
    isModelChanged=false;
}




