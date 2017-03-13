#pragma once

#include "ofMain.h"
#include "agline.h"

class agmllhelper{
    agmllhelper();
    
    
};
class agModel{
public:
    agModel(){
    
    }
    
    vector<ofVec3f> pointlist;// get the real point p=pointlist[ip]
    vector<ofIndexType> linelist;// hlod {ipL ipH} the index point to point list add with addnewline() and pL will be lower
    map<agline,ofIndexType> linecopymap;// use for searchline , for great speed
    vector<bool> linehorizonlist;// is this line horizon true horizon // add with addnewline() work with adddXdY
    vector<ofIndexType> nearpointlist;//{ipa ipb} the index refer point list add with addnewline() and addoldline
    vector<float> dXdYlist;// init with addFacet() work with adddXdY()
    vector<int> linetypelist;//init with addFacet() work with adddXdY()
    vector<float> touchedlist;//init with addFacet() work with adddXdY()
    vector<float> horizonFacetHeightlist;// hold every height that get horizong facet/triangle
    

};
