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
    moutsideBox.setTranslation(0, 0, meshMin.z-400);
    
    sliceLayPlane.set(1280,800,0.4);
    cam.setDistance(2000);
    
    layertest=layertestat(readyModel, layertestZ,testtri);// to do
    
    
    layertestmove.glTranslate(200, 200, 0);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //sliceHeight=layertestZ;//sliceLayer*sliceLayerThickness;
    if(layertestZ!=layertestZlast||testtri!=testtrilast){
        layertest=layertestat(readyModel, layertestZ,testtri);// to do
        
        msliceLayPlane.setTranslation(0, 0, layertestZ);
        layertestZlast=layertestZ;
        testtrilast=testtri;
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
            testtri-=100;
            break;
        case OF_KEY_RIGHT:
            testtri+=100;
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
    ofVec3f zeroPoint;
    zeroPoint.x=zeroPoint.y=zeroPoint.z=0;
    // compare the upper point and the down point
    if(pointDown.z>pointUp.z){
        float tempz=pointDown.z;
        pointDown.z=pointUp.z;
        pointUp.z=tempz;
    }
    //check if the line cross the z plane
    if(pointUp.z>z||pointDown.z<z){
        
    }else{
        
        return zeroPoint;
    }
    if(pointUp.x==pointDown.x&&pointUp.y==pointDown.y){
        returnPoint.x=pointDown.x;
        returnPoint.y=pointDown.y;
    }else{
        float zLength=(pointUp.z-pointDown.z);
        float z1Length=1/zLength;
        float kx=(pointUp.x-pointDown.x)*z1Length;
        returnPoint.x=(z-pointUp.z)*kx+pointUp.x;
        float ky=(pointUp.y-pointDown.y)*z1Length;
        returnPoint.y=(z-pointUp.z)*ky+pointUp.y;
    }
    returnPoint.z=z;
    return returnPoint;
}
ofPath ofApp::layertestat(ofMesh mesh,float z,int tri){
    // debug only
    int outputPeak=1;
    int outputUncross=0;
    int uncrossface=0;
    int crossface=0;//normal cross
    int pointcount=0;
    ofPath returnpath;
    int indexcount=mesh.getNumIndices();
    cout<<"======================================================="<<"\n";
    
    
    for(ofIndexType i=0;i<indexcount&&i<tri*3;i+=3){
    //for(ofIndexType i=0;i<indexcount;i+=3){
        if(outputUncross==1)cout<<"------------------------------------------------------";
        if(outputUncross==1)cout<<"\n"<<"step:"<<i/3+1<<"\n";
        ofVec3f addpoint;
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
        if(1){ //the first point
            int atz=0;
            if(pa.z==z){
                addpoint=pa;
                pointcount++;
                returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
                atz++;
            }
            if(pb.z==z){
                addpoint=pb;
                pointcount++;
                returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
                atz++;
            }
            if(pc.z==z){
                addpoint=pc;
                pointcount++;
                returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
                atz++;
            }
            //returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
            if(atz>0){
                cout<<"atz:"<<atz<<"\n";
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
            if(outputUncross==1)cout<<"a.z:"<<pa.z<<" b.z:"<<pb.z<<" c.z:"<<pc.z<<" Z:"<<z<<"\n"<<"no cross points"<<"\n";
            continue;// because the is now cross point
        }
        cout<<"a.z:"<<pa.z<<" b.z:"<<pb.z<<" c.z:"<<pc.z<<" Z:"<<z<<"\n";
         cout<<"cross points"<<"\n";
        
        // get the high middle and low point
        ofVec3f ph;//point high
        ofVec3f pm;//point middle
        ofVec3f pl;//point low
        if(pa.z>pb.z){
            if(pb.z>pc.z){
                ph=pa;
                pm=pb;
                pl=pc;
            }else{
                if(pa.z>pc.z){
                    ph=pa;
                    pm=pc;
                    pl=pb;
                }else{// pa.z<pc.z
                    ph=pc;
                    pm=pa;
                    pl=pb;
                }
            }
        }else{//pa.z<pb.z
            if(pa.z>pc.z){
                ph=pb;
                pm=pa;
                pl=pc;
            }else{//pa.z<pc.z
                if(pb.z>pc.z){
                    ph=pb;
                    pm=pc;
                    pl=pa;
                }else{//pb.z<pc.z
                    ph=pc;
                    pm=pb;
                    pl=pa;
                }
            }
        }
        
        // check the middle point that can know which 2 line will cross the plane layer
        int crosstype=0;// 0:unknown 1:above middle -1:below middle
        if(pm.z<z){ // 2 point above z
            crosstype=1;
        }else{// pm.z>z // 2 point below z
            crosstype=-1;
        }
        // now we get the cross point
        ofVec3f pstart;
        ofVec3f pend;
        
        cout<<"crosstype:"<<crosstype<<"\n"<<"ph.z:"<<ph.z<<","<<" pm.z"<<pm.z<<" pl.z"<<pl.z<<"\n";
        switch (crosstype) {
            case 1:
                pstart=getLinePlaneIntersection(ph, pm, z);
                pend=getLinePlaneIntersection(ph, pl, z);
                break;
            case -1:
                pstart=getLinePlaneIntersection(pm, pl, z);
                pend=getLinePlaneIntersection(ph, pl, z);
                break;
            default:
                break;
        }
        
        cout<<"pstart:"<<pstart.x<<","<<pstart.y<<"\n";
        cout<<"pend:"<<pend.x<<","<<pend.y<<"\n";
        // add cross point to the returnpath
        addpoint=pstart;
        pointcount++;
        returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
        addpoint=pend;
        pointcount++;
        returnpath=addPointToPath(returnpath, addpoint.x, addpoint.y, i);
        continue;
        
        
    }
    
    cout<<"point:"<<indexcount<<"\n";

    cout<<"uncrossface:"<<uncrossface<<" pathpoint:"<<pointcount<<"\n";
    returnpath.close();
    returnpath.setStrokeColor(ofColor::blue);
    returnpath.setFillColor(ofColor::white);
    returnpath.setFilled(true);
    returnpath.setStrokeWidth(1);
    return returnpath;

}

ofPath ofApp::addPointToPath(ofPath path,float x,float y,ofIndexType i){
    ofPath returnpath;
    if(i==0){
        returnpath.moveTo(x,y);
    }else{
        returnpath=path;
        returnpath.lineTo(x,y);
    }
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





