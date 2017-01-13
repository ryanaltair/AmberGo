#include "agplate.h"

agplate::agplate(){
    
    boxSize.x=126.72; //1280;//126.72mm
    boxSize.y=74.88; //800;//74.88mm
    boxSize.z=138;//1393;//138mm
    groundheight=2;
    mreset.setTranslation(0, 0, 0);
}
agplate::~agplate(){

}


void agplate::setup(){
    cam.orbit(0, 40, cam.getDistance());
    playground.set(boxSize.x,boxSize.y,groundheight);
    mplayground.setTranslation(0, 0,-groundheight*0.5);
    playground.setTransformMatrix(mplayground);
    outsideBox.set(boxSize.x,boxSize.y,boxSize.z);
    moutsideBox.setTranslation(0, 0, 0);
    outsideBox.setTransformMatrix(moutsideBox);
    sliceLayPlane.set(boxSize.x,boxSize.y,0.4);
    cam.setDistance(200);
    
 }

void agplate::update(){
    msliceLayPlane.setTranslation(0, 0, layertestZ);
}



void agplate::sliceAt(float Z){
    layertestZ=Z;

}

void agplate::addModel(ofMesh model){
    drawmode=0;
    nodemodel.getMesh()=model;
    if(models.size()>0){
        models[0]=model;
    }else{
        models.push_back(model);
    }
    cout<<"add a model in plate"<<endl;


}

void agplate::drawincamera(ofRectangle view){
    
    cam.begin(view);
    ofBackground(ofColor::lightBlue);
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
    ofSetColor(ofColor::darkCyan);
    if(drawmode==1){
        //assimploader.drawFaces();
        return;
    }
    if(models.size()>0){
        // the model
        //  models[0].draw();
    }
    if(drawmode==0){
    
    }
   
   
    nodemodel.draw();
    ofSetColor(ofColor::black);
    nodemodel.drawWireframe();
}

void agplate::setPosition(ofVec3f newpostion){
    nodemodel.setPosition(-newpostion);
    cout<<"position"<<ofToString(newpostion);
}

