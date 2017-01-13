#include "agplate.h"

agplate::agplate(){
    
    boxSize.x=126.72; //1280;//126.72mm
    boxSize.y=74.88; //800;//74.88mm
    boxSize.z=138;//1393;//138mm
    groundheight=2;
}
agplate::~agplate(){
    
}


/**
 init the playground and box , and cam 
 */
void agplate::setup(){
    //cam.orbit(180, 0, cam.getDistance());
    cam.orbit(0, 40, cam.getDistance());
    playground.set(boxSize.x,boxSize.y,groundheight);
    playground.setPosition(0, 0,-groundheight*0.5);
    outsideBox.set(boxSize.x,boxSize.y,boxSize.z);
    outsideBox.setPosition(0,0,0);
    sliceLayPlane.set(boxSize.x,boxSize.y,0.4);
    cam.setDistance(200);
    
}

void agplate::update(){
    
}



void agplate::sliceAt(float Z){
    layertestZ=Z;
    
}

void agplate::addModel(ofMesh model){
    drawmode=0;
    of3dPrimitive nodemodel;
    nodemodel.getMesh()=model;
    if(nodemodels.size()>0){
        nodemodels[0]=nodemodel;
    }else{
        nodemodels.push_back(nodemodel);
    }
    cout<<"add a model in plate"<<endl;
    
    
}

void agplate::drawincamera(ofRectangle view){
    
    cam.begin(view);
    ofBackground(ofColor::lightBlue);
    if(sliceLayPlaneEnable==1){
        
        ofSetColor(255,0,0,127);
        sliceLayPlane.setPosition(0, 0, layertestZ);
        sliceLayPlane.draw();
        
    }
    drawModels();
    ofSetColor(150);
    playground.draw();// the ground

    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        outsideBox.draw();  // the outsidebox
    }
    
    cam.end();
}
void agplate::drawModels(){
    if(drawmode==1){
        return;
    }
    if(drawmode==0){
        
    }
    if(nodemodels.size()>0){
        ofSetColor(ofColor::darkCyan);
        nodemodels[0].draw();
        ofSetColor(ofColor::black);
        nodemodels[0].drawWireframe();
    }
}

void agplate::setPosition(ofVec3f newpostion){
    if(nodemodels.size()>0){
        nodemodels[0].setPosition(-newpostion);
        cout<<"position"<<ofToString(newpostion);
    }
}

