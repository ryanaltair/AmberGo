#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //for app looks like a app
    ofSetWindowTitle("Amber Go Demo");//
    ofSetEscapeQuitsApp(false);
    panel.setup();
    
    // set framerate // PS: setVSYNC will failed in Windows but still work well in Mac
    //     ofSetVerticalSync(true);
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
    pixelsbuffer.allocate(1280, 768,3);
    pixelsbuffervoid.allocate(1280, 768, 3);
    // fbo end
    
}

//--------------------------------------------------------------
void ofApp::update(){
    apppreference.updatelayerout(panel.getWidth());
    panel.update();
    loadModel();
    if(panel.isSliceHeightChange()){
        panel.layertestZlast=panel.layertestZ;
        //cout<<"we slice at now "<<endl;
        plate.sliceAt(panel.layertestZ);
        plate.update();
    }
    if(threadSlice.isThreadRunning()==false){
        if(apppreference.bHaveModelLoaded==false){return;}
        if(panel.bAllSlice==true){
            if(threadSlice.isAllSliceDone==false){
                threadSlice.allSlice(panel.layerthickness);
            }
            panel.bAllSlice=false;
            
        }
        if(threadSlice.layertestZ!=panel.layertestZ){
            //threadSlice.sliceAt(panel.layertestZ);
        }
        
        if(/* DISABLES CODE */ (0)&&threadSlice.isSliceChanged==true){
            // cout<<"B:we just got to here to try layertest"<<"\n";
            layertest=threadSlice.layertest;
            if(panel.bPrint==true){
                bSnapshot=true;
                panel.snapcount++;
            }
            drawFBO();
            outputLayer();
            panel.setOutputDone(true);
            threadSlice.isSliceChanged=false;
        }
        if(threadSlice.isAllSliceDone==true&&panel.bShowAllSlice==true){
            if(panel.iShowAllSliceLayerCount<threadSlice.alllayertests.size()){
                panel.layertestZ=panel.layerthickness*panel.iShowAllSliceLayerCount;
                layertest=threadSlice.alllayertests[panel.iShowAllSliceLayerCount];
                panel.iShowAllSliceLayerCount++;
                panel.layertestZ+=panel.layerthickness;
                if(panel.bPrint==true){
                    bSnapshot=true;
                    panel.snapcount++;
                }
                if(panel.iShowAllSliceLayerCount==1){
                    easyLogTimeFrom("output");
                }
                drawFBO();
                string pathname="output2/"+ofToString(panel.iShowAllSliceLayerCount)+".png";
                saveImage(pathname);
                if(panel.iShowAllSliceLayerCount==threadSlice.alllayertests.size()-1){
                    
                    easyLogTimeTo("output");
                }
            }
        }
        
    }
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
            panel.bAllSlice=true;
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
        panel.setSliceUnready();
        threadSlice.loadModel(modelpath);
        bModelLoaded=false;
        modelpath.clear();
    }
    
    // when loading
    if(threadSlice.isThreadRunning()==true){
        return;
    }
    //when loaded
    if(bModelLoaded==false){
        bModelLoaded=true;
        return;
    }
    plate.addModel(threadSlice.mll.mergedMesh);
    threadSlice.assimpmodel.clear();
    panel.setSliceReady();
    plate.modelSize=threadSlice.mll.getScale();
    ofVec3f newposti;
    
    newposti=threadSlice.mll.meshMin;
    newposti.x=0;//-plate.modelSize.x/2;
    newposti.y=0;//-plate.modelSize.y/2;
    plate.setPosition(newposti);
    panel.sliceMax=plate.modelSize.z;
    cout<<" modelsize:"<<plate.modelSize<<endl;
    apppreference.isModelChanged=false;
    apppreference.bHaveModelLoaded=true;
}
/**
 draw the fbo
 */
void ofApp::drawFBO(){
    // now we try fbo
    
    fbo.begin();
    ofClear(ofColor::black);
//        ofBackground(0,0,0);
    
    layertest.scale(apppreference.getpixelpermm().x, apppreference.getpixelpermm().y);
    layertest.draw(1280/2,768/2);
    vector<ofPolyline> outlines= layertest.getOutline();
    ofSetColor(255, 0, 0);

    outlines[0].draw();
//    for (auto line:outlines) {
//        //
//        cout<<"line size:"<<line.size()<<endl;
//    }
//    outline[0].draw(1280/2,768/2);
    fbo.end();
}
void ofApp::saveImage(string picname){
    fbo.readToPixels(pixelsbuffer);
    if(panel.outputToggle->getChecked()==true){
        ofSaveImage(pixelsbuffer, picname);
        //cout<<"save image"<<endl;
        bSnapshot = false;
    }
    
}
/**
 output the slice use FBO as png file
 */
void ofApp::outputLayer(){
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
        cout<<"save image"<<endl;
        bSnapshot = false;
        
        if(panel.snapcount>=9000||panel.layertestZ>plate.modelSize.z){
            panel.bPrint=false;
        }
    }
    
}




