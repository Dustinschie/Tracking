#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include <vector>
#include <sstream>
#include <map>

#include "Bot.h"
#include "ZMQThreadedObject.h"

//#define _USE_LIVE_VIDEO

#define SET_NEW_BACKGROUND 0
#define SET_BEGIN_TRACKING 1
#define SET_CAPTURE_NEW_BOT 2


class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    //--------------------------------------------------------------
    #ifdef _USE_LIVE_VIDEO
    ofVideoGrabber 		vidGrabber;
    #else
    ofVideoPlayer 		vidPlayer;
    #endif

    ofxCvColorImage		colorImg;

    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;

    ofxCvColorImage     displayedImage;
    ofxCvContourFinder 	contourFinder;


    //  GUI: label
    string              label;
    //  GUI: Array of color for Bots
    
    ofColor             colors[18];
//    ofPoint             mouse_point;
    
    map<int, Bot>       bots;
    vector<ofxCvBlob>   blobs;

    ofVec2f             vidSize;
    bool                shouldSetNewBackGround,
                        shouldBeginSendingBotInfo,
                        shouldCaptureNewBot;
    //  vector that will contain the keys to bots that no longer exist
    vector<int>         black_list;
    
    
    ZMQThreadedObject   zmqThread;
    int                 potential_bot_id,
                        botColorNum,
                        newBotID;

    void                drawInfoStrings(string s,
                                        ofPoint& info);
    void                drawAssociation(ofPoint& botCenter, ofPoint& pt);
    
    //--------------------------------------------------------------
    bool                showGUI;

    ofxIntSlider        thresholdSlider,
                        dilateSlider,
                        minimunThreshSlider,
                        maximunThreshSlider;

    ofxToggle           drawBotRectToggle,
                        drawBotIDToggle,
                        drawBotPathToggle,
                        drawAssociationToggle,
                        drawReportStringToggle,
                        autoFindBots;

    ofxButton           showColorImage,
                        showBackgroundImage,
                        showSubtractedImage,
                        showGrayImage;

    ofxLabel            alterationsLabel,
                        infoLabel,
                        imageLabel;


    ofxPanel            gui;

    void                showColorPressed();
    void                showBGPressed();
    void                showGrayPressed();
    void                showSubtractedPressed();

    int                 vidID;
};
