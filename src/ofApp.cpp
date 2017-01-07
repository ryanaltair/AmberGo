#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Amber Go Demo");
    panel.setup();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
    modelpath="testcube.stl";
    apppreference.isModelChanged=true;
    plate.setup();
    //setup fbo
    ofFbo::Settings fbosettings;
    fbosettings.width = 1280;
    fbosettings.height =768;
    fbosettings.textureTarget = GL_TEXTURE_2D;
    fbo.allocate(fbosettings);
    // fbo end
    
}

//--------------------------------------------------------------
void ofApp::update(){
    apppreference.updatelayerout(panel.getWidth());
    panel.update();
    //panel.layertestZ=panel.sliceHeight;
    loadModel();
    //sliceHeight=panel.layertestZ;//sliceLayer*sliceLayerThickness;
    if(panel.layertestZ!=panel.layertestZlast){
       // panel.layertestZ=panel.layertestZ;
        panel.layertestZlast=panel.layertestZ;
        //cout<<"we slice at now "<<endl;
        plate.sliceAt(panel.layertestZ);
        merger.sliceAt(panel.layertestZ);
        //cout<<"we slice at end"<<endl;
        plate.update();
    }

    if(merger.isSliceChanged==true){
       // cout<<"B:we just got to here to try layertest"<<"\n";
        layertest=merger.layertest;
        drawFBO();
        panel.outputDone(true);
        merger.isSliceChanged=false;
        if(panel.bPrint==true){
            bSnapshot=true;
            panel.snapcount++;
        }
    }
    //mll.update();
    //cout<<"dxdyfilled"<<mll.isdXdYlistfilled<<endl;
    
   }

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    ofBackground(ofColor::black);
    plate.drawincamera(apppreference.plateview);
    ofDisableDepthTest();
    if(panel.ShowSlice==true){
        fbo.draw(apppreference.sliceview);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            panel.layertestZ++;
          // panel.sliceHeight=panel.layertestZ;
            break;
        case OF_KEY_DOWN:
            panel.layertestZ--;
         //    panel.sliceHeight=panel.layertestZ;
            break;
        case OF_KEY_LEFT:
            //bSnapshot=true;
            //plate.cam.orbit(1, 0, plate.cam.getDistance());
            //plate.cam.rotate(90, plate.cam.getUpDir() );
            break;
        case OF_KEY_RIGHT:
            panel.bPrint=true;
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
        apppreference.isModelChanged=true;
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            modelpath=dragInfo.files[k];
            cout<<modelpath<<endl;
        }
    }
}
//side window

void ofApp::drawSideWindow(ofEventArgs & args){
    fbo.draw(0,0);
    
    //savePic();
}


//addons

void ofApp::loadModel(){
    if(apppreference.isModelChanged==false){
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
        panel.setSliceUnready();
        merger.setpixelpermm(apppreference.getpixelpermm());
        merger.setmmperpixel(apppreference.getmmperpixel());
        merger.start(modelpath);
        bModelLoaded=false;
        modelpath.clear();
    }
    
    // when loading
    if(merger.isThreadRunning()==true){
    //    cout<<"jell"<<endl;
        return;
    }
    //when loaded
    if(bModelLoaded==false){
        readyModel=merger.meshmodel;
        bModelLoaded=true;
        return;
    }
    
    panel.setSliceReady();
    mll.setup(readyModel);
    plate.modelSize=mll.getScale();
    ofVec3f newposti;
    newposti=mll.meshMin;
    newposti.x=0;//-plate.modelSize.x/2;
    newposti.y=0;//-plate.modelSize.y/2;
    plate.setPosition(newposti);
    panel.sliceMax=plate.modelSize.z;
    cout<<"modelsize"<<plate.modelSize<<endl;
    apppreference.isModelChanged=false;
}
void ofApp::drawFBO(){
    // now we try fbo
    timer=ofGetElapsedTimef();
    
    fbo.begin();
    
    ofClear(ofColor::black);
    //ofBackground(0,0,0);
    layertest.scale(apppreference.getpixelpermm().x, apppreference.getpixelpermm().y);
    layertest.draw(1280/2,768/2);
    fbo.end();
    
    //put fbo into pixels
    ofPixels pixelsbuffer;
    ofPixels pixelsbuffervoid;
    pixelsbuffer.allocate(1280, 768,3);
    pixelsbuffervoid.allocate(1280, 768, 3);
    fbo.readToPixels(pixelsbuffer);
    
    // and save images with pixels
    string annnn;
    string emmmm;
    annnn=ofToString(panel.snapcount);
    int zinpulse;
    float layertestZmm=panel.layertestZ;
    float pulsepermm=50; //for 1204 the 1 round for 4 mm in z , 1 round means 4 mms head up and 200 steps for stepper
    zinpulse=layertestZmm*pulsepermm+1000;// to easy the bits problem ,we just start from 1000
    emmmm=ofToString(zinpulse);
   // cout<<"|||| save start: "<<ofToString(ofGetElapsedTimef())<<endl;
    string tickfileName = "output/fabfiles/A" +annnn+emmmm+ ".png";
    panel.snapcount+=1;
    annnn=ofToString(panel.snapcount);
    emmmm=ofToString(panel.exposedTime);
    string tockfileName ="output/fabfiles/A" +annnn+emmmm+ ".png";
    if(bSnapshot==true){
        ofSaveImage(pixelsbuffer, tickfileName);
        ofSaveImage(pixelsbuffervoid, tockfileName);

    }
    //end fbo
    //cout<<"|||| save middl: "<<ofToString(ofGetElapsedTimef())<<endl;

    if (bSnapshot == true){
        bSnapshot = false;
        if(panel.snapcount>=9000||panel.layertestZ>plate.modelSize.z){
            panel.bPrint=false;
        }
    }

}




