#pragma once
#include "ofMain.h"
/**
this is not work now 
 **/
class agtimemanager{
public:
    void startPrint(){
        if(state==statemodelReady){
            state=statemodelPrepare;
        }
    
    
    }
    void update(){
    
        if(state==statemodelPrepare){
            
        }
    }
    int state=0;
    const stateSleep=0;
    const statemodelPrepare=1;
    const statemodelReady=2;
    const statemodelPrintReady=3;
    const statemodelPrintReset=4;
    const statemodelPrintExposing=5;
    const statemodelPrintSendMessage=6;

}


