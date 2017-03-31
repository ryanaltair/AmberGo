#pragma once
#include "ofMain.h"
class easyLogTimer{
public:
    easyLogTimer(){
    }
    
    float timekeep;
    void from(string title){
        cout<<"------";
        timekeep=ofGetElapsedTimef();
        cout<<title<<" from:"<<ofToString(timekeep) <<" senconds "<<endl;
    }
    void to(string title){
        cout<<"------";
        float timetake=ofGetElapsedTimef()-timekeep;
        timekeep=ofGetElapsedTimef();
        cout<<title<<"  to:"<<ofToString(timekeep) <<" senconds "<<endl;
        cout<<"------";
        cout<<title<<"take:"<<ofToString(timetake) <<" senconds "<<endl;
    }
};
