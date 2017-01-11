#include "agmll.h"

agmll::agmll(){


}

void agmll::setup(ofMesh mesh){
    
    mergedMesh=mesh;
    
    //clean up
    pointlist.clear();
    linelist.clear();// {p0 p1} the index point to point list
    linehorizonlist.clear();// is this line horizon true horizon // add with addnewline work with adddXdY
    nearpointlist.clear();//{pa pb} the index point to point list
    dXdYlist.clear();// add with addnewline work with adddXdY
    
    //process -> 0
    ismeshMerged=0;//100 means done
    islinelistfilled=0;//100 means filled
    isdXdYlistfilled=0;//100 means filled
    isAllReady=0;//100 means all done

    // progress flag
     linelistloaded=0;
     dxdylistloaded=0;
}

void agmll::update(){
    //this update will put inside the app update in old day
    // new this will be excute in thread
    //calc work here
    // step 1: merge mesh points
    if(ismeshMerged<100){
        mergedMesh.mergeDuplicateVertices();
        addpointlist(mergedMesh);
        getScale(mergedMesh);
        ismeshMerged=100;
        return;
    }
    // step 2: add line list
    if(islinelistfilled<100){
        addface();
        islinelistfilled+=100;
        return;
    }
    
    // step 3: add dXdY
    if(isdXdYlistfilled<100){
        if(dxdylistloaded>=linelist.size()){
            isdXdYlistfilled=100;
            return;
        }
         adddXdY();
        ////cout<<"dxdylist:"<<dxdylistloaded<<"++++++++++++++++++"<<"\n";
        
        isdXdYlistfilled=100;
        return;
    }
    // step 4: to do list
    if(isAllReady<100){
    
    }
   
}
//step reset
void agmll::stepreset(){

    //stepreset
    ipstartL=0;
    ipstartH=0; // ipstart0<ipstart1 always
    ipstarta=0;// we never use ipa as next point until we find it
    ////cout<<"ipstart:"<<ipstartL<<":"<<ipstartH<<"\n";
    ipnext=0;
    ipused=0;
    
    ip0=0;//linelist[ip0]
    ip1=0;//linelist[ip1]
    iplp0=0;//pointlist[iplp0]
    iplp1=0;//pointlist[iplp1]
    
    ipH=0;
    ipL=0;
    ipa=0;//ipa=nearpointlist[ip0]
    ipb=0;//ipb=nearpointlist[ip1]
    // how to refer a point
    // point =pointlist[iplp0]
    // point =pointlist[linelist[ip0]]
    // point =pointlist[nearpointlist[ip0]]
    // point =pointlist[ipa]
    
    

}
void agmll::stepstart(ofIndexType i){
    ip0=i;
    ip1=ip0+1;
    ipa=nearpointlist[ip0];
    ipb=nearpointlist[ip1];
    ////cout<<"we find the cross point done"<<"\n";
    // get the first point XY
    // get the ph pl
    getipHipLfrom(ip0, ip1);

    //get the XY and move to
    ofVec3f XYpoint=getXY(pointlist[ipH], pointlist[ipL], dXdYlist[ip0], dXdYlist[ip1], dH, testatZ);
    
    //layertestpath.moveTo(XYpoint.x,XYpoint.y);
    addPointMoveToPath(XYpoint);
    // set pstart and pnext
    ipstarta=ipa;
    ipstartL=ipL;
    ipstartH=ipH;
    //cout<<"now we change the startpoint"<<endl;
    //cout<<"startA:"<<ipstarta<<endl;
    //cout<<"line list [a]:"<<linelist[ipstarta]<<endl;
    //cout<<"start at H L a:"<<ofToString(pointlist[linelist[ipstartH]])<<":"<<ofToString(pointlist[linelist[ipstartL]])<<":"<<ofToString(pointlist[linelist[ipstarta]])<<endl;
    ipnext=ipb;
}
void agmll::getipHipLfrom(ofIndexType indexpoint0, ofIndexType indexpoint1){
    if (linetypelist[indexpoint0+1]==riseline) {
        ipL=linelist[indexpoint0];
        ipH=linelist[indexpoint1];
    }else{
        ipL=linelist[indexpoint1];
        ipH=linelist[indexpoint0];
    }
}
/**
 find the next line

 @param nextlineip0 the ip0 of line we find
 @param nextlineip1 the ip1 of line we find
 @return return the ip0 of the line we found
 */
ofIndexType agmll::findnextline(ofIndexType lineip0, ofIndexType lineip1){
    iplp0=lineip0;
    iplp1=lineip1;
    if(iplp0<iplp1){
        //do nothing
    }else{// ip0>ip1 need swap
        ofIndexType iplp2=iplp1;
        iplp1=iplp0;
        iplp0=iplp2;
    }
    
    //new theory
         ip0=searchline(iplp0, iplp1);
    ip1=ip0+1;
    ipa=nearpointlist[ip0];
    ipb=nearpointlist[ip1];
    justtouch(ip0);
    debuglinelist(ip0);
 
  
    return ip0;

}
void agmll::checkpnextZ(){
    if(pointlist[ipnext].z<=testatZ){
        ipused=ipL;
        iplp0=ipH;
        iplp1=ipnext;
        if(pointlist[ipnext].z==testatZ){
            testatZoffset=0.000001;
        }
    }else{
        ipused=ipH;
        iplp0=ipL;
        iplp1=ipnext;
    }

}
// public
ofPath agmll::layertestat(float z){
   // cout<<"------layertest start------"<<endl;
    alluntouched();
    continueflag=0;
    layertestpath.clear();
    z+=meshMin.z;
    testatZ=z;//+meshMin.z;
   // cout<<"z="<<testatZ<<" minZ "<<meshMin.z<<endl;
    ofPath returnpath;
    if(isdXdYlistfilled!=100){
        return returnpath;
    }
    ofIndexType continuepoint;
    for( continuepoint=0;continuepoint<linelist.size();continuepoint+=2){

        //continuepoint=linelist.size();
    stepreset();
    //step start
    // find a cross point
        if(continueflag==linelist.size()){
            break;
        }
        ofIndexType iCross=findcrosspointat(z);
        if(iCross==linelist.size()){// iCross== linelist.size means we failed to find the cross point
        //    cout<<"find nothing yet"<<endl;
            break;// that we failed to a slice layer because there is nothing on this layer
        }
        // cout<<"find something yet"<<endl;
        layertestpath.clear();
        
        ip0=iCross;
    //cout<<"find a start"<<ip0<<"at"<<continuepoint<<" flag is:"<<continueflag<<endl;
        //debuglinelist(ip0);
    stepstart(ip0);
    
  
    //step loop
//    cout<<"we just going to loop"<<"\n";
    
    int loopcount=0;
    bool isloopover=false;
    int finalsteptogo=1;
    for(int i=0;i<linelist.size()+1;i+=2){
        if(isloopover!=true&&ipnext==ipstarta){
            isloopover=true;
           //cout<<"now loop will over soon"<<"\n";
            //cout<<ipnext<<" to "<<linelist[ipstarta]<<endl;
            
        }else{
           // cout<<ipnext<<" to "<<linelist[ipstarta]<<endl;
        }
        
        loopcount++;
        //cout<<"loop:"<<loopcount<<endl;
        // check z with pnext
        checkpnextZ();
        //find the lnext that have linelist[i]=iplp0 linelist[i+1]=iplp1
       // so we find next line
        findnextline(iplp0,iplp1);

        //then we get the new ipH ipL from new ip0 ip1
        getipHipLfrom(ip0, ip1);
        
        //check if it the last line?
        if(isloopover==true){
            if((ipL==ipstartL&&ipH==ipstartH)||(ipL==ipstartH&&ipH==ipstartL)){
                finalsteptogo=0;
               // cout<<"we find the last line "<<"\n";
            }else{
                //cout<<"we are near the last line surely"<<"\n";
                //cout<<"ipL ipH:"<<ipL<<" "<<ipH<<"start"<<ipstartL<<" "<<ipstartH<<"\n";
            }
        }
        ////cout<<"so we find the pl ph"<<ipL<<","<<ipH<<"\n";
        ofVec3f XYpoint=getXY(pointlist[ipH], pointlist[ipL], dXdYlist[ip0], dXdYlist[ip1], dH, z);
        //layertestpath.lineTo(XYpoint.x,XYpoint.y);
        addPointLineToPath(XYpoint);
        //cout<<"we now line to x y "<<XYpoint.x<<","<<XYpoint.y<<" to path"<<endl;
        
        if(isloopover==true){
            if(finalsteptogo==0){
              // cout<<"now we go out"<<"\n";
                break;
            }
        }
        // set pnext
        if(ipused==ipa){
            ipnext=ipb;
        }else{//ipused==ipb
            ipnext=ipa;
        }
        
    }
    layertestpath.close();
        returnpath.append(layertestpath);
     //  cout<<"now we close the sub path"<<endl;
    }

   // returnpath=layertestpath;
    // cout<<"------layertest end------"<<endl;
    returnpath.close();
    cout<<"continepoint="<<continuepoint<<endl;
    returnpath.setPolyWindingMode(OF_POLY_WINDING_ODD);
    returnpath.setStrokeColor(ofColor::blue);
    returnpath.setFillColor(ofColor::white);
    returnpath.setFilled(true);
   // cout<<"windingmode"<<returnpath.getWindingMode()<<endl;
    returnpath.setStrokeWidth(1);
    return returnpath;
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
    ofIndexType ia;
    ofIndexType ib;
    ofIndexType ic;
    ofVec3f pa;
    ofVec3f pb;
    ofVec3f pc;
    ofIndexType i=0;
    //cout<<"ia,ib,ic"<<ia<<ib<<ic<<"\n";
    if(i==0){// first face
        // get the point first
         ia=mergedMesh.getIndex(i);
         ib=mergedMesh.getIndex(i+1);
         ic=mergedMesh.getIndex(i+2);
        // pa=mergedMesh.getVertex(ia);
        // pb=mergedMesh.getVertex(ib);
        // pc=mergedMesh.getVertex(ic);
        //ab
        addnewline(ia,ib, ic);
        //ac
        addnewline(ia,ic,ib);
        //bc
        addnewline(ib,ic,ia);
        //cout<<"first 3 line"<<"\n";
    }
    counter0=0;
    counter1=0;
   
    for(i=3;i<mergedMesh.getNumIndices();i+=3){//i>0
        // get the point first
         ia=mergedMesh.getIndex(i);
         ib=mergedMesh.getIndex(i+1);
         ic=mergedMesh.getIndex(i+2);
         //pa=mergedMesh.getVertex(ia);
         //pb=mergedMesh.getVertex(ib);
         //pc=mergedMesh.getVertex(ic);
        
        // the first line ab
        int oldline0=searchline(ia,ib);
        // line exist and not
        // ab
        if(oldline0<linelist.size()){
            addoldline(oldline0, ic);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ia, ib, ic);
        }
        
        // the second line ac
        int oldline1=searchline(ia,ic);
        
        //ac
        if(oldline1<linelist.size()){
            addoldline(oldline1, ib);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ia, ic, ib);
        }
        
        //the third line bc
        int oldline2=searchline(ic,ib);
        //bc
        if(oldline2<linelist.size()){
            addoldline(oldline2, ia);
            ////cout<<"add old line"<<"\n";
        }else{
            addnewline(ic, ib, ia);
        }
        // new line
    }
    }
void agmll::adddXdY(){
    if(islinelistfilled<100){// can't work when linelist is not filled
        return;
    }
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
void agmll::justtouch(ofIndexType ip){
    touchedlist[ip]=isTouched;
}
// private:
//do in setup
void agmll::addpointlist(ofMesh mesh){
    for(ofIndexType i=0;i<mesh.getNumVertices();i++){
        pointlist.push_back(mesh.getVertex(i));
    }
}
//do once


//do loop

// tools
ofVec3f agmll::getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z){
    ofVec3f returnpoint;
    //z=z;
    
    float divdH=1/dH;
    float h=z-pL.z-testatZoffset;
    returnpoint.x=pL.x+dX*divdH*h;
    returnpoint.y=pL.y+dY*divdH*h;
    returnpoint.z=z;
    //cout<<"getXY():"<<ofToString(returnpoint)<<" when pH:"<<ofToString(pH)<<" pL:"<<ofToString(pL)<<endl;
    testatZoffset=0;
    return returnpoint;
}

void agmll::addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn){
    ////cout<<"ip0,ip1:"<<ip0<<","<<ip1<<"\n";
    ////cout<<"before"<<linelist.size();
    
    if(ip0<ip1){
       agline newline(ip0,ip1);
        linecopymap[newline]=linelist.size();
        linelist.push_back(ip0);
        linelist.push_back(ip1);
        //cout<<"0<1"<<"\n";
    }else{// ip1<ip0
        agline newline(ip1,ip0);
        linecopymap[newline]=linelist.size();
        linelist.push_back(ip1);
        linelist.push_back(ip0);
        //linecopymap[newline]=ip1;
        //cout<<"1<0"<<"\n";
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
    
    if(ip0<ip1){
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
    }else{// ip1<ip0
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
ofPath agmll::addPointToPath(ofPath path,float x,float y,ofIndexType i){
    ofPath returnpath;
    if(i==0){
        returnpath.moveTo(x,y);
        cout<<"we move to x y :"<<x<<","<<y<<"to path"<<"\n";
    }else{
        returnpath=path;
        returnpath.lineTo(x,y);
        cout<<"we line to  x y :"<<x<<","<<y<<"to path"<<"\n";
    }
    
    return returnpath;
}
void agmll::addPointMoveToPath(ofVec3f addpoint){
    layertestpath.moveTo(addpoint.x,addpoint.y);
 //   cout<<"||:"<<ofToString(addpoint)<<endl;
}

void agmll::addPointLineToPath(ofVec3f addpoint){

    layertestpath.lineTo(addpoint.x,addpoint.y);
 //cout<<"|||"<<ofToString(addpoint)<<endl;
}

/**
 find the first line that we begin when slice at z
TODO: we need a quickstartlist that hold every z start line
 @param z slice at z
 @return return the i for linelist[i]
 */
ofIndexType agmll::findcrosspointat(float z){
    ofIndexType ip0=0;//linelist[ip0]
    ofIndexType ip1=0;//linelist[ip1]
    // find a cross point
    for(int i=continueflag;i<linelist.size();i+=2){
        ip0=i;
        ip1=i+1;
        // we don't touch anything that already touched
        if(touchedlist[ip0]==isTouched){
           // cout<<"it was touched"<<endl;
            continue;
        }else{
        // cout<<"it untouched "<<ip0<<endl;
        }
        
        justtouch(ip0);
        // we don't touch when z>zmax 
        if(z>touchedlist[ip1]){
           // cout<<"it is ip0:"<<ip0<<">zmax"<<touchedlist[ip1]<<endl;
            continue;
        }
        if( isPointPlaneCross(ip0, ip1, linetypelist[ip1], testatZ)==true){
           // cout<<"we roll"<<endl;
            continueflag=ip0+2;
            return ip0;
            break;
        }
        
        
    }
    continueflag=ip0+2;
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
        //cout<<"testatZ:"<<planeatz<<endl;
            return true;
        }
    
    
    
    return false;
    
}

void agmll::debuglinelist(ofIndexType ip){
    return;
    cout<<"ip0,ip1:"<<ip<<","<<ip+1<<"   ";
    if(ip+1>linelist.size()){
        cout<<"waring:ip>linelist.size"<<endl;
        return;
    }else{
        cout<<"linelist.size:"<<linelist.size()<<"   ";
    }
    ofIndexType index0=linelist[ip];
    ofIndexType index1=linelist[ip+1];
    cout<<"point index is:"<<index0<<":"<<index1<<"   ";
    if(index0>pointlist.size()||index1>pointlist.size()){
        cout<<"waring:index>pointlist.size"<<endl;
        return;
    }else{
        cout<<"pointlist.size:"<<pointlist.size()<<"   ";
    }
    ofPoint point0,point1;
    point0=pointlist[index0];
    point1=pointlist[index1];
    cout<<"line from:"<<ofToString(point0)<<" to "<<ofToString(point1)<<"   ";
    if(point0.x==point1.x&&point0.y==point1.y&&point0.z==point1.z){
        cout<<"this is a singlepoint;"<<endl;
    }else{
     cout<<"this is a line;"<<endl;
    }
    //cout<<"nearpointtype:"<<nearpointlist[ip]<<" dir:"<<nearpointlist[ip+1]<<endl;
    //cout<<"linetype:"<<linetypelist[ip]<<" dir:"<<linetypelist[ip+1]<<endl;
    //cout<<"zmax:"<<touchedlist[ip+1]<<" when slice at Z:"<<testatZ<<endl;

}
