#include "agplate.h"

agplate::agplate(){
    
    boxSize.x=1280;//126.72mm
    boxSize.y=800;//74.88mm
    boxSize.z=1393;//138mm
    groundheight=10;
    mreset.setTranslation(0, 0, 0);
}
agplate::~agplate(){

}


void agplate::setup(){
    playground.set(boxSize.x,boxSize.y,groundheight);
    mplayground.setTranslation(0, 0,-groundheight*0.5);
    playground.setTransformMatrix(mplayground);
    
    outsideBox.set(boxSize.x,boxSize.y,boxSize.z);
    moutsideBox.setTranslation(0, 0, 0);
    outsideBox.setTransformMatrix(moutsideBox);
    sliceLayPlane.set(1280,800,0.4);
    
    cam.setDistance(2000);
    
    mreadyModel.setTranslation(0, 0, modelSize.z*0.5);
}

void agplate::update(){
 mreadyModel.setTranslation(0, 0, 0);
    msliceLayPlane.setTranslation(0, 0, layertestZ);
   
    

}

void agplate::sliceAt(float Z){
    layertestZ=Z;

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

    if(sliceLayPlaneEnable==1){

        ofSetColor(255,0,0,127);
        sliceLayPlane.setTransformMatrix(msliceLayPlane);
        sliceLayPlane.draw();
        
    }
     drawModel();
     // the ground
    ofSetColor(150);
    playground.draw();
        // the outsidebox
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
           outsideBox.draw();
     }
   
    cam.end();
}
void agplate::drawModel(){
    if(models.size()>0){
        // the model
        ofSetColor(ofColor::darkCyan);
        models[0].draw();
    }

}


