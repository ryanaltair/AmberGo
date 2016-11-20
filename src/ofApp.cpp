#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
	 
    playground.set(1280,800,10);
    playground.setPosition(0,0,-5);
    
    outsideBox.set(1280,800,800);
    outsideBox.setPosition(0, 0, 400);
    
    sliceLayPlane.set(1280,800,0.4);
  
   
    cam.setDistance(2000);
    readyModel = ofMesh::box(200.0, 200.0, 200.0);
    //layertestat(<#ofMesh mesh#>, <#float z#>) // to do
}

//--------------------------------------------------------------
void ofApp::update(){
    sliceHeight=sliceLayer*sliceLayerThickness;

   }

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
	
	
	cam.begin();
	
    
    // the model
    ofTranslate(0,0,100);
    ofSetColor(ofColor::blueSteel);
    readyModel.draw();
    ofTranslate(0,0,-100);
    // the ground
    ofSetColor(150);
    playground.draw();
    // the outsidebox
    if(outsideBoxEnable==1){
    ofSetColor(20, 20, 20, 50);
    outsideBox.draw();
    }
    if(sliceLayPlaneEnable==1){
        sliceLayPlane.setPosition(0,0,sliceHeight);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();}
	cam.end();
    
    if(1) {
        screenText.str("");
        screenText << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        ofDrawBitmapString(screenText.str().c_str(), 20, 20);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

}

//addons

void fillpointlinelist(ofMesh frommesh){
    vector<ofVec3f> point = frommesh.getVertices();
    ofVec3f a,b,c;
    ofIndexType ia,ib,ic;
    for(ofIndexType i=0;i<frommesh.getNumIndices();i+=3){
        ia=frommesh.getIndex(i);
        a=frommesh.getVertex(ia);
        
    }
}

ofVec3f ofApp::getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
    ofVec3f returnPoint;
    // compare the upper point and the down point
    if(pointDown.z>pointUp.z){
        float tempz=pointDown.z;
        pointDown.z=pointUp.z;
        pointUp.z=tempz;
    }
    //check if the line cross the z plane
    if(pointUp.z>z||pointDown.z<z){
        returnPoint.x=returnPoint.y=0;
        returnPoint.z=-1; //false
        return returnPoint;
    }
    // check if the line parallel to z plane
    if(pointDown.z==pointUp.z){
        returnPoint.x=returnPoint.y=0;
        returnPoint.z=-2; //
    }
    if(pointUp.z==z||pointDown.z==z){
        returnPoint.z=0;
        returnPoint.x=0;
        returnPoint.y=0;
        return returnPoint;
    }
    float zLength=(pointUp.z-pointDown.z);
    float z1Length=1/zLength;
    float kx=(pointUp.x-pointDown.x)*z1Length;
    returnPoint.x=(z-pointUp.z)*kx+pointUp.x;
    float ky=(pointUp.y-pointDown.y)*z1Length;
    returnPoint.y=(z-pointUp.z)*ky+pointUp.y;
    returnPoint.z=z;
    return returnPoint;
}
ofPath ofApp::layertestat(ofMesh mesh,float z){
    ofPath returnpath;
    for(ofIndexType i=0;i<mesh.getNumIndices();i+=3){
        // get the point first
        ofIndexType ia=mesh.getIndex(i);
        ofIndexType ib=mesh.getIndex(i+1);
        ofIndexType ic=mesh.getIndex(i+2);
        ofVec3f pa=mesh.getVertex(ia);
        ofVec3f pb=mesh.getVertex(ib);
        ofVec3f pc=mesh.getVertex(ic);
        int sa=1;// s =1 means above the z -1 means below the z we assume above
        int sb=1;
        int sc=1;
       // check if any point.z == z the layer
        if(i==0){ //the first point
            if(pa.z==z){
                returnpath.moveTo(pa.x,pa.y);
                continue;
            }
            if(pa.z==z){
                returnpath.moveTo(pa.x,pa.y);
                continue;
            }
            if(pa.z==z){
                returnpath.moveTo(pa.x,pa.y);
                continue;
            }
        }else{// not the first point
            if(pa.z==z){
                returnpath.lineTo(pa.x,pa.y);
                continue;
            }
            if(pa.z==z){
                returnpath.lineTo(pa.x,pa.y);
                continue;
            }
            if(pa.z==z){
                returnpath.lineTo(pa.x,pa.y);
                continue;
            }
            
            
        }
        // now we find how many point over the z and how many point below z
        
        // that can check which line will cross the layer z // maybe none
        
        int abovepoint=0;
        int belowpoint=0;
        if(pa.z>z){
            abovepoint++;
        }else{
            sa=-1;
            belowpoint++;
        }
        if(pb.z>z){
            abovepoint++;
        }else{
            sb=-1;
            belowpoint++;
        }
        if(pc.z>z){
            abovepoint++;
        }else{
            sc=-1;
            belowpoint++;
        }
        
        if(abovepoint==3||belowpoint==3){
            continue;// because the is now cross point
        }
        
        // get the middle point
        int middlepoint=0;// 0 = unknow 1= a 2 =b 3 =c
        int lowpoint=0;
        int highpoint=0;
        if(sb==sc){lowpoint=1;}
        if(sa==sc){lowpoint=2;}
        if(sa==sc){lowpoint=3;}
        
        
        //check cross line only two line will cross
        if(abovepoint==2){
            
            
        }else{// abovepoint=1
        
        }
        
        //
        // check which type triangle
        int triangletype=0;
        if(pa.z==pb.z){
            triangletype++;
        }
        if(pa.z==pc.z){
            triangletype++;
        }
        if(pc.z==pb.z){
            triangletype++;
        }
        
        
        
        // re order by the .z
        // and get how many line cross the layer z
        
        switch(triangletype){
            case 0: // a normal triangle
                
                break;
            case 1: //
                break;
            case 2://thie won't happen
                break;
            case 3: // the parelle triangle that need more reorder
                break;
            
        
        }
        
        
        //get the cross point at layer z
        ofVec3f pstart;
        ofVec3f pend;
        
        switch (lowpoint) {
            case 1:
                pstart=getLinePlaneIntersection(pa, pb, z);
                pend=getLinePlaneIntersection(pa, pc, z);
                break;
            case 2:
                pstart=getLinePlaneIntersection(pb, pa, z);
                pend=getLinePlaneIntersection(pb, pc, z);
                break;
            case 3:
                pstart=getLinePlaneIntersection(pc, pa, z);
                pend=getLinePlaneIntersection(pc, pb, z);
                break;
                
            default:
                break;
        }
        // add cross point to the returnpath
        if(i==0){ //the first point
            returnpath.moveTo(pstart.x,pstart.y);
            returnpath.moveTo(pend.x,pend.y);
            continue;
        }else{// not the first point
            returnpath.lineTo(pstart.x,pstart.y);
            returnpath.lineTo(pend.x,pend.y);
        
        }
        
    }
    returnpath.close();
    returnpath.setStrokeColor(ofColor::blue);
    returnpath.setFillColor(ofColor::red);
    returnpath.setFilled(true);
    returnpath.setStrokeWidth(2);
    return returnpath;

}





