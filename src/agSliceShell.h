#pragma once
#include "ofMain.h"
#include "agfacet.h"
#include "ofxClipper.h"
class agSliceShell{
public:
    agSliceShell(){
    }
    void load(ofMesh mesh){
    
    }
    
    ofMesh getShell(ofMesh mesh){
        ofxClipper clipper;
    //get every meshface first
        vector<ofMeshFace> faces;
        vector<ofMeshFace> newfaces;
        faces=mesh.getUniqueFaces();
        size_t facesize= faces.size();
        //now we try to get the newface
        for(int i=0;i<facesize;i++){
            if(faces[i].getFaceNormal().z<0){
                newfaces.push_back(faces[i]);
            }
        }
        //now we make new the mesh
        ofMesh newMesh;
        newMesh.setFromTriangles(newfaces,true);
        return newMesh;
    }
protected:
    ofMesh shell;
    ofPath supportPolygon;
    vector<int> grades;
    
};
