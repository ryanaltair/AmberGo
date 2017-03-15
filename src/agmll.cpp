#include "agmll.h"

agmll::agmll(){
    divdH=1/dH;
    
}
agmll::~agmll(){
    mergedMesh.clear();
    
}
void agmll::load(ofMesh mesh){
    mergedMesh.clear();
    mergedMesh=mesh;
    indexsize=mesh.getNumIndices();
    supportPolygon.clear();
}
/**
 use swap to clean the vector
 */
void agmll::cleanmermory(){
    // let us do the clean
    sliceModel.cleanmermory();
    //
}
void agmll::prepareModel(){
    cleanmermory();
    //calc work here
    mergedMesh.mergeDuplicateVertices();// step 1: merge mesh points
    cout<<"Normals count is "<<mergedMesh.getNumNormals()<<endl;
    addSupport();
    addpointlist();//
    getScale();
    addFacet();  // step 2: add line list
}


// public
ofPath  agmll::layerAt(float z){
    z += sliceModel.scaleMin.z;
    z=sliceModel.getNoHorizonHappenZ(z);
    ofPath returnpath;
    sliceModel.alluntouched();
    ofIndexType continueflag = 0;
    for (ofIndexType loopcount = 0; loopcount < sliceModel.multilinklinelist.size(); loopcount ++) {// every loop build a closed path
        ofIndexType iCross = sliceModel.findcrosspointat(continueflag,z);//find a cross point
        continueflag=iCross+1;
        if (iCross < sliceModel.multilinklinelist.size()) { // iCross== sliceModel.linelist.size means we failed to find the cross
            returnpath.append(layerCloseLoop(z,iCross));// add the single closed loop to the returnpath
        }else{
            break;// that we failed to a slice layer because there is nothing on this layer
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
 @return whole slice layer as ofPath
 */
ofPath  agmll::layerCloseLoop(float z,ofIndexType iBegin){
    ofPath layerisland;
    ofIndexType i0;//multilinklinelist[i0]
    ofIndexType iStart0;//multilinklinelist[iStart0]
    ofIndexType ipStartA;//pointlist[ipStartA]
    ofIndexType ipHigh,ipLow;
    ofIndexType ipNearA,ipNearB;//pointlist[ipNearA]
    ofIndexType ipUsed,ipNext0,ipNext1;
    ofVec3f oldpoint;
    int pointcount=1;
    bool isNextLineuseNearA=false;
    // ready to go
    if(1){// get the first point
        i0=iBegin;
        sliceModel.multilinklinelist[i0].touch();
        ipNearA=sliceModel.multilinklinelist[i0].ipa;
        ipNearB=sliceModel.multilinklinelist[i0].ipb;
        ////cout<<"we find the cross point done"<<"\n";
        // get the first point XY
        // get the ph pl
        ipLow=sliceModel.multilinklinelist[i0].ip0;
        ipHigh=sliceModel.multilinklinelist[i0].ip1;
        //get the XY and move to
        ofVec3f XYpoint=sliceModel.getXY(sliceModel.multilinklinelist[i0], divdH, z);
        oldpoint=XYpoint;
        layerisland.moveTo(XYpoint.x,XYpoint.y);
        // set pstart and pnext
        iStart0=i0;// when we meet iStart0 again,we end
        ipStartA=ipNearA;
        ipNext0=ipNearB;
    }
    //step loop
    int loopcount = 0;
    bool isMeetStart0 = false;
    unsigned long i ;
    for ( i = 0; i < sliceModel.multilinklinelist.size() + 1; i++) {
        //checkpnextZ, figure out use which side as next line
        float nextZ=z;
        if(ipNext0>sliceModel.pointlist.size()){
            cout<<"wrong!!!!!!!!"<<endl;
        }
        if(1){// get the next line with ipNext0 and ipNext1
            // a complicated work to find out use which line
            bool nextLineUseUpperSide=sliceModel.multilinklinelist[i0].nextLineUsingUpperPoint(isNextLineuseNearA,z);
            // get the next line index
            if(nextLineUseUpperSide==true){
                ipUsed=ipLow;
                ipNext1=ipHigh;
            }else{
                ipUsed=ipHigh;
                ipNext1=ipLow;
            }
            // make sure ipNext1 is z higher than ipNext0
            agline nextline;
            nextline.set(ipNext0,ipNext1);
            agline sortnewline=zsortline(nextline);
            i0=sliceModel.searchLine(sortnewline);
            sliceModel.multilinklinelist[i0].touch();
            ipNearA=sliceModel.multilinklinelist[i0].ipa;
            ipNearB=sliceModel.multilinklinelist[i0].ipb;
            ipLow=sliceModel.multilinklinelist[i0].ip0;
            ipHigh=sliceModel.multilinklinelist[i0].ip1;
        }
        
        if(true){ // get slice point and add it to the path
            if(ipHigh>=sliceModel.pointlist.size()){
                //                debuglinelist(i0);
                cout<<"BAD_ACESS"<<ipHigh<<":"<<sliceModel.pointlist.size()<<endl;
            }
            ofVec3f XYpoint = sliceModel.getXY(sliceModel.multilinklinelist[i0],  divdH, z);
            if(oldpoint!=XYpoint){
                layerisland.lineTo(XYpoint.x,XYpoint.y);
                pointcount++;
            }
            oldpoint=XYpoint;
        }
        if(true){  // set pnext point
            if (ipUsed == ipNearA) {
                isNextLineuseNearA=false;
                ipNext0 = ipNearB;
            } else { //ipUsed==ipNearB
                isNextLineuseNearA=true;
                ipNext0 = ipNearA;
            }
        }
        if (i0 == iStart0){//check if the last line
            //cout<<"we end the loop path"<<endl;
            break;
        }
        
    }
    layerisland.close();
    //    cout<<"iBegin:"<<iBegin<<"path get point :"<< pointcount<<endl;
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
ofMesh agmll::getSliceShell(){



}
void agmll::addSupport(){
    vector<ofMeshFace> facets;
    facets=mergedMesh.getUniqueFaces();
    size_t facesize= facets.size();
    for(int i=0;i<facesize;i++){
        agfacet facet;
        facet.setFromTri(facets[i].getVertex(0),facets[i].getVertex(1),facets[i].getVertex(2));
        facet.setNormal(facets[i].getNormal(0), facets[i].getNormal(1), facets[i].getNormal(2));
        facet.setFaceNormal(facets[i].getFaceNormal());
        if(facet.getGradiant()>0){
            cout<<i<<" get gradiant:"<<facet.getGradiant()<<endl;
        supportPolygon.append(facet.getPath());
        }
    }
    supportPolygon.setPolyWindingMode(OF_POLY_WINDING_ODD);
    supportPolygon.setStrokeColor(ofColor::white);
    supportPolygon.setFillColor(ofColor::blue);
    supportPolygon.setFilled(true);
    supportPolygon.setStrokeWidth(2);
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
        //add index to lines
        //        lines[0].set(ipA,ipB,ipC);
        lines[0].set(ipA,ipB);
        sidepoint[0]=ipC;
        
        //        lines[1].set(ipA,ipC,ipB);
        lines[1].set(ipA,ipC);
        sidepoint[1]=ipB;
        
        //        lines[2].set(ipB,ipC,ipA);
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
        
        ofSort(sliceModel.horizonFacetHeightlist);// sort the map will help search
        
        //add lines to the sliceModel.linelist
        for(int i=0;i<3;i++){
            agline newline=zsortline(lines[i]);
            //[new]
            
            
            if( sliceModel.addLine(newline,sidepoint[i])){//add line with dxdy
                //
            }
            //[new end]
            
        }
    }
    
    
    
    //check out
    cout<<"horizong facet:"<<facetcount<<":"<<(counter0+counter1)/3<<", "<<(counter0+counter1)<<"?="<<mergedMesh.getNumIndices()<<endl;
    cout<<"add new"<<counter1<<":"<<counter0<<endl;
    if(counter1!=counter0){
        cout<<"!!!!add face get problem!!!!!"<<endl;
    }
    printsize();
}
void agmll::addpointlist(){
    ofMesh mesh=mergedMesh;
    sliceModel.pointlist.swap(mesh.getVertices());
}


agline agmll::zsortline(agline line){
    ofIndexType ip0,ip1,ipa,ipb;
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




