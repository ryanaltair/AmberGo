#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings mainWindowSetting;
    ofGLFWWindowSettings sideWindowSetting;
    mainWindowSetting.width = 800;
    mainWindowSetting.height = 600;
    mainWindowSetting.windowMode=OF_WINDOW;
    //settings.setPosition(ofVec2f(300,0));
    mainWindowSetting.resizable = true;
    mainWindowSetting.monitor=0;
    auto mainWindow = ofCreateWindow(mainWindowSetting);
    
    sideWindowSetting.width = 1920;
    sideWindowSetting.height = 1080;
    sideWindowSetting.windowMode=OF_FULLSCREEN;

    //settings.setPosition(ofVec2f(0,0));
    sideWindowSetting.resizable = false;
    // uncomment next line to share main's OpenGL resources with gui
    sideWindowSetting.shareContextWith = mainWindow;

    sideWindowSetting.monitor=1;
    auto sideWindow = ofCreateWindow(sideWindowSetting);
    sideWindow->setWindowTitle("sidePlay");
    sideWindow->setVerticalSync(false);
    
   
    shared_ptr<ofApp> mainApp(new ofApp);
    //mainApp->setupGui();
    ofAddListener(sideWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
