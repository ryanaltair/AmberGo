#include "agplate.h"

agplate::agplate(){
    
    boxSize.x=1280;//126.72mm
    boxSize.y=800;//74.88mm
    boxSize.z=1393;//138mm
    groundheight=10;
    
}
agplate::~agplate(){

}


void agplate::setup(){
    playground.set(boxSize.x,boxSize.y,groundheight);
    mplayground.setTranslation(0, 0,-groundheight*0.5);
    outsideBox.set(boxSize.x,boxSize.y,boxSize.z);
    moutsideBox.setTranslation(0, 0, 0);
    sliceLayPlane.set(1280,800,0.4);
    
    cam.setDistance(2000);
    
    mreadyModel.setTranslation(0, 0, modelSize.z*0.5);
}

void agplate::update(){

    msliceLayPlane.setTranslation(0, 0, layertestZ);
    if(sliceLayPlaneEnable==1){
        ofMultMatrix(msliceLayPlane);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();
        ofMultMatrix(mreset);
        
    }
    

}

void agplate::addModel(ofMesh model){
    if(models.size()>0){
        models[0]=model;
    }else{
   models.push_back(model);
    }
    cout<<"add a model in plate"<<endl;


}
void agplate::drawincamera(){
    cam.begin();
    
    drawModel();
   
    // the ground
    ofSetColor(150);
    ofMultMatrix(mplayground);
    playground.draw();
    ofMultMatrix(mreset);
    
    
    
    // the outsidebox
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        ofMultMatrix(moutsideBox);
        outsideBox.draw();
        ofMultMatrix(mreset);
        
    }
   
    cam.end();
}
void agplate::drawModel(){
    if(models.size()>0){
        
        // the model
        ofSetColor(ofColor::darkCyan);
        ofMultMatrix(mreadyModel);
        models[0].draw();
        ofMultMatrix(mreset);
        //cout<<"draw a model on plate"<<endl;
    }

}


