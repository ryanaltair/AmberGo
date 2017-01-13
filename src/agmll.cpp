#include "agmll.h"

agmll::agmll(){
    
    
}
agmll::~agmll(){
    mergedMesh.clear();
    
}
void agmll::setup(ofMesh mesh){
    //clean up
    cleanmermory();
    mergedMesh.clear();
    mergedMesh=mesh;
}
void agmll::cleanmermory(){
    
    linelist.clear();
    //vector<ofIndexType>().swap(linelist);// use for really clean memory
    pointlist.clear();
    //vector<ofVec3f>().swap(pointlist);
    linehorizonlist.clear();// is this line horizon true horizon // add with addnewline work with adddXdY
    //vector<bool>().swap(linehorizonlist);// use for really clean memory
    nearpointlist.clear();//{pa pb} the index point to point list
    //vector<ofIndexType>().swap(nearpointlist);
    dXdYlist.clear();// add with addnewline work with adddXdY
    //vector<float>().swap(dXdYlist);
    //
}
void agmll::calcaulateModel(){
    //this update will put inside the app update in old day
    // new this will be excute in thread
    //calc work here
    mergedMesh.mergeDuplicateVertices();// step 1: merge mesh points
    addpointlist(mergedMesh);
    getScale(mergedMesh);
    addface();  // step 2: add line list
    adddXdY();
    return;
    
}


// public
ofPath agmll::layertestat(float z) {
    // printlineandpoint();
    //alluntouched();
    for(ofIndexType i=0;i<touchedlist.size();i=i+2){
        touchedlist[i]=isUntouched;
    }
    continueflag = 0;
    z += meshMin.z;
    ofPath returnpath;
    ofIndexType closetestpoint;
    for ( closetestpoint = 0; closetestpoint < linelist.size(); closetestpoint += 2) {
        
        //step start
        //find a cross point
        if (continueflag == linelist.size()) {
            break;
        }
        ofIndexType iCross = findcrosspointat(z);
        if (iCross == linelist.size()) { // iCross== linelist.size means we failed to find the cross point
            //    cout<<"find nothing yet"<<endl;
            break;// that we failed to a slice layer because there is nothing on this layer
        }
        // cout<<"find something yet"<<endl;
        returnpath.append( layertestcloseloop(z,iCross));// add the indie loop to the returnpath
        // cout<<"now we close the sub path"<<endl;
    }
    // cout<<"------layertest end------"<<endl;
    returnpath.close();
    //    cout << "continepoint=" << closetestpoint << endl;
    returnpath.setStrokeColor(ofColor::white);
    returnpath.setFilled(false);
    returnpath.setStrokeWidth(10);
    if(0){
        returnpath.setPolyWindingMode(OF_POLY_WINDING_ODD);
        returnpath.setStrokeColor(ofColor::blue);
        returnpath.setFillColor(ofColor::white);
        returnpath.setFilled(true);
        returnpath.setStrokeWidth(1);
    }
    // cout<<"windingmode"<<returnpath.getWindingMode()<<endl;
    
    return returnpath;
}
ofPath agmll::layertestcloseloop(float z,ofIndexType ipbegin){
    ofPath layerisland;
    ofIndexType i0,i1;
    ofIndexType iStart0,iStart1,ipStartA;
    ofIndexType ipHigh,ipLow;
    ofIndexType ipNearA,ipNearB;
    ofIndexType ipUsed,ipNext0,ipNext1;
    ofVec3f oldpoint;
    // ready to go
    if(1){
        i0=ipbegin;
        i1=i0+1;
        ipNearA=nearpointlist[i0];
        ipNearB=nearpointlist[i1];
        ////cout<<"we find the cross point done"<<"\n";
        // get the first point XY
        // get the ph pl
        ipLow=linelist[i0];
        ipHigh=linelist[i1];
        //get the XY and move to
        ofVec3f XYpoint=getXY(pointlist[ipHigh], pointlist[ipLow], dXdYlist[i0], dXdYlist[i1], dH, z);
        oldpoint=XYpoint;
        layerisland.moveTo(XYpoint.x,XYpoint.y);
        
        // set pstart and pnext
        iStart0=i0;
        iStart1=i1;
        ipStartA=ipNearA;
        ipNext0=ipNearB;
    }
    //step loop
    //    cout<<"loop start"<<endl;
    int loopcount = 0;
    bool isloopover = false;
    int finalsteptogo = 1;
    unsigned long i ;
    for ( i = 0; i < linelist.size() + 1; i += 1) {
        if (isloopover != true && ipNext0 == ipStartA) {
            isloopover = true;
            //            cout<<"now loop will over soon"<<" : ";
            //            cout<<ipNext0<<" to "<<ipStartA<<endl;
            //            cout<<i<<" to "<<ipStartA<<endl;
        }
        //checkpnextZ
        float nextZ=z;
        if(ipNext0>pointlist.size()){
            cout<<"wrong!!!!!!!!"<<endl;
        }
        bool nextLineTheoryUseHigh;
        float nextpointZ=pointlist[ipNext0].z;
        if(nextpointZ>=pointlist[ipHigh].z){
            nextLineTheoryUseHigh=false;
        }else if(nextpointZ<=pointlist[ipLow].z){
            nextLineTheoryUseHigh=true;
        }else{
            if(nextpointZ<=z){
                nextLineTheoryUseHigh=true;
            }else{
                nextLineTheoryUseHigh=false;
            }
            
        }
        if(nextLineTheoryUseHigh==true){
            ipUsed=ipLow;
            ipNext1=ipHigh;
            
        }else{
            ipUsed=ipHigh;
            ipNext1=ipLow;
        }
        
        //
        if(1){
            i0=searchline(ipNext0,ipNext1);
            //cout<<"i0:"<<i0<<":"<<linelist.size()<<":"<<pointlist.size()<<endl;
            i1=i0+1;
            ipNearA=nearpointlist[i0];
            ipNearB=nearpointlist[i1];
            justtouch(i0);
            ipLow=linelist[i0];
            ipHigh=linelist[i1];
            if(ipHigh>=pointlist.size()){
                debuglinelist(i0);
                cout<<"EXC_BAD_ACESS"<<ipHigh<<":"<<pointlist.size()<<endl;
            }
            ofVec3f XYpoint = getXY(pointlist[ipHigh], pointlist[ipLow], dXdYlist[i0], dXdYlist[i1], dH, z);
            if(oldpoint!=XYpoint){
                layerisland.lineTo(XYpoint.x,XYpoint.y);
            }
            oldpoint=XYpoint;
            if (isloopover == true) {
                //check if it the last line?
                //                i--;
                if (i0 == iStart0 && i1 == iStart1) {
                    finalsteptogo = 0;
                    //                    cout<<"we find the last line right way"<<"\n";
                }
                if (i1 == iStart0 && i0 == iStart1) {
                    finalsteptogo = 0;
                    //                    cout<<"we find the last line wrong way "<<"\n";
                }
                if (finalsteptogo == 0) {
                    //                     cout<<"now we go out"<<"\n";
                    //                    cout<<ipNext0<<" to "<<ipStartA<<endl;
                    //                    cout<<i<<" to "<<ipStartA<<endl;
                    break;
                }
            }
            // set pnext
            if (ipUsed == ipNearA) {
                ipNext0 = ipNearB;
            } else { //ipUsed==ipNearB
                ipNext0 = ipNearA;
            }
            // modify the z if we need
        }
    }
    if(i==linelist.size()){
        
        cout<<"we get something wrong"<<endl;
    }
    layerisland.close();
    return layerisland;
}
ofVec3f agmll::getScale(){
    ofMesh mesh=mergedMesh;
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
    meshScale=scale;
    meshMin=b;
    meshMax=a;
    return scale;
    
}

ofVec3f agmll::getScale(ofMesh mesh){
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
    meshScale=scale;
    meshMin=b;
    meshMax=a;
    return scale;
    
}

void agmll::addface(){
    // get the point first
    ofIndexType ipA;
    ofIndexType ipB;
    ofIndexType ipC;
    ofVec3f pa;
    ofVec3f pb;
    ofVec3f pc;
    ofIndexType i=0;
    //cout<<"ipA,ipB,ipC"<<ipA<<ipB<<ipC<<"\n";
    counter0=0;
    counter1=0;
    
    for(i=0;i<mergedMesh.getNumIndices();i+=3){//i>0
        // get the point first
        ipA=mergedMesh.getIndex(i);
        ipB=mergedMesh.getIndex(i+1);
        ipC=mergedMesh.getIndex(i+2);
        //pa=mergedMesh.getVertex(ipA);
        //pb=mergedMesh.getVertex(ipB);
        //pc=mergedMesh.getVertex(ipC);
        
        // the first line ab
        int oldline0=searchline(ipA,ipB);
        // line exist and not
        // ab
        if(oldline0<linelist.size()){
            addoldline(oldline0, ipC);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ipA, ipB, ipC);
        }
        
        // the second line ac
        int oldline1=searchline(ipA,ipC);
        
        //ac
        if(oldline1<linelist.size()){
            addoldline(oldline1, ipB);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ipA, ipC, ipB);
        }
        
        //the third line bc
        int oldline2=searchline(ipC,ipB);
        //bc
        if(oldline2<linelist.size()){
            addoldline(oldline2, ipA);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ipC, ipB, ipA);
        }
        // new line
    }
}
void agmll::adddXdY(){
    for(ofIndexType i=0;i<linelist.size();i+=2){
        //   cout<<"adddxdy"<<i<<endl;
        ofVec3f p0,p1,pL,pH;
        p0=pointlist[linelist[i]];
        p1=pointlist[linelist[i+1]];
        // if horizonline
        if(p0.z==p1.z){
            addlinetype(i,horizonline,evenline);
            addtouchedlist(i, isUntouched, p0.z);
            linehorizonlist[i]=true;
            linehorizonlist[i+1]=true;
            continue;
        }
        //if line vertical
        if(p0.x==p1.x&&p0.y==p1.y){
            if(p0.z>p1.z){
                addlinetype(i,verticalline,fallline);
                addtouchedlist(i, isUntouched, p0.z);
                
            }else{//p0.z<p1.z
                addlinetype(i, verticalline, riseline);
                addtouchedlist(i, isUntouched, p1.z);
                
            }
            
            dXdYlist[i]=0;
            dXdYlist[i+1]=0;
        }
        
        //get the high point and low point
        if(p0.z>p1.z){
            pL=p1;
            pH=p0;
            addlinetype(i,bevelline,fallline);
            addtouchedlist(i, isUntouched, p0.z);
            
        }else{// p0.z<p1.z
            pL=p0;
            pH=p1;
            addlinetype(i,bevelline,riseline);
            addtouchedlist(i, isUntouched, p1.z);
            
        }
        float H,divH;
        H=pH.z-pL.z;
        divH=1/H;
        if(pH.x==pL.x){
            dXdYlist[i]=0;
        }else{
            dXdYlist[i]=(pH.x-pL.x)*divH*dH;
        }
        if(pH.y==pL.y){
            dXdYlist[i+1]=0;
        }else{
            dXdYlist[i+1]=(pH.y-pL.y)*divH*dH;
        }
    }
}
void agmll::addlinetype(ofIndexType i,int linetype,int riseorfall){
    linetypelist[i]=linetype;
    linetypelist[i+1]=riseorfall;
    
}
void agmll::addtouchedlist(ofIndexType i,float isTouchedOrNot,float ZMax){
    touchedlist[i]=isTouchedOrNot;
    touchedlist[i+1]=ZMax;
    
}
void agmll::alluntouched(){
    for(ofIndexType i=0;i<touchedlist.size();i=i+2){
        touchedlist[i]=isUntouched;
    }
    
}
void agmll::justtouch(ofIndexType i){
    touchedlist[i]=isTouched;
}
// private:
//do in setup
void agmll::addpointlist(ofMesh mesh){
    
    pointlist.swap(mesh.getVertices());
    
}
//do once


//do loop

// tools
ofVec3f agmll::getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z){
    ofVec3f returnpoint;
    //z=z;
    
    float divdH=1/dH;
    float h=z-pL.z;
    
    returnpoint.x=pL.x+dX*divdH*h;
    returnpoint.y=pL.y+dY*divdH*h;
    returnpoint.z=z;
    //cout<<"getXY():"<<ofToString(returnpoint)<<" when pH:"<<ofToString(pH)<<" pL:"<<ofToString(pL)<<endl;
    return returnpoint;
}

void agmll::addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn){
    ////cout<<"ip0,ip1:"<<ip0<<","<<ip1<<"\n";
    ////cout<<"before"<<linelist.size();
    //check the ip first
    
    if(ip0>pointlist.size()||ip1>pointlist.size()){
        cout<<"we get a bigggggg ip "<<ip0<<"\t"<<ip1<<"\t"<<pointlist.size()<<endl;
    }
    
    if(pointlist[ip0].z<pointlist[ip1].z){
        // if(ip0<ip1){
        agline newline(ip0,ip1);
        linecopymap[newline]=linelist.size();
        linelist.push_back(ip0);
        linelist.push_back(ip1);
        //  cout<<"---------- < "<<"\n";
    }else if(pointlist[ip0].z>pointlist[ip1].z){// ip1<ip0
        agline newline(ip1,ip0);
        linecopymap[newline]=linelist.size();
        linelist.push_back(ip1);
        linelist.push_back(ip0);
        // cout<<"---------- > "<<"\n";
        //linecopymap[newline]=ip1;
        //cout<<"1<0"<<"\n";
    }else{
        // if they the same
        // make sure ip0<ip1
        if(ip0<ip1){
            agline newline(ip0,ip1);
            linelist.push_back(ip0);
            linelist.push_back(ip1);
            linecopymap[newline]=linelist.size();
            //     cout<<"---------- - "<<ip0<<":"<<ip1<<"\n";
        }else{
            agline newline(ip1,ip0);
            linelist.push_back(ip1);
            linelist.push_back(ip0);
            linecopymap[newline]=linelist.size();
            //     cout<<"---------- - "<<ip1<<":"<<ip0<<"\n";
        }
        
        
        
    }
    
    ////cout<<linelist[linelist.size()-2];
    
    ////cout<<linelist[linelist.size()-1];
    
    ////cout<<"\n";
    ////cout<<":"<<linelist.size()<<"\n";
    linehorizonlist.push_back(false);
    linehorizonlist.push_back(false);
    // linehashlist.push_back(ip0+ip1);
    //    linehashlist.push_back(0);
    dXdYlist.push_back(0);
    dXdYlist.push_back(0);
    
    nearpointlist.push_back(ipn);
    nearpointlist.push_back(ipn);
    
    linetypelist.push_back(0);
    linetypelist.push_back(0);
    touchedlist.push_back(0);
    touchedlist.push_back(0);
}
void agmll::addoldline(ofIndexType ipl,ofIndexType ipn){
    if(nearpointlist[ipl]==nearpointlist[ipl+1]){
        nearpointlist[ipl+1]=ipn;
        //    linehashlist[ipl+1]=1;
        return;
    }
}
/**
 find the findline in linelist which need linecopymap help
 
 @param ip0 the findline's ip0
 @param ip1 the findline's ip1
 @return i that from linelist[i]=findline // failed whene i =linelist.size()
 */
ofIndexType agmll::searchline(ofIndexType ip0,ofIndexType ip1){
    if(pointlist[ip0].z<pointlist[ip1].z){
        //     if(ip0<ip1){
        if(1){
            agline findline(ip0,ip1);
            auto it=linecopymap.find(findline);
            if(it==linecopymap.end()){
                //not found
                return linelist.size();
            }else{
                return it->second;
            }
        }
    }else if(pointlist[ip0].z>pointlist[ip1].z){// ip1<ip0
        if(1){
            agline findline(ip1,ip0);
            auto it=linecopymap.find(findline);
            if(it==linecopymap.end()){
                //not found
                return linelist.size();
            }else{
                return it->second;
            }
        }
    }else{//pointlist[ip0].z==pointlist[ip1].z
        if(ip0<ip1){
            agline findline(ip0,ip1);
            auto it=linecopymap.find(findline);
            if(it==linecopymap.end()){
                //not found
                return linelist.size();
            }else{
                return it->second;
            }
            
        }else{
            agline findline(ip1,ip0);
            auto it=linecopymap.find(findline);
            if(it==linecopymap.end()){
                //not found
                return linelist.size();
            }else{
                return it->second;
            }
            
        }
        
        
    }
    //counter1++;
    return -1;// false
}
ofVec3f agmll::getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
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

/**
 find the first line that we begin when slice at z
 TODO: we need a quickstartlist that hold every z start line
 @param z slice at z
 @return return the i for linelist[i]
 */
ofIndexType agmll::findcrosspointat(float z){
    ofIndexType i0=0;//linelist[ip0]
    ofIndexType i1=0;//linelist[ip1]
    // find a cross point
    for(int i=continueflag;i<linelist.size();i+=2){
        i0=i;
        i1=i+1;
        // we don't touch anything that already touched
        if(touchedlist[i0]==isTouched){
            // cout<<"it was touched"<<endl;
            continue;
        }else{
            // cout<<"it untouched "<<ip0<<endl;
        }
        
        justtouch(i0);
        // we don't touch when z>zmax
        if(z>touchedlist[i1]){
            // cout<<"it is ip0:"<<ip0<<">zmax"<<touchedlist[ip1]<<endl;
            continue;
        }
        if( isPointPlaneCross(i0, i1, linetypelist[i1], z)==true){
            // cout<<"we roll"<<endl;
            continueflag=i0+2;
            return i0;
            break;
        }
        
        
    }
    continueflag=i0+2;
    //  cout<<"we find nothing any more so we quit"<<endl;
    return linelist.size();
    
}
bool agmll::isPointPlaneCross(ofIndexType indexpoint0,ofIndexType indexpoint1,int riseorfall,float planeatz){
    ofVec3f pointLower,pointHigher;
    if(riseorfall==evenline){return false;}
    if(riseorfall==riseline){
        pointLower=pointlist[linelist[indexpoint0]];
        pointHigher=pointlist[linelist[indexpoint1]];
    }else if(riseorfall==fallline){
        pointLower=pointlist[linelist[indexpoint1]];
        pointHigher=pointlist[linelist[indexpoint0]];
        
    }
    
    if(pointLower.z<planeatz&&pointHigher.z>planeatz){
        // cout<<"get the cross line: "<<ofToString(pointHigher)<<","<<ofToString(pointLower)<<endl;
        return true;
    }
    
    
    
    return false;
    
}

void agmll::debuglinelist(ofIndexType index){
    
    
    cout<<"index0,index1:"<<index<<","<<index+1<<"   ";
    if(index+1>linelist.size()){
        cout<<"waring:ip>linelist.size"<<endl;
        //return;
    }else{
        cout<<"linelist.size:"<<linelist.size()<<"   ";
    }
    ofIndexType ip0=linelist[index];
    ofIndexType ip1=linelist[index+1];
    cout<<"point index is:"<<ip0<<":"<<ip1<<"   ";
    if(ip0>pointlist.size()||ip1>pointlist.size()){
        cout<<"waring:index of point > pointlist.size"<<endl;
        //return;
    }else{
        cout<<"pointlist.size:"<<pointlist.size()<<"   ";
    }
    ofPoint point0,point1;
    point0=pointlist[ip0];
    point1=pointlist[ip1];
    cout<<"line from:"<<ofToString(point0)<<" to "<<ofToString(point1)<<"   ";
    if(point0.x==point1.x&&point0.y==point1.y&&point0.z==point1.z){
        cout<<"this is a singlepoint;"<<endl;
    }else{
        cout<<"this is a line;"<<endl;
    }
    //cout<<"nearpointtype:"<<nearpointlist[ip]<<" dir:"<<nearpointlist[ip+1]<<endl;
    //cout<<"linetype:"<<linetypelist[ip]<<" dir:"<<linetypelist[ip+1]<<endl;
    
    
}
