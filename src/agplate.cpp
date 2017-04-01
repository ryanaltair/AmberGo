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
    cam.setDistance(140);
    cam.move(0,0,40);// lift up the cam so we see clear
    for(int i=0;i<5;i++){
        ofLight l;
        ofNode c;
        c.setPosition(0, 0, 0);
        l.lookAt(c);
        aroundLight.push_back(l);
        aroundLight[i].setup();
        
    }
    ofNode c;
    c.setPosition(0, 0, 0);
    if(1){
        int x,y,z;
        x=playground.getWidth()/2+10;
        y=playground.getHeight()/2+10;
        z=-10;
        aroundLight[0].setPosition(x, y, z);
        aroundLight[1].setPosition(-x, -y, z);
        aroundLight[2].setPosition(x, -y, z);
        aroundLight[3].setPosition(-x, y, z);
        aroundLight[4].setPosition(0, 0, 120);
        
    }
    
}

void agplate::update(){
    
}



void agplate::sliceAt(float Z){
    slicelayerZ=Z;
//    cout<<"plate slice at "<<Z<<endl;
}

void agplate::addModel(ofMesh model){
    drawmode=0;
    of3dPrimitive nodemodel;
    nodemodel.getMesh()=model;
    //    nodemodel.setUseVbo(true);
    if(nodemodels.size()>0){
        nodemodels[0]=nodemodel;
        nodemodels[1]=nodemodel;
    }else{
        nodemodels.push_back(nodemodel);
        nodemodels.push_back(nodemodel);
    }
    cout<<"add a model in plate"<<endl;
    
    
}

void agplate::drawincamera(ofRectangle view){
    cam.begin(view);
    ofBackground(255,255,255);// a white with bit yellow
    if(sliceLayPlaneEnable==1){
        
        ofSetColor(96,185,287);
        sliceLayPlane.setPosition(0, 0, slicelayerZ);
        sliceLayPlane.draw();
        
    }
    
    if(1){
        ofSetSmoothLighting(true);
        for(auto &light:aroundLight){
            //            light.draw();
            light.enable();
        }
    }
    drawModels();
    ofSetColor(124, 121, 119);// 100,149,237, CornflowerBlue
    playground.draw();// the ground
    
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        //  outsideBox.draw();  // the outsidebox
    }
    ofDisableLighting();
    cam.end();
    //envlight.disable();
}
void agplate::drawModels(){
    if(drawmode==1){
        return;
    }
    if(drawmode==0){
        
    }
    if(nodemodels.size()>0){
        ofSetColor(123, 123, 123);//
        
        nodemodels[0].setScale(scaleFactor.x, scaleFactor.y, scaleFactor.z);
        nodemodels[0].draw();
         ofSetColor(80, 80, 80);//
//        ofSetColor(ofColor::black);
        nodemodels[0].drawWireframe();
        
    }
    //envlight.disable();
    
}


