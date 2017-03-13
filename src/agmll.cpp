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
    
    vector<ofIndexType>().swap(sliceModel.linelist);// use for really clean memory
    vector<ofVec3f>().swap(sliceModel.pointlist);
    vector<bool>().swap(sliceModel.linehorizonlist);// use for really clean memory
    vector<ofIndexType>().swap(sliceModel.nearpointlist);
    vector<float>().swap(sliceModel.dXdYlist);
    sliceModel.horizonFacetHeightlist.clear();
    sliceModel.linelist.clear();
    sliceModel.pointlist.clear();
    sliceModel.linehorizonlist.clear();// is this line horizon true horizon // add with addnewline work with adddXdY
    sliceModel.linehorizonlist.clear();//{pa pb} the index point to point list
    sliceModel.dXdYlist.clear();// add with addnewline work with adddXdY
    linecopymap.clear();
    
    
    
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
    for(ofIndexType i=0;i<sliceModel.linetypelist.size();i=i+2){
        sliceModel.linetypelist[i]=isUntouched;
    }
    
    z += meshMin.z;
    size_t findzhorizon=ofFind(sliceModel.horizonFacetHeightlist, z);
    if(findzhorizon<sliceModel.horizonFacetHeightlist.size()){// means find it is horizon facet
        if(findzhorizon+1!=sliceModel.horizonFacetHeightlist.size()){
            float zmoveup=sliceModel.horizonFacetHeightlist[findzhorizon+1]-z;
            if(zmoveup>dH){
                z=z+0.001;// now we safa to slice at z
            }else{
                z=z+zmoveup/2;// now we safa to slice at z
            }
        }else{
            z=z+0.001;
        }
    }
    ofPath returnpath;
    ofIndexType continueflag = 0;
    for (ofIndexType loopcout = 0; loopcout < sliceModel.linelist.size(); loopcout ++) {// every loop build a closed path
        ofIndexType iCross = findcrosspointat(continueflag,z);//find a cross point
        continueflag=iCross+2;
        if (iCross == sliceModel.linelist.size()) { // iCross== sliceModel.linelist.size means we failed to find the cross point
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
        returnpath.setStrokeColor(ofColor::white);
        returnpath.setFillColor(ofColor::white);
        returnpath.setFilled(true);
        returnpath.setStrokeWidth(1);
    }
    return returnpath;
}
/**
 one closed loop path in Z , stalk from that sliceModel.linelist[iBegin]
 
 @param z slice height
 @param ipbegin sliceModel.linelist[ipbegin,ipbein]
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
        ipNearA=sliceModel.nearpointlist[i0];
        ipNearB=sliceModel.nearpointlist[i1];
        ////cout<<"we find the cross point done"<<"\n";
        // get the first point XY
        // get the ph pl
        ipLow=sliceModel.linelist[i0];
        ipHigh=sliceModel.linelist[i1];
        //get the XY and move to
        ofVec3f XYpoint=getXY(sliceModel.pointlist[ipHigh], sliceModel.pointlist[ipLow], sliceModel.dXdYlist[i0], sliceModel.dXdYlist[i1], divdH, z);
        oldpoint=XYpoint;
        layerisland.moveTo(XYpoint.x,XYpoint.y);
        // set pstart and pnext
        iStart0=i0;
        iStart1=i1;
        ipStartA=ipNearA;
        ipNext0=ipNearB;
    }
    //step loop
    int loopcount = 0;
    bool isMeetPointStartA = false;
    unsigned long i ;
    for ( i = 0; i < sliceModel.linelist.size() + 1; i += 1) {
        // step test meet pointStartA
        if (isMeetPointStartA != true && ipNext0 == ipStartA) {
            isMeetPointStartA = true;
        }
        //checkpnextZ, figure out use which side as next line
        float nextZ=z;
        if(ipNext0>sliceModel.pointlist.size()){
            cout<<"wrong!!!!!!!!"<<endl;
        }
        bool nextLineUseUpperSide;
        float nextpointZ=sliceModel.pointlist[ipNext0].z;
        if(nextpointZ>=sliceModel.pointlist[ipHigh].z){
            nextLineUseUpperSide=false;
        }else if(nextpointZ<=sliceModel.pointlist[ipLow].z){
            nextLineUseUpperSide=true;
        }else{
            if(nextpointZ<=z){
                nextLineUseUpperSide=true;
            }else{
                nextLineUseUpperSide=false;
            }
        }
        // get the next line index
        if(nextLineUseUpperSide==true){
            ipUsed=ipLow;
            ipNext1=ipHigh;
            
        }else{
            ipUsed=ipHigh;
            ipNext1=ipLow;
        }
        
        if(true){// get the next line
            agline nextline;
            nextline.set(ipNext0,ipNext1);
            i0=searchline(nextline);
            //cout<<"i0:"<<i0<<":"<<sliceModel.linelist.size()<<":"<<sliceModel.pointlist.size()<<endl;
            i1=i0+1;
            ipNearA=sliceModel.nearpointlist[i0];
            ipNearB=sliceModel.nearpointlist[i1];
            justtouch(i0);
            ipLow=sliceModel.linelist[i0];
            ipHigh=sliceModel.linelist[i1];
        }
        
        if(true){ // get slice point and add it to the path
            if(ipHigh>=sliceModel.pointlist.size()){
                debuglinelist(i0);
                cout<<"EXC_BAD_ACESS"<<ipHigh<<":"<<sliceModel.pointlist.size()<<endl;
            }
            ofVec3f XYpoint = getXY(sliceModel.pointlist[ipHigh], sliceModel.pointlist[ipLow], sliceModel.dXdYlist[i0], sliceModel.dXdYlist[i1], divdH, z);
            if(oldpoint!=XYpoint){
                layerisland.lineTo(XYpoint.x,XYpoint.y);
            }
            oldpoint=XYpoint;
        }
        if(true){ //check if the last line
            if (isMeetPointStartA == true) {
                
                //                i--;
                if ((i0 == iStart0 && i1 == iStart1) ||(i1 == iStart0 && i0 == iStart1) ){
                    //
                    break;
                    
                }
            }
        }
        if(true){  // set pnext point
            if (ipUsed == ipNearA) {
                ipNext0 = ipNearB;
            } else { //ipUsed==ipNearB
                ipNext0 = ipNearA;
            }
        }
    }
    layerisland.close();
    return layerisland;
}
ofVec3f agmll::getScale(){
    ofMesh mesh=mergedMesh;
    ofVec3f a;// the min
    a.x=a.y=a.z=0;
    ofVec3f b;// the max
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
 add sliceModel.linelist and sliceModel.nearpointlist
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
        float zhorizon=sliceModel.pointlist[ipA].z;
        if(zhorizon==sliceModel.pointlist[ipB].z&&zhorizon==sliceModel.pointlist[ipC].z){//
            if(ofContains(sliceModel.horizonFacetHeightlist, zhorizon)==false){
                sliceModel.horizonFacetHeightlist.push_back(zhorizon);
                facetcount++;
            }
        }
        
        //add lines to the sliceModel.linelist
        for(int i=0;i<3;i++){
            int indexoldline=searchline(lines[i]);
            ofIndexType ipn=sidepoint[i];
            if(indexoldline<sliceModel.linelist.size()){
                //addoldline(indexoldline, ipn);
                if(sliceModel.nearpointlist[indexoldline]==sliceModel.nearpointlist[indexoldline+1]){
                    sliceModel.nearpointlist[indexoldline+1]=ipn;
                    //return;
                }else{
                    cout<<"waring!! add old line failed"<<endl;
                }
                
                counter1++;
            }else{
                agline newline=zsortline(lines[i]);
                linecopymap[newline]=sliceModel.linelist.size();// add only a line had zsort
                sliceModel.linelist.push_back(newline.ip0);
                sliceModel.linelist.push_back(newline.ip1);
                sliceModel.nearpointlist.push_back(ipn);
                sliceModel.nearpointlist.push_back(ipn);
                counter0++;
            }
        }
    }
    
    
    ofSort(sliceModel.horizonFacetHeightlist);// sort the map will help search
    //now we add the rest list
    size_t linelistlen=sliceModel.linelist.size();
    sliceModel.dXdYlist.assign(linelistlen, 0);
    sliceModel.linehorizonlist.assign(linelistlen, false);
    sliceModel.linetypelist.assign(linelistlen, false);
    sliceModel.linetypelist.assign(linelistlen, 0);
    
    //check out
    cout<<"horizong facet:"<<facetcount<<":"<<(counter0+counter1)/3<<", "<<(counter0+counter1)<<"?="<<mergedMesh.getNumIndices()<<endl;
    cout<<"add new"<<counter1<<":"<<counter0<<endl;
    if(counter1!=counter0){
        cout<<"!!!!add face get problem!!!!!"<<endl;
    }
    printsize();
    
    
}
void agmll::adddXdY(){
    float divdH;// divdH=1/H
    divdH=1/dH;
    for(ofIndexType i=0;i<sliceModel.linelist.size();i+=2){
        //   cout<<"adddxdy"<<i<<endl;
        ofVec3f p0,p1,pL,pH;
        p0=sliceModel.pointlist[sliceModel.linelist[i]];
        p1=sliceModel.pointlist[sliceModel.linelist[i+1]];
        // if horizonline
        if(p0.z==p1.z){
            addlinetype(i,horizonline,evenline);
            addtouchedlist(i, isUntouched, p0.z);
            sliceModel.linehorizonlist[i]=true;
            sliceModel.linehorizonlist[i+1]=true;
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
            
            sliceModel.dXdYlist[i]=0;
            sliceModel.dXdYlist[i+1]=0;
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
            sliceModel.dXdYlist[i]=0;
        }else{
            sliceModel.dXdYlist[i]=(pH.x-pL.x)*divH*dH;
        }
        if(pH.y==pL.y){
            sliceModel.dXdYlist[i+1]=0;
        }else{
            sliceModel.dXdYlist[i+1]=(pH.y-pL.y)*divH*dH;
        }
    }
}
void agmll::addlinetype(ofIndexType i,int linetype,int riseorfall){
    sliceModel.linetypelist[i]=linetype;
    sliceModel.linetypelist[i+1]=riseorfall;
    
}
void agmll::addtouchedlist(ofIndexType i,float isTouchedOrNot,float ZMax){
    sliceModel.linetypelist[i]=isTouchedOrNot;
    sliceModel.linetypelist[i+1]=ZMax;
}

void agmll::justtouch(ofIndexType i){
    sliceModel.linetypelist[i]=isTouched;
}


void agmll::addpointlist(){
    ofMesh mesh=mergedMesh;
    sliceModel.pointlist.swap(mesh.getVertices());
}

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
    if(ip0>sliceModel.pointlist.size()||ip1>sliceModel.pointlist.size()){
        cout<<"we get a bigggggg ip "<<ip0<<"\t"<<ip1<<"\t"<<sliceModel.pointlist.size()<<endl;
    }
    agline newline;
    if(sliceModel.pointlist[ip0].z<sliceModel.pointlist[ip1].z){
        newline.set(ip0,ip1);
    }else if(sliceModel.pointlist[ip0].z>sliceModel.pointlist[ip1].z){// ip1<ip0
        newline.set(ip1,ip0);
    }else{   // if they the same      // make sure ip0<ip1
        if(ip0<=ip1){
            newline.set(ip0,ip1);
        }else{
            newline.set(ip1,ip0);
        }
    }
    newline.markZSortTrue();// mark zsort that we can use it to search or add to linelist correctly
    return newline;
}

/**
 find the findline in linelist which need linecopymap help
 
 @param agline we will find
 @return i that from linelist[i]=findline // failed whene i =linelist.size()
 */
ofIndexType agmll::searchline( agline line){
    agline sortline;
    if(line.getzsort()==false){
        sortline=zsortline(line);
        
    }else{
        sortline =line;
    }
    auto it=linecopymap.find(zsortline(sortline));
    if(it==linecopymap.end()){
        //not found
        return sliceModel.linelist.size();
    }else{
        //found
        return it->second;
    }
}

ofIndexType agmll::findcrosspointat(ofIndexType startflag,float z){
    ofIndexType i0=0;//sliceModel.linelist[ip0]
    ofIndexType i1=0;//sliceModel.linelist[ip1]
    // find a cross point
    for(int i=startflag;i<sliceModel.linelist.size();i+=2){
        i0=i;
        i1=i+1;
        // we don't touch anything that already touched
        if(sliceModel.linetypelist[i0]==isTouched){
            // cout<<"it was touched"<<endl;
            continue;
        }else{
            // cout<<"it untouched "<<ip0<<endl;
        }
        
        justtouch(i0);
        // we don't touch when z>zmax
        if(z>sliceModel.linetypelist[i1]){
            // cout<<"it is ip0:"<<ip0<<">zmax"<<sliceModel.linetypelist[ip1]<<endl;
            continue;
        }
        if( isPointPlaneCross(sliceModel.pointlist[sliceModel.linelist[i0]], sliceModel.pointlist[sliceModel.linelist[i1]], z)==true){
            startflag=i0+2;
            return i0;
            break;
        }
    }
    startflag=i0+2;
    //  cout<<"we find nothing any more so we quit"<<endl;
    return sliceModel.linelist.size();
    
}


bool agmll::isPointPlaneCross(ofVec3f pointLower, ofVec3f  pointHigher, float planeatz){
    if(pointLower.z<planeatz&&pointHigher.z>planeatz){
        // cout<<"get the cross line: "<<ofToString(pointHigher)<<","<<ofToString(pointLower)<<endl;
        return true;
    }
    return false;
}

