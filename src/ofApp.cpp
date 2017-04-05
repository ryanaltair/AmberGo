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
    //    fbosettings.numSamples=4;
    fbo.allocate(fbosettings);
    pixelsbuffer.allocate(1280, 768,3);
    pixelsbuffervoid.allocate(1280, 768, 3);
    // fbo end
    welcomeImage.load("welcome");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if(welcomeNow){
        if(ofGetElapsedTimef()>1.8){
            welcomeNow=false;
            welcomeImage.clear();
        }
    }
    apppreference.updatelayerout(panel.getWidth());
    panel.update();
    loadModel();
    outputManager.checkEnd();
    checkSliceHeightChange();
    plate.setPositionOffset(panel.getPositionOffset());
    plate.setScaleFactor(panel.getScaleFactor());
    checkNeedSlice();
    sliceModel();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(welcomeNow){
         ofEnableDepthTest();
        
        ofBackground(ofColor::black);
        welcomeImage.draw(ofGetWindowSize().x/2-welcomeImage.getWidth()/2,ofGetWindowSize().y/2-welcomeImage.getHeight()/2);
    }else{
    ofEnableDepthTest();
    ofBackground(ofColor::black);
    plate.drawincamera(apppreference.plateview);
    ofDisableDepthTest();
    if(panel.ShowSlice==true){
        fbo.draw(apppreference.sliceview);
    }
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            panel.setSliceHeight(panel.getSliceHeight()+1);
            break;
        case OF_KEY_DOWN:
            panel.setSliceHeight(panel.getSliceHeight()-1);
            break;
        case OF_KEY_LEFT:
            
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
    outputManager.end();
}
void ofApp::exit(){
    outputManager.end();
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
    plate.addModel(threadSlice.getMergedMesh());
    panel.setSliceReady();
    plate.modelSize=threadSlice.mll.getScale();
    panel.setModelScale(threadSlice.mll.getScale());
    ofVec3f newposti;
    newposti=-threadSlice.mll.meshMin;
 
    plate.setMinPosition(newposti);
    panel.sliceMax=plate.modelSize.z;
    cout<<" modelsize:"<<plate.modelSize<<endl;
    apppreference.isModelChanged=false;
    apppreference.bHaveModelLoaded=true;
}
void ofApp::checkNeedSlice(){
    if(threadSlice.isThreadRunning()){
        return;
    }
    if(panel.ShowingAllSlice==false){
        if(threadSlice.isAllSliced()){
            panel.setSliceDone();
        }
    }
    if(panel.needAllToSlice()==true){
        if(apppreference.bHaveModelLoaded==false){
        //there is no model so need we need to add one
            ofFileDialogResult result = ofSystemLoadDialog("Load file");
            if(result.bSuccess) {
                apppreference.isModelChanged=true;
                modelpath=result.getPath();
                // load  file using dialog 
            }
            return;
        }
        
        outputManager.init();
        if(threadSlice.isAllSliced()==false||panel.isModelUpdated()==true){
            threadSlice.setScaleFactor(panel.getScaleFactor());
            threadSlice.setModelOffset(panel.getPositionOffset());
            threadSlice.allSlice(panel.layerthickness);
            panel.setSlicing();
            outputManager.setPrint(panel.exposedTime, 6, 6, panel.baseExposedTime, 4, 3000, 4);
        }
    }
    
}
void ofApp::sliceModel(){
    if(threadSlice.isThreadRunning()==false){
        if(apppreference.bHaveModelLoaded==false){return;}
        if(threadSlice.isAllSliced()==false){return;}
        if(panel.ShowingAllSlice==true){
            ofIndexType currentSliceLayer= panel.iShowAllSliceLayerCount;
            ofIndexType allSliceLayerCount=threadSlice.alllayertests.size()-1;
            
            if(currentSliceLayer<=allSliceLayerCount){
                if(panel.getSaveDirectoryChanged()==true){
                    cout<<"now we have new save dir"<<endl;
                    outputManager.startOutput(panel.getSaveDirectory());
                }
                if(currentSliceLayer==0){
                    easyLogTime.from("output to thread");
                }
                layertest=threadSlice.alllayertests[currentSliceLayer];
                panel.setSliceHeight(threadSlice.alllayertesstsHeight[currentSliceLayer]);

                if(layertest.getCommands().size()>10000){
                    cout<<"now z:"<<panel.getSliceHeight()<<" outline count:"<<layertest.getOutline().size()<<" commands count:"<<layertest.getCommands().size()<<endl;
                    for(int i=0;i<layertest.getOutline().size();i++){
                        if(layertest.getOutline()[i].size()>10000){
                            
                            cout<<"now "<<i<<":"<<layertest.getOutline()[i].size()<<endl;
                            cout<<"now we print the line"<<endl;
                            for(int j=0;j<layertest.getOutline()[i].getVertices().size();j++){
                                cout<<":"<<layertest.getOutline()[i].getVertices()[j]<<endl;
                                
                            }
                            cout<<"now we end print the line"<<endl;
                        }
                    }
                }
                
                drawFBO(layertest);
                if(panel.needOutput()==true){// need output?
                    if(currentSliceLayer==allSliceLayerCount){
                        outputManager.setLastPic();
                    }
                    if(outputManager.usingSVG==true){
                        outputManager.saveImage(layertest,panel.getSliceHeight());
                    }else{
                        drawFBO(layertest);
                        fbo.readToPixels(pixelsbuffer);
                        outputManager.saveImage(pixelsbuffer,panel.getSliceHeight());
                    }
                }
                if(currentSliceLayer==allSliceLayerCount){
                    easyLogTime.to("output to thread");
                    panel.ShowingAllSlice=false;
                }else{
                    panel.setSliceShowing();
                }
            }
            panel.iShowAllSliceLayerCount++;

        }
    }
}

void ofApp::checkSliceHeightChange(){
    if(panel.isSliceHeightUpdated()){
        //cout<<"we slice at now "<<endl;
        plate.sliceAt(panel.getSliceHeight());
        plate.update();
        if(panel.ShowingAllSlice==false){
            if(threadSlice.isAllSliced()){
                ofIndexType layerindex= panel.getSliceHeight()/threadSlice.allthickness;
                if(layerindex<threadSlice.alllayertests.size()){
                    drawFBO(threadSlice.alllayertests[layerindex]);
                }else{
                    ofPath pclean;
                    drawFBO(pclean);
                }
            }
        }
    }
}
/**
 draw the fbo
 */

void ofApp::drawFBO(ofPath pathdraw){
    // now we try fbo
    
    fbo.begin();
    ofClear(ofColor::black);
    pathdraw.draw(0,0);
    fbo.end();
}





