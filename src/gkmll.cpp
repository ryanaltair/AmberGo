#include "gkmll.h"

gkmll::gkmll(){


}

void gkmll::setup(ofMesh mesh){
    mergedMesh=mesh;
    
}

void gkmll::update(ofMesh mesh){
    //this update will put inside the app update
    //calc work here
    // step 1: merge mesh points
    if(ismeshMerged<100){
    
        
        addpointlist(mesh);
        getScale(mesh);
        ismeshMerged=100;
        return;
    }
    // step 2: add line list
    if(islinelistfilled<100){
        int loadstep=10;
        if(linelistloaded>=mesh.getNumIndices()){
            islinelistfilled=100;
            for(int i=0;i<linelist.size();i++){
            cout<<i<<":"<<linelist[i]<<"\n";
            }
            return;
        }
        for(size_t i=linelistloaded;i<mesh.getNumIndices()/3&&i<linelistloaded+loadstep;i+=1){
            addface(mesh, i);
        }
        cout<<"linelist:"<<linelist.size()<<"++++++++++++++++++"<<"\n";
        linelistloaded+=loadstep;
        return;
    }
    
    // step 3: add dXdY
    if(isdXdYlistfilled<100){
        int dxdyloadstep=1;
        if(dxdylistloaded>=linelist.size()){
            isdXdYlistfilled=100;
            return;
        }
        for(size_t i=dxdylistloaded;i<linelist.size()/2&&i<dxdylistloaded+dxdyloadstep;i+=1){
            adddXdY(i);
            
        }
        cout<<"dxdylist:"<<dxdylistloaded<<"++++++++++++++++++"<<"\n";
        dxdylistloaded+=dxdyloadstep;
        return;
    }
    // step 4: to do list
    if(isAllReady<100){
    
    }
   
}
// public
ofPath gkmll::layertestat0(float z){
    ofPath returnpath;
    ofIndexType ipstartL=0;
    ofIndexType ipstartH=0; // ipstart0<ipstart1 always
    ofIndexType ipstarta=0;// we never use ipa as next point until we find it
    cout<<"ipstart:"<<ipstartL<<":"<<ipstartH<<"\n";
    ofIndexType ipnext=0;
    ofIndexType ipused=0;
    
    if(isdXdYlistfilled!=100){
        return ;
    }
    
    ofIndexType ip0=0;//linelist[ip0]
    ofIndexType ip1=0;//linelist[ip1]
    ofIndexType iplp0=0;//pointlist[iplp0]
    ofIndexType iplp1=0;//pointlist[iplp1]
    
    ofIndexType ipH=0;
    ofIndexType ipL=0;
    ofIndexType ipa=0;//ipa=nearpointlist[ip0]
    ofIndexType ipb=0;//ipb=nearpointlist[ip1]
    //dX=dXdYlist[ip0]
    //dY=dXdYlist[ip1]
    // how to refer a point
    // point =pointlist[iplp0]
    // point =pointlist[linelist[ip0]]
    // point =pointlist[nearpointlist[ip0]]
    // point =pointlist[ipa]
    
    //start
    // find a cross point
    for(int i=0;i<linelist.size();i+=2){
        ip0=i;
        ip1=i+1;
        
        if(pointlist[linelist[ip0]].z<pointlist[linelist[ip1]].z){
            
            if(pointlist[linelist[ip0]].z<z&&pointlist[linelist[ip1]].z>z){
                
                ipa=nearpointlist[ip0];
                ipb=nearpointlist[ip1];
                break;
            }
        }else{// pointlist[linelist[ip1]].z<pointlist[linelist[ip0]].z
            if(pointlist[linelist[ip1]].z<z&&pointlist[linelist[ip0]].z>z){
                
                ipa=nearpointlist[ip0];
                ipb=nearpointlist[ip1];
                break;
            }
        }
        
    }
    cout<<"we find the cross point done"<<"\n";
    // get the first point XY
    // get the ph pl
    if(pointlist[linelist[ip0]].z<pointlist[linelist[ip1]].z){
        ipL=linelist[ip0];
        ipH=linelist[ip1];
    }else{//pointlist[linelist[ip0]].z>pointlist[linelist[ip1]].z
        ipL=linelist[ip1];
        ipH=linelist[ip0];
    }
    
    cout<<"we find the pl ph at start done"<<"\n";

    //get the XY and move to
    ofVec3f XYpoint=getXY(pointlist[ipH], pointlist[ipL], dXdYlist[ip0], dXdYlist[ip1], dH, z);
    returnpath=addPointToPath(returnpath, XYpoint.x, XYpoint.y, 0);
    // set pstart and pnext
    ipstarta=ipa;
    
    ipstartL=ipL;
    ipstartH=ipH;
    cout<<"=======================";
    cout<<"ipL ipH:"<<ipL<<" "<<ipH<<"start"<<ipstartL<<" "<<ipstartH<<"\n";
    
    ipnext=ipb;
    // check z with pnext
    if(pointlist[ipnext].z<=z){
        ipused=ipL;
        iplp0=ipH;
        iplp1=ipnext;
    }else{
        ipused=ipH;
        iplp0=ipL;
        iplp1=ipnext;
    }
    
    //loop
    cout<<"we just going to loop"<<"\n";
    
    int loopcount=0;
    bool isloopover=false;
    int finalsteptogo=1;
    for(int i=0;i<linelist.size();i+=2){
        if(isloopover!=true&&ipnext==linelist[ipstarta]){
            isloopover=true;
            cout<<"now loop will over soon"<<"\n";
        }
        
        loopcount++;
        cout<<"we loop "<<loopcount<<"\n";
        

        //find the lnext that have linelist[i]=iplp0 linelist[i+1]=iplp1
        if(iplp0<iplp1){
            //do nothing
        }else{// ip0>ip1 need swap
            ofIndexType iplp2=iplp1;
            iplp1=iplp0;
            iplp0=iplp2;
        }
        //cout<<"the iplp0 iplp1 is "<<iplp0<<","<<iplp1<<"\n";
        //find the iplp0 iplp1
        for(i=0;i<linelist.size();i+=2){
            if(linelist[i]==iplp0&&linelist[i+1]==iplp1){
                ip0=i;
                ip1=i+1;
                ipa=nearpointlist[ip0];
                ipb=nearpointlist[ip1];
                break;
            }
        }
        //cout<<"and we find ip0 ip1 ipa ipb is "<<ip0<<","<<ip1<<","<<ipa<<","<<ipb<<"\n";
        
        //get pH pL
        if(pointlist[linelist[ip0]].z<pointlist[linelist[ip1]].z){
            ipL=linelist[ip0];
            ipH=linelist[ip1];
        }else{//pointlist[linelist[ip0]].z>pointlist[linelist[ip1]].z
            ipL=linelist[ip1];
            ipH=linelist[ip0];
        }
        //check if it the last line?
        if(isloopover==true){
            if((ipL==ipstartL&&ipH==ipstartH)||(ipL==ipstartH&&ipH==ipstartL)){
                finalsteptogo=0;
                cout<<"we find the last line "<<"\n";
            }else{
                cout<<"we are near the last line surely"<<"\n";
                cout<<"ipL ipH:"<<ipL<<" "<<ipH<<"start"<<ipstartL<<" "<<ipstartH<<"\n";
            }
        }
        //cout<<"so we find the pl ph"<<ipL<<","<<ipH<<"\n";
        ofVec3f XYpoint=getXY(pointlist[ipH], pointlist[ipL], dXdYlist[ip0], dXdYlist[ip1], dH, z);
        returnpath=addPointToPath(returnpath, XYpoint.x, XYpoint.y, loopcount);
        if(isloopover==true){
            if(finalsteptogo==0){
               cout<<"now we go out"<<"\n";
                break;
            }
        }
        // set pnext
        if(ipused==ipa){
            ipnext=ipb;
        }else{//ipused==ipb
            ipnext=ipa;
        }
        // check z with pnext
        if(pointlist[ipnext].z<z){
            ipused=ipL;
            iplp0=ipH;
            iplp1=ipnext;
        }else{//pointlist[ipnext].z>z
            ipused=ipH;
            iplp0=ipL;
            iplp1=ipnext;
        }
    }
    returnpath.close();
    returnpath.setStrokeColor(ofColor::blue);
    returnpath.setFillColor(ofColor::white);
    returnpath.setFilled(true);
    returnpath.setStrokeWidth(1);
    return returnpath;
    
    

}
 
ofVec3f gkmll::getScale(ofMesh mesh){
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

void gkmll::addface(ofMesh mesh,ofIndexType i){
    i=i*3;
    // get the point first
    ofIndexType ia=mesh.getIndex(i);
    ofIndexType ib=mesh.getIndex(i+1);
    ofIndexType ic=mesh.getIndex(i+2);
    ofVec3f pa=mesh.getVertex(ia);
    ofVec3f pb=mesh.getVertex(ib);
    ofVec3f pc=mesh.getVertex(ic);
    cout<<"ia,ib,ic"<<ia<<ib<<ic<<"\n";
    if(i==0){// first face
        //ab
        addnewline(ia,ib, ic);
        //ac
        addnewline(ia,ic,ib);
        //bc
        addnewline(ib,ic,ia);
        cout<<"first 3 line"<<"\n";
    }else{//i>0
        
        int oldline0=searchline(ia,ib);
        
        // line exist and not
        // ab
        if(oldline0>=0){
            addoldline(oldline0, ic);
            
            cout<<"add old line"<<"\n";
        }else{
            addnewline(ia, ib, ic);
        }
        int oldline1=searchline(ia,ic);
        
        //ac
        if(oldline1>=0){
            addoldline(oldline1, ib);
            cout<<"add old line"<<"\n";
        }else{
            addnewline(ia, ic, ib);
        }
        int oldline2=searchline(ic,ib);
        //bc
        if(oldline2>=0){
            addoldline(oldline2, ia);
            cout<<"add old line"<<"\n";
        }else{
            addnewline(ic, ib, ia);
        }
        // new line
    }

}
void gkmll::adddXdY(ofIndexType i){
    if(islinelistfilled<100){// can't work when linelist is not filled
        return;
    }
    ofVec3f p0,p1,pL,pH;
    i=i*2;
    p0=pointlist[linelist[i]];
    p1=pointlist[linelist[i+1]];
    if(p0.z==p1.z){
        linehorizonlist[i]=true;
        linehorizonlist[i+1]=true;
        return;
    }
    //if line vertical
    if(p0.x==p1.x&&p0.y==p1.y){
        dXdYlist[i]=0;
        dXdYlist[i+1]=0;
    }
    //get the high point and low point
    if(p0.z>p1.z){
        pL=p1;
        pH=p0;
    }else{// p0.z<p1.z
        pL=p0;
        pH=p1;
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
// private:
//do in setup
void gkmll::addpointlist(ofMesh mesh){
    for(ofIndexType i=0;i<mesh.getNumVertices();i++){
        pointlist.push_back(mesh.getVertex(i));
    }
}
//do once


//do loop

// tools
ofVec3f gkmll::getXY(ofVec3f pH,ofVec3f pL,float dX,float dY,float dH,float z){
    ofVec3f returnpoint;
    float divdH=1/dH;
    float h=z-pL.z;
    returnpoint.x=pL.x+dX*divdH*h;
    returnpoint.y=pL.y+dY*divdH*h;
    returnpoint.z=z;
    return returnpoint;
}

void gkmll::addnewline(ofIndexType ip0,ofIndexType ip1,ofIndexType ipn){
    cout<<"ip0,ip1:"<<ip0<<","<<ip1<<"\n";
    //cout<<"before"<<linelist.size();
    if(ip0<ip1){
        linelist.push_back(ip0);
        linelist.push_back(ip1);
        cout<<"0<1"<<"\n";
    }else{// ip1<ip0
        linelist.push_back(ip1);
        linelist.push_back(ip0);
        cout<<"1<0"<<"\n";
    }
    cout<<linelist[linelist.size()-2];
    
    cout<<linelist[linelist.size()-1];
    
    cout<<"\n";
    cout<<":"<<linelist.size()<<"\n";
    linehorizonlist.push_back(false);
    linehorizonlist.push_back(false);
    
    dXdYlist.push_back(0);
    dXdYlist.push_back(0);
    
    nearpointlist.push_back(ipn);
    nearpointlist.push_back(ipn);
    
}
void gkmll::addoldline(ofIndexType ipl,ofIndexType ipn){
    if(nearpointlist[ipl]==nearpointlist[ipl+1]){
        nearpointlist[ipl+1]=ipn;
        return;
    }
}
ofIndexType gkmll::searchline(ofIndexType ip0,ofIndexType ip1){
    if(ip0<ip1){
        for(size_t j=0;j<linelist.size();j+=2){
            if(ip0==linelist[j]&&ip1==linelist[j+1]){
                
                //cout<<"find the same1"<<"\n";
                return j;
                
            }
        }
    }else{// ip1<ip0
        for(size_t j=0;j<linelist.size();j+=2){
            if(ip1==linelist[j]&&ip0==linelist[j+1]){
                
                //cout<<"find the same2"<<"\n";
                return j;
            }
        }
    }
    return -1;// false
}
ofVec3f gkmll::getLinePlaneIntersection(ofVec3f pointUp, ofVec3f pointDown, float z){
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
ofPath gkmll::addPointToPath(ofPath path,float x,float y,ofIndexType i){
    ofPath returnpath;
    if(i==0){
        returnpath.moveTo(x,y);
    }else{
        returnpath=path;
        returnpath.lineTo(x,y);
    }
    //cout<<"we get the x y :"<<x<<","<<y<<"\n";
    return returnpath;
}

