#include "gkplate.h"

gkplate::gkplate(){
    
    size.x=1280;//126.72mm
    size.y=800;//74.88mm
    size.z=1393;//138mm
    groundheight=10;
    
}
gkplate::~gkplate(){

}


void gkplate::setup(){
    playground.set(size.x,size.y,groundheight);
    mplayground.setTranslation(0, 0, size.z-groundheight*0.5);
    
    outsideBox.set(size.x,size.y,size.z);
    moutsideBox.setTranslation(0, 0, size.z-400);
    
   
}

void gkplate::drawincamera(){
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
   

}
