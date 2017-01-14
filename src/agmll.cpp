#include "agmll.h"

agmll::agmll(){
    divdH=1/dH;
    
}
agmll::~agmll(){
    mergedMesh.clear();
    
}
void agmll::setup(ofMesh mesh){
    mergedMesh.clear();
    mergedMesh=mesh;
    indexsize=mesh.getNumIndices();
}
/**
 use swap to clean the vector
 */
void agmll::cleanmermory(){
    divdH=1/dH;
    horizonFacetHeightlist.clear();
    linelist.clear();
    pointlist.clear();
    linehorizonlist.clear();// is this line horizon true horizon // add with addnewline work with adddXdY
    nearpointlist.clear();//{pa pb} the index point to point list
    dXdYlist.clear();// add with addnewline work with adddXdY
    linecopymap.clear();
    vector<ofIndexType>().swap(linelist);// use for really clean memory
    vector<ofVec3f>().swap(pointlist);
    vector<bool>().swap(linehorizonlist);// use for really clean memory
    vector<ofIndexType>().swap(nearpointlist);
    vector<float>().swap(dXdYlist);
    
    
    
    //
}
void agmll::calcaulateModel(){
    cleanmermory();
    //calc work here
    mergedMesh.mergeDuplicateVertices();// step 1: merge mesh points
    addpointlist();
    getScale();
    addFacet();  // step 2: add line list
    adddXdY();
    //printsize();
    return;
    
}


// public
ofPath agmll::layertestat(float z) {
    
    // printlineandpoint();
    //alluntouched();
    for(ofIndexType i=0;i<touchedlist.size();i=i+2){
        touchedlist[i]=isUntouched;
    }
    
    z += meshMin.z;
    size_t findzhorizon=ofFind(horizonFacetHeightlist, z);
    if(findzhorizon<horizonFacetHeightlist.size()){// means find it is horizon facet
        if(findzhorizon+1!=horizonFacetHeightlist.size()){
            float zmoveup=horizonFacetHeightlist[findzhorizon+1]-z;
            z=z+zmoveup/2;// now we safa to slice at z
            
        }else{
            z=z+0.001;
        }
    }
    ofPath returnpath;
    ofIndexType continueflag = 0;
    for (ofIndexType loopcout = 0; loopcout < linelist.size(); loopcout ++) {// every loop build a closed path
        ofIndexType iCross = findcrosspointat(continueflag,z);//find a cross point
        continueflag=iCross+2;
        if (iCross == linelist.size()) { // iCross== linelist.size means we failed to find the cross point
            break;// that we failed to a slice layer because there is nothing on this layer
        }else{
            returnpath.append(layertestcloseloop(z,iCross));// add the indie loop to the returnpath
        }
    }
    returnpath.close();
    returnpath.setStrokeColor(ofColor::white);
    returnpath.setFilled(false);
    returnpath.setStrokeWidth(10);
    if(1){
        returnpath.setPolyWindingMode(OF_POLY_WINDING_ODD);
        returnpath.setStrokeColor(ofColor::blue);
        returnpath.setFillColor(ofColor::white);
        returnpath.setFilled(true);
        returnpath.setStrokeWidth(1);
    }
    return returnpath;
}
/**
 one closed loop path in Z , stalk from that linelist[iBegin]
 
 @param z slice height
 @param ipbegin linelist[ipbegin,ipbein]
 @return <#return value description#>
 */
ofPath agmll::layertestcloseloop(float z,ofIndexType iBegin){
    ofPath layerisland;
    ofIndexType i0,i1;
    ofIndexType iStart0,iStart1,ipStartA;
    ofIndexType ipHigh,ipLow;
    ofIndexType ipNearA,ipNearB;
    ofIndexType ipUsed,ipNext0,ipNext1;
    ofVec3f oldpoint;
    // ready to go
    if(1){
        i0=iBegin;
        i1=i0+1;
        ipNearA=nearpointlist[i0];
        ipNearB=nearpointlist[i1];
        ////cout<<"we find the cross point done"<<"\n";
        // get the first point XY
        // get the ph pl
        ipLow=linelist[i0];
        ipHigh=linelist[i1];
        //get the XY and move to
        ofVec3f XYpoint=getXY(pointlist[ipHigh], pointlist[ipLow], dXdYlist[i0], dXdYlist[i1], divdH, z);
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
            agline nextline;
            nextline.set(ipNext0,ipNext1);
            i0=searchline(nextline);
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
            ofVec3f XYpoint = getXY(pointlist[ipHigh], pointlist[ipLow], dXdYlist[i0], dXdYlist[i1], divdH, z);
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

/**
 add linelist and nearpointlist
 */
void agmll::addFacet(){
    // get the point first
    ofIndexType ipA;
    ofIndexType ipB;
    ofIndexType ipC;
    ofVec3f pa;
    ofVec3f pb;
    ofVec3f pc;
    ofIndexType ifacet;
    counter0=0;
    counter1=0;
    ofIndexType facetcount=0;
    
    for(ifacet=0;ifacet<mergedMesh.getNumIndices();ifacet+=3){//ifacet>0
        // get the point first
        ipA=mergedMesh.getIndex(ifacet);
        ipB=mergedMesh.getIndex(ifacet+1);
        ipC=mergedMesh.getIndex(ifacet+2);
        agline lineAB,lineAC,lineBC;
        vector<agline> lines;
        vector<ofIndexType> sidepoint;
        lines.resize(3);
        sidepoint.resize(3);
        lines[0].set(ipA,ipB);
        sidepoint[0]=ipC;
        lines[1].set(ipA,ipC);
        sidepoint[1]=ipB;
        lines[2].set(ipB,ipC);
        sidepoint[2]=ipA;
        
        //check if a horizon facet
        float zhorizon=pointlist[ipA].z;
        if(zhorizon==pointlist[ipB].z&&zhorizon==pointlist[ipC].z){//
            if(ofContains(horizonFacetHeightlist, zhorizon)==false){
                horizonFacetHeightlist.push_back(zhorizon);
                facetcount++;
            }
        }
        
        //add lines to the linelist
        for(int i=0;i<3;i++){
            int indexoldline=searchline(lines[i]);
            ofIndexType ipn=sidepoint[i];
            if(indexoldline<linelist.size()){
                //addoldline(indexoldline, ipn);
                if(nearpointlist[indexoldline]==nearpointlist[indexoldline+1]){
                    nearpointlist[indexoldline+1]=ipn;
                    //return;
                }else{
                    cout<<"waring!! add old line failed"<<endl;
                }
                
                counter1++;
            }else{
                agline newline=zsortline(lines[i]);
                linecopymap[newline]=linelist.size();
                linelist.push_back(newline.ip0);
                linelist.push_back(newline.ip1);
                nearpointlist.push_back(ipn);
                nearpointlist.push_back(ipn);
                counter0++;
            }
        }
    }
    
    
    ofSort(horizonFacetHeightlist);
    if(/* DISABLES CODE */ (0)){
        for(int i=0;i<horizonFacetHeightlist.size();i++){
            cout<<i<<":"<<horizonFacetHeightlist[i]<<endl;
        }
    }
    //now we add the rest list
    size_t linelistlen=linelist.size();
    dXdYlist.assign(linelistlen, 0);
    linehorizonlist.assign(linelistlen, false);
    linetypelist.assign(linelistlen, false);
    touchedlist.assign(linelistlen, 0);
    
    //check out
    cout<<"horizong facet:"<<facetcount<<":"<<(counter0+counter1)/3<<", "<<(counter0+counter1)<<"?="<<mergedMesh.getNumIndices()<<endl;
    cout<<"add new"<<counter1<<":"<<counter0<<endl;
    if(counter1!=counter0){
        cout<<"!!!!add face get problem!!!!!"<<endl;
    }
    printsize();
    
    
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

void agmll::justtouch(ofIndexType i){
    touchedlist[i]=isTouched;
}


void agmll::addpointlist(){
    ofMesh mesh=mergedMesh;
    pointlist.swap(mesh.getVertices());
    
    
    
}

//do once


//do loop

// tools
ofVec3f agmll::getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float divdH,float z){
    ofVec3f returnpoint;
    float h=z-pL.z;
    float divdHxh=divdH*h;
    returnpoint.x=pL.x+dX*divdHxh;
    returnpoint.y=pL.y+dY*divdHxh;
    returnpoint.z=z;
    return returnpoint;
}


agline agmll::zsortline(agline line){
    ofIndexType ip0,ip1;
    ip0=line.ip0;
    ip1=line.ip1;
    if(ip0>pointlist.size()||ip1>pointlist.size()){
        cout<<"we get a bigggggg ip "<<ip0<<"\t"<<ip1<<"\t"<<pointlist.size()<<endl;
    }
    agline newline;
    if(pointlist[ip0].z<pointlist[ip1].z){
        newline.set(ip0,ip1);
    }else if(pointlist[ip0].z>pointlist[ip1].z){// ip1<ip0
        newline.set(ip1,ip0);
    }else{   // if they the same      // make sure ip0<ip1
        if(ip0<=ip1){
            newline.set(ip0,ip1);
        }else{
            newline.set(ip1,ip0);
        }
    }
    return newline;
}

/**
 find the findline in linelist which need linecopymap help
 
 @param agline we will find
 @return i that from linelist[i]=findline // failed whene i =linelist.size()
 */
ofIndexType agmll::searchline( agline line){
    
    auto it=linecopymap.find(zsortline(line));
    if(it==linecopymap.end()){
        //not found
        return linelist.size();
    }else{
        //found
        return it->second;
    }
}

ofIndexType agmll::findcrosspointat(ofIndexType startflag,float z){
    ofIndexType i0=0;//linelist[ip0]
    ofIndexType i1=0;//linelist[ip1]
    // find a cross point
    for(int i=startflag;i<linelist.size();i+=2){
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
            startflag=i0+2;
            return i0;
            break;
        }
    }
    startflag=i0+2;
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
bool agmll::isPointPlaneCross(ofVec3f pointHigher, ofVec3f pointLower, float planeatz){
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
        return;
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
