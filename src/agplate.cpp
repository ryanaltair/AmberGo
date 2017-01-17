#include "agplate.h"

agplate::agplate(){
    
    boxSize.x=126.72; //1280;//126.72mm
    boxSize.y=74.88; //800;//74.88mm
    boxSize.z=138;//1393;//138mm
    groundheight=2;
    for(int i=0;i<4;i++){
        
        
    }
    if(1){
        int i=0;
        float z;
        for (int x=-5; x<=5; x+=10) {
            for (int y=-5; y<=5; y+=10) {
                if(x==-5||y==-5||x==5||y==5){
                    z=5;
//                    lights.push_back(envlight);
//                    lights[lights.size()-1].setPosition(x*20, y*20, z);
//                    
//                    lights[lights.size()-1].setup();
                    i++;
                }else{
                    z=100;
                    // lights[i].setDirectional();
                }
                
                
            }
        }
    }

    
}
agplate::~agplate(){
    
}


/**
 init the playground and box , and cam
 */
void agplate::setup(){
        //    envlight.setPosition(0, 0, 100);
    //    envlight.setAmbientColor(ofColor::darkCyan);
    //    envlight.setAreaLight(100, 100);
    //    envlight.setSpecularColor(ofColor::darkCyan);
    //    envlight.setup();
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
    ofSetColor(ofColor::darkCyan);
    playground.draw();// the ground
    
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        //  outsideBox.draw();  // the outsidebox
    }
    
    cam.end();
    //envlight.disable();
}
void agplate::drawModels(){
    if(drawmode==1){
        return;
    }
    if(drawmode==0){
        
    }
    if(1){
//        ofSetSmoothLighting(true);
//        for(auto &light:lights){
//            light.draw();
//            light.enable();
//        }
        
    }
    //    envlight.enable();
    //    envlight.draw();
    if(nodemodels.size()>0){
        
        ofSetColor(ofColor::darkCyan);
        nodemodels[0].draw();
        //        ofSetColor(ofColor::black);
        //        nodemodels[0].drawWireframe();
    }
//    ofDisableLighting();
    //envlight.disable();
    
}

void agplate::setPosition(ofVec3f newpostion){
    if(nodemodels.size()>0){
        nodemodels[0].setPosition(-newpostion);
        cout<<"position"<<ofToString(newpostion);
    }
}

