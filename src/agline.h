#pragma once

#include "ofMain.h"

class agline{
public:
    ofIndexType ip0,ip1;
    agline(){
    }
    void set(ofIndexType ip0Value,ofIndexType ip1Value){
        ip0=ip0Value;
        ip1=ip1Value;
    }
    bool operator < ( const agline& other) const
    {
        if ( ip0 == other.ip0 ) {
            
            return ip1 < other.ip1;
        }
        
        return ip0 < other.ip0;
    }
    
};
