#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofSetupOpenGL(800,600-22, OF_WINDOW);			// <-------- setup the GL context
    ofRunApp( new ofApp());
/**
    ofGLFWWindowSettings mainWindowSetting;
    ofGLFWWindowSettings sideWindowSetting;
    mainWindowSetting.width = 800;
    mainWindowSetting.height = 600;
    mainWindowSetting.windowMode=OF_WINDOW;
    //settings.setPosition(ofVec2f(300,0));
    mainWindowSetting.resizable = true;
    mainWindowSetting.monitor=0;
    auto mainWindow = ofCreateWindow(mainWindowSetting);
    
    sideWindowSetting.width = 1280;
    sideWindowSetting.height = 768;
    
//    sideWindowSetting.windowMode=OF_WINDOW;

    sideWindowSetting.windowMode=OF_FULLSCREEN;
    sideWindowSetting.visible=false;
    //settings.setPosition(ofVec2f(0,0));
    sideWindowSetting.resizable = true;
    // uncomment next line to share main's OpenGL resources with gui
    sideWindowSetting.shareContextWith = mainWindow;

    //sideWindowSetting.monitor=1;
    auto sideWindow = ofCreateWindow(sideWindowSetting);
    sideWindow->setWindowTitle("sidePlay");
    sideWindow->setVerticalSync(false);
 
   
    shared_ptr<ofApp> mainApp(new ofApp);
    //mainApp->setupGui();
    ofAddListener(sideWindow->events().draw,mainApp.get(),&ofApp::drawSideWindow);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
 **/
}
