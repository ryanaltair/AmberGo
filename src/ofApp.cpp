#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    panel.setup();
    panel.sliceHeightBind(layertestZ);
	ofSetVerticalSync(true);
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
    modelpath="testcube.stl";
    isModelChanged=true;
    plate.setup();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    panel.update();
    layertestZ=panel.sliceHeight;
    loadModel();
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast){
        plate.layertestZ=layertestZ;
        layertestZlast=layertestZ;
        //cout<<"we slice at now "<<endl;
        plate.sliceAt(layertestZ);
        merger.sliceAt(layertestZ);
        //cout<<"we slice at end"<<endl;
        plate.update();
    }

    if(merger.isSliceChanged==true){
        cout<<"B:we just got to here to try layertest"<<"\n";
        layertest=merger.layertest;
        panel.outputDone(true);
        merger.isSliceChanged=false;
    }
    //mll.update();
    //cout<<"dxdyfilled"<<mll.isdXdYlistfilled<<endl;
    
   }

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    ofBackground(ofColor::black);

    plate.drawincamera();
    ofDisableDepthTest();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            layertestZ++;
           panel.sliceHeight=layertestZ;
            break;
        case OF_KEY_DOWN:
            layertestZ--;
             panel.sliceHeight=layertestZ;
            break;
        case OF_KEY_LEFT:
            bSnapshot=true;
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
//side window

void ofApp::drawSideWindow(ofEventArgs & args){
    ofBackground(0,0,0);
     layertest.draw(1280/2,768/2);
    
    savePic();
}


//addons

void ofApp::loadModel(){
    if(isModelChanged==false){
        return;
      
    }
    
    //assimp model load
    if(modelpath.size()>0){
        //plate.addModel(modelpath);
        assimpLoader.loadModel(modelpath);
        if(assimpLoader.getMeshCount()>1){
            //plate.addModel(assimpLoader.getMesh(1));
            
            assimpLoader.drawFaces();
        }else{
            plate.addModel(assimpLoader.getMesh(0));
            
        }
        
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
    isModelChanged=false;
}

void ofApp::savePic(){
    if (bSnapshot == true){
        // grab a rectangle at 200,200, width and height of 300,180
        snapImg.grabScreen(0,0,1280,768);
        
        string fileName = "a" + ofToString(ofToInt(ofToString(layertestZ*100))) + ".png";
        snapImg.save(fileName);
        bSnapshot = false;
    }
}



