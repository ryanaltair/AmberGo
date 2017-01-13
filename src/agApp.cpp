#include "agApp.h"

//--------------------------------------------------------------
void agApp::setup(){
    //for app looks like a app
    ofSetWindowTitle("Amber Go Demo");//
    ofSetEscapeQuitsApp(false);
    panel.setup();
    
    // set framerate // PS: setVSYNC will failed in Windows but work well in Mac
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    // init the modelpath
//    modelpath="testcube.stl";
//    apppreference.isModelChanged=true;
    
    // init the plate which hold the 3D model we see
    plate.setup();
    //setup fbo which will for export images
    ofFbo::Settings fbosettings;
    fbosettings.width = 1280;
    fbosettings.height =768;
    fbosettings.textureTarget = GL_TEXTURE_2D;
    fbo.allocate(fbosettings);
    // fbo end
    
}

//--------------------------------------------------------------
void agApp::update(){
    apppreference.updatelayerout(panel.getWidth());
    panel.update();
    loadModel();
    if(panel.layertestZ!=panel.layertestZlast){
        panel.layertestZlast=panel.layertestZ;
        //cout<<"we slice at now "<<endl;
        plate.sliceAt(panel.layertestZ);
        plate.update();
    }
    if(threadMerge.isThreadRunning()==false){
        if(apppreference.bHaveModelLoaded==false){return;}
        if(threadMerge.layertestZ!=panel.layertestZ){
        threadMerge.sliceAt(panel.layertestZ);
        }
        
        if(threadMerge.isSliceChanged==true){
       // cout<<"B:we just got to here to try layertest"<<"\n";
            layertest=threadMerge.layertest;
            drawFBO();
            panel.outputDone(true);
            threadMerge.isSliceChanged=false;
            if(panel.bPrint==true){
                bSnapshot=true;
                panel.snapcount++;
            }
        }
        
    }
}

//--------------------------------------------------------------
void agApp::draw(){
    ofEnableDepthTest();
    ofBackground(ofColor::black);
    plate.drawincamera(apppreference.plateview);
    ofDisableDepthTest();
    if(panel.ShowSlice==true){
        fbo.draw(apppreference.sliceview);
    }
}


//--------------------------------------------------------------
void agApp::keyPressed(int key){
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
void agApp::keyReleased(int key){

}

//--------------------------------------------------------------
void agApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void agApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void agApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void agApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void agApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void agApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void agApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void agApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void agApp::dragEvent(ofDragInfo dragInfo){ 
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

void agApp::drawSideWindow(ofEventArgs & args){
    fbo.draw(0,0);
    
    //savePic();
}


//addons

void agApp::loadModel(){
    if(apppreference.isModelChanged==false){
        return;
      
    }
    //assimp model load
    if(modelpath.size()>0){
        panel.setSliceUnready();
        threadMerge.start(modelpath);
        bModelLoaded=false;
        modelpath.clear();
    }
    
    // when loading
    if(threadMerge.isThreadRunning()==true){
        return;
    }
    //when loaded
    if(bModelLoaded==false){
        bModelLoaded=true;
        return;
    }
    plate.addModel(threadMerge.mll.mergedMesh);
    threadMerge.assimpmodel.clear();
    panel.setSliceReady();
    plate.modelSize=threadMerge.mll.getScale();
    ofVec3f newposti;

    newposti=threadMerge.mll.meshMin;
    newposti.x=0;//-plate.modelSize.x/2;
    newposti.y=0;//-plate.modelSize.y/2;
    plate.setPosition(newposti);
    panel.sliceMax=plate.modelSize.z;
    cout<<"modelsize"<<plate.modelSize<<endl;
    apppreference.isModelChanged=false;
    apppreference.bHaveModelLoaded=true;
}
/**
 draw the fbo and save the pic as png file
 */
void agApp::drawFBO(){
    // now we try fbo
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




