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
    sliceModel.refresh();
    returnpath.setStrokeColor(ofColor::white);
    returnpath.setFilled(false);
    returnpath.setStrokeWidth(10);
    if(1){
        returnpath.setPolyWindingMode(OF_POLY_WINDING_ODD);
        returnpath.setStrokeColor(ofColor::gray);
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
   
    
    vector<ofIndexType> pathindexs;
    vector<ofVec3f> pathpoints;
    pathindexs.push_back(i0);
    // set the first point
    ofIndexType i0=iBegin;//multilinklinelist[i0]
    //get the XY and move to
    ofVec3f XYpoint=sliceModel.getXY(sliceModel.multilinklinelist[i0], divdH, z,640,384);
    ofVec3f oldpoint=XYpoint;
    ofVec3f middlepoint;
    pathpoints.push_back(XYpoint);
    //        cout<<"XY:"<<ofToString(XYpoint)<<endl;
    sliceModel.multilinklinelist[i0].touch();
    
    //step loop
    int loopcount = 0;
    bool isMeetStart0 = false;
    agline nextline;
    for ( unsigned long i = 0; i < sliceModel.multilinklinelist.size() ; i++) {
        //checkpnextZ, figure out use which side as next line
        if(sliceModel.checkLineError(sliceModel.multilinklinelist[i0])){
            break;
        }
        if(i>0){
            if(sliceModel.multilinklinelist[i0].touchUsedLink(nextline.getLastpoint())==false){
                break;
            }
        }
        sliceModel.multilinklinelist[i0].touch();
        pathindexs.push_back(i0);
        ofVec3f XYpoint = sliceModel.getXY(sliceModel.multilinklinelist[i0],divdH,z,640,384);
        if(oldpoint!=XYpoint){
            pathpoints.push_back(XYpoint);
        }
        oldpoint=XYpoint;
        // make next line
        // a complicated work to find out use which line
        if(sliceModel.multilinklinelist[i0].isFilled()==false){// check first
            cout<<"now we meet unfilled line so quick break"<<endl;
            break;
        }
        nextline=sliceModel.multilinklinelist[i0].getNextLine(z);;
        if(sliceModel.checkLineError(nextline)){
            cout<<"now we meet line is over size !!!!!!!!"<<endl;
            break;
        }else{
            agline sortnextline=sliceModel.zsortline(nextline);
            i0=sliceModel.searchLine(sortnextline);
        }
        if (i0 == iBegin){//check if the last line
            
            //cout<<"we end the loop path"<<endl;
            break;
        }
        
    }
    //     cout<<"path point count:"<<pathpoints.size()<<endl;
    ofPath layerisland;
    //now make the layer island
    layerisland.moveTo(pathpoints[0].x,pathpoints[0].y);
    for(int p=1;p<pathpoints.size();p++){
        layerisland.lineTo(pathpoints[p].x,pathpoints[p].y);
        //         cout<<"path "<<p<<":"<< pathindexs[p]<<":"<<pathpoints[p]<<endl;
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
ofMesh agmll::getSliceShell(){
    
    
    
}
void agmll::addSupport(){
    vector<ofMeshFace> facets;
    facets=mergedMesh.getUniqueFaces();
    size_t facesize= facets.size();
    for(int i=0;i<facesize;i++){
        agfacet facet;
        facet.setFromTri(facets[i].getVertex(0),facets[i].getVertex(1),facets[i].getVertex(2));
        //        facet.setNormal(facets[i].getNormal(0), facets[i].getNormal(1), facets[i].getNormal(2));
        facet.setFaceNormal(facets[i].getFaceNormal());
        if(facet.getGradiant()>0){
            //        cout<<i<<" get gradiant:"<<facet.getGradiant()<<endl;
            supportPolygon.append(facet.getPath());
        }
    }
    supportPolygon.setPolyWindingMode(OF_POLY_WINDING_ODD);
    supportPolygon.setStrokeColor(ofColor::white);
    supportPolygon.setFillColor(ofColor::blue);
    supportPolygon.setFilled(true);
    supportPolygon.setStrokeWidth(1);
}
/**
 add sliceModel.linelist and sliceModel.nearpointlist
 */
void agmll::addFacet(){
    addpointlist();//
    // get the point first
    ofIndexType ipA;
    ofIndexType ipB;
    ofIndexType ipC;
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
            
            if(sliceModel.checkLineError(lines[i])){
                cout<<"[WARNING]add facet big wrong!!!!!!!!big index>pointlist.size"<<endl;
                continue;
            }
            agline newline=sliceModel.zsortline(lines[i]);// sort the line
            //[new]
            if( sliceModel.addLine(newline,sidepoint[i])){//add line with dxdy
                
            }else{
                
                cout<<"add facet line failed"<<endl;
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
    if(sliceModel.printAllWrongLine()==false){
        sliceModel.tryFix();
        cout<<"now we get fixed"<<endl;
        sliceModel.printAllWrongLine();
    }else{
        cout<<"now we never fixed "<<endl;
        
    }
}
void agmll::addpointlist(){
    ofMesh mesh=mergedMesh;
    sliceModel.pointlist.swap(mesh.getVertices());
}







