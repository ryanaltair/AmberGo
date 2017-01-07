#pragma once
#include "ofMain.h"
/**
 app layerout preference store here
 
 
 all toggle will put in here
 
 
 
 TODO:
 - 由多方面构成 主要由以下影响
 - 拔模高度 readygap的高度 fixed
 - 上升速度 unfixed
 - 停整时间 拔模完成后等待下降的时间 really small and fixed
 - 下降速度 fixed
 - 曝光时间 main option
 - 层厚（层数）main option
 unit: mm second
 **/
class agAppPreference{
public:
    // layerout unit:pixels
    // the big screen
    int screenX;
    int screenY;
    int screenWidth;
    int screenHeight;
    ofRectangle screenview;
    // the model view
    ofRectangle plateview;
    // the slice view
    ofRectangle sliceview;
    // the panelview;
    ofRectangle panelview;
    // update the layerout
    agAppPreference(){
        updatelayerout(0);
    }
    void updatelayerout(float panelwidth){
        //the main layout
        screenX= ofGetWindowPositionX();
        screenY= ofGetWindowPositionY();
        screenWidth = ofGetWindowWidth();
        screenHeight= ofGetWindowHeight();
        screenview.set(screenX,screenY,screenWidth,screenHeight);
        
        // the model view
        float platewidth=screenWidth-panelwidth;
        plateview.set(0,0,platewidth,screenHeight);
        //modelview.set(0,0,320,200);
        // the slice view
        
        sliceview.set(platewidth,screenHeight-panelwidth*xyratio,panelwidth,panelwidth*xyratio);
        
    }
    //printer scale
    ofVec3f printscale;
    float exposedTime;
    float layerThickness;

    float xyratio=768.0/1280.0; // use for slice view ratio y= x* xyratio

    
    //the toggle

    bool bPrint=false;
    
    
    bool isModelChanged=false;
    ofVec3f getmmperpixel(){
        ofVec3f lcd1280768;
        lcd1280768.set(1280,768,1280+768);
        ofVec3f lcdinch57;
        lcdinch57.set(126.72,74.88,126.72+74.88);
        ofVec3f mmperpixel;
        mmperpixel=lcdinch57/lcd1280768;
        
        return mmperpixel;
    };
    ofVec3f getpixelpermm(){
        ofVec3f lcd1280768;
        lcd1280768.set(1280,768,1280+768);
        ofVec3f lcdinch57;
        lcdinch57.set(126.72,74.88,126.72+74.88);
        ofVec3f pixelpermm;
        pixelpermm=lcd1280768/lcdinch57;
        return pixelpermm;
    };
    ofVec3f getscalefactor(){
        ofVec3f f;
        f.set(1,1,1);
        return f;
    };
    
};