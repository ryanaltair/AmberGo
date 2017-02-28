#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //for app looks like a app
    ofSetWindowTitle("Amber Go Demo");//
    ofSetEscapeQuitsApp(false);
    panel.setup();
    
    // set framerate // PS: setVSYNC will failed in Windows but still work well in Mac
    //     ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
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
    
    threadImageSaver.setPrefix(ofToDataPath("output3/")); // this directory must already exist
    threadImageSaver.setFormat("png"); // png is really slow but high res, bmp is fast but big, jpg is just right

    
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
                
                saveImage(ofToString(panel.iShowAllSliceLayerCount));
                
                if(panel.iShowAllSliceLayerCount==threadSlice.alllayertests.size()-1){
                    
                    if(threadImageSaver.isThreadRunning()==false){
                        if(threadImageSaver.q.size()==0){
                            panel.setSliceDone();
                            easyLogTimeTo("output");
                        }
                        
                    }
                    
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
    threadImageSaver.waitForThread();
}
void ofApp::exit(){
    threadImageSaver.waitForThread();
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
    fbo.end();
}
void ofApp::saveImage(string picname){
    fbo.readToPixels(pixelsbuffer);
    if(panel.outputToggle->getChecked()==true){
//        ofSaveImage(pixelsbuffer, picname);
        threadImageSaver.addFrame(pixelsbuffer);
        if(threadImageSaver.isThreadRunning()){
            
        } else {
            threadImageSaver.startThread();
        }
        //cout<<"save image"<<endl;
        bSnapshot = false;
    }
    
}





