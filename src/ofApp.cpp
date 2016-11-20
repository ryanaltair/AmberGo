#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
    
    //assimp model load
    assimpModel.loadModel("testcube.stl");
    assimpModel.calculateDimensions();
    //readyModel = ofMesh::box(300, 200, h);//cone(200.0, 200.0);
    readyModel=assimpModel.getMesh(0);
    meshScale=getScale(readyModel);
    meshMin=getMinPoint(readyModel);
    
    h=meshMin.z;
    //playground
    playground.set(1280,800,10);
    mplayground.setTranslation(0, 0, meshMin.z-5);
    
    outsideBox.set(1280,800,800);
    moutsideBox.setTranslation(0, 0, -400);
    
    sliceLayPlane.set(1280,800,0.4);
    cam.setDistance(2000);
    
    
    
    layertestmove.glTranslate(200, 200, 0);
    //layertest=layertestat(readyModel, layertestZ);// to do
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast){
        layertest=layertestat(readyModel, layertestZ,testtri);// to do
        
        msliceLayPlane.setTranslation(0, 0, layertestZ);
        layertestZlast=layertestZ;
    }

   }

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
    
    layertest.draw(300,300);
    
     cam.begin();
    
    
    
    // the model
    ofSetColor(ofColor::blueSteel);
    ofMultMatrix(mreadyModel);
    readyModel.draw();
    ofMultMatrix(mreset);
    
    // the ground
    ofSetColor(150);
    ofMultMatrix(mplayground);
    playground.drawWireframe();
    ofMultMatrix(mreset);
    
    // the outsidebox
    if(outsideBoxEnable==1){
        ofSetColor(20, 20, 20, 50);
        ofMultMatrix(moutsideBox);
        outsideBox.draw();
        ofMultMatrix(mreset);
        
    }
    if(sliceLayPlaneEnable==1){
        ofMultMatrix(msliceLayPlane);
        ofSetColor(255,0,0,127);
        sliceLayPlane.draw();
        ofMultMatrix(mreset);
        
    }
	cam.end();
    
    if(1) {
        screenText.str("");
        screenText << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        screenText << "layertestat:"<<ofToString(layertestZ)<<"\n";
        screenText << "testtri"<<testtri<<"\n";
        screenText << "assimpmodel:"<<meshScale.z<<"\n";
        //screenText << ofToString(layertest.);
        ofDrawBitmapString(screenText.str().c_str(), 20, 20);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            layertestZ++;
            break;
        case OF_KEY_DOWN:
            layertestZ--;
            break;
        case OF_KEY_LEFT:
            testtri--;
            break;
        case OF_KEY_RIGHT:
            testtri++;
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
ofPath ofApp::layertestat(ofMesh mesh,float z,int tri){
    // debug only
    int outputPeak=1;
    int uncrossface=0;
    int crossface=0;//normal cross
    ofPath returnpath;
    int indexcount=mesh.getNumIndices();
    cout<<"======================================================="<<"\n";
    
    cout<<"point:"<<indexcount<<"\n";
    for(ofIndexType i=0;i<indexcount;i+=3){
        cout<<"step:"<<i<<"\n"<<"\n";
        // get the point first
        ofIndexType ia=mesh.getIndex(i);
        ofIndexType ib=mesh.getIndex(i+1);
        ofIndexType ic=mesh.getIndex(i+2);
        ofVec3f pa=mesh.getVertex(ia);
        ofVec3f pb=mesh.getVertex(ib);
        ofVec3f pc=mesh.getVertex(ic);
       // cout<<"point:"<<i<<"\n";
       // cout<<"pa.z:"<<pa.z<<"\n";
        int sa=1;// s =1 means above the z -1 means below the z we assume above
        int sb=1;
        int sc=1;
       // check if any point.z == z the layer
        if(i==0){ //the first point
            if(pa.z==z){
                returnpath.moveTo(pa.x,pa.y);
                
                   if(outputPeak==1) cout<<"drawnow"<<pa.x<<" "<<pa.y<<"\n";
                
                continue;
            }
            if(pb.z==z){
                returnpath.moveTo(pb.x,pb.y);
                
                if(outputPeak==1)cout<<"drawnow"<<pb.x<<" "<<pb.y<<"\n";
                continue;
            }
            if(pc.z==z){
                returnpath.moveTo(pc.x,pc.y);
                if(outputPeak==1)cout<<"drawnow"<<pc.x<<" "<<pc.y<<"\n";
                
                
                continue;
            }
        }
        else{// not the first point
            if(pa.z==z){
                returnpath.lineTo(pa.x,pa.y);
                if(outputPeak==1)cout<<"drawnow"<<pa.x<<" "<<pa.y<<"\n";
                
                continue;
            }
            if(pb.z==z){
                returnpath.lineTo(pb.x,pb.y);
                if(outputPeak==1)cout<<"drawnow"<<pb.x<<" "<<pb.y<<"\n";
                
                continue;
            }
            if(pc.z==z){
                returnpath.lineTo(pc.x,pc.y);
                if(outputPeak==1)cout<<"drawnow"<<pc.x<<" "<<pc.y<<"\n";
                
                continue;
            }
            
            
        }
        // now we find how many point over the z and how many point below z
        
        // that can check which line will cross the layer z // maybe none
        
        int abovepoint=0;
        int belowpoint=0;
        cout<<"a.z:"<<pa.z<<" b.z:"<<pb.z<<" c.z:"<<pc.z<<" Z:"<<z<<"\n";
        if(pa.z>z){
            abovepoint++;
        }else{
            sa=-1;
            belowpoint++;
        }
        //cout<<"abovepoint:"<<abovepoint<<"\n";
        //cout<<"belowpoint:"<<belowpoint<<"\n";
        
        if(pb.z>z){
            abovepoint++;
        }else{
            sb=-1;
            belowpoint++;
        }
        //cout<<"abovepoint:"<<abovepoint<<"\n";
        //cout<<"belowpoint:"<<belowpoint<<"\n";
        
        if(pc.z>z){
            abovepoint++;
        }else{
            sc=-1;
            belowpoint++;
        }
        
        //cout<<"abovepoint:"<<abovepoint<<"\n";
        //cout<<"belowpoint:"<<belowpoint<<"\n";
        if(abovepoint==3||belowpoint==3){
            uncrossface++;
            cout<<"no cross points"<<"\n";
            continue;// because the is now cross point
        }
         cout<<"cross points"<<"\n";
        
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
        ofVec3f p0;
        ofVec3f p1;
        ofVec3f p2;
        
        
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
        
        cout<<"lowpoint is "<<lowpoint<<"\n";
        //get the cross point at layer z
        ofVec3f pstart;
        ofVec3f pend;
        
        switch (lowpoint) {
            case 1:
                pstart=getLinePlaneIntersection(pb, pa, z);
                pend=getLinePlaneIntersection(pc, pa, z);
                break;
            case 2:
                pstart=getLinePlaneIntersection(pc, pb, z);
                pend=getLinePlaneIntersection(pc, pb, z);
                break;
            case 3:
                pstart=getLinePlaneIntersection(pa, pc, z);
                pend=getLinePlaneIntersection(pb, pc, z);
                break;
                
            default:
                break;
        }
        cout<<"pstart:"<<pstart.x<<","<<pstart.y<<"\n";
        cout<<"pend:"<<pend.x<<","<<pend.y<<"\n";
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
    cout<<"uncrossface:"<<uncrossface<<"\n";
    returnpath.close();
    returnpath.setStrokeColor(ofColor::blue);
    returnpath.setFillColor(ofColor::red);
    returnpath.setFilled(false);
    returnpath.setStrokeWidth(8);
    return returnpath;

}
ofVec3f ofApp::getScale(ofMesh mesh){
    ofVec3f a;
    a.x=a.y=a.z=0;
    ofVec3f b;
    b.x=b.y=b.z=0;
    
    ofVec3f scale;
    for (ofIndexType i=0; i<mesh.getNumVertices(); i++) {
        // a
        if(mesh.getVertex(i).x>=a.x){
            a.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y>=a.y){
            a.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z>=a.z){
            a.z=mesh.getVertex(i).z;
        }
        
        //b
        if(mesh.getVertex(i).x<=b.x){
            b.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y<=b.y){
            b.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z<=b.z){
            b.z=mesh.getVertex(i).z;
        }
    }
    scale=a-b;
    return scale;
    
}
ofVec3f ofApp::getMinPoint(ofMesh mesh){
    ofVec3f a;
    a.x=a.y=a.z=0;
    ofVec3f b;
    b.x=b.y=b.z=0;
    
    ofVec3f scale;
    for (ofIndexType i=0; i<mesh.getNumVertices(); i++) {
        // a
        if(mesh.getVertex(i).x>=a.x){
            a.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y>=a.y){
            a.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z>=a.z){
            a.z=mesh.getVertex(i).z;
        }
        
        //b
        if(mesh.getVertex(i).x<=b.x){
            b.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y<=b.y){
            b.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z<=b.z){
            b.z=mesh.getVertex(i).z;
        }
    }
    //scale=a-b;
    return b;
    
}
ofVec3f ofApp::getMaxPoint(ofMesh mesh){
    ofVec3f a;
    a.x=a.y=a.z=0;
    ofVec3f b;
    b.x=b.y=b.z=0;
    
    ofVec3f scale;
    for (ofIndexType i=0; i<mesh.getNumVertices(); i++) {
        // a
        if(mesh.getVertex(i).x>=a.x){
            a.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y>=a.y){
            a.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z>=a.z){
            a.z=mesh.getVertex(i).z;
        }
        
        //b
        if(mesh.getVertex(i).x<=b.x){
            b.x=mesh.getVertex(i).x;
        }
        if(mesh.getVertex(i).y<=b.y){
            b.y=mesh.getVertex(i).y;
        }
        if(mesh.getVertex(i).z<=b.z){
            b.z=mesh.getVertex(i).z;
        }
    }
    //scale=a-b;
    return a;
    
}





