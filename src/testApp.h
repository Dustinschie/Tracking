#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxNetwork.h"
#include "ofxOscMessage.h"

#include <vector>
#include <sstream>
#include <map>

#include "Bot.h"
#include "TCPThread.h"

//#define _USE_LIVE_VIDEO

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

    bool				bLearnBakground;


    string              label;

    ofColor             colors[18];
    ofColor             reticlColor;
    ofPoint             mouse_point;
    map<int, Bot>       bots;
    vector<ofxCvBlob>   blobs;

    ofPoint             botIDLocation,
                        botRectLocation,
                        botCoordLocation,
                        botPathLocation;
    ofVec2f             vidSize;

    void                drawPath(vector<ofPoint>& path);
    void                drawInfoStrings(string s,
                                        ofPoint& info);
    void                drawAssociation(ofPoint& botCenter, ofPoint& pt);

    int                 offset,
                        potential_bot_id,
                        reticleRotationNum,
                        botColorNum;
    //  vector that will contain the keys to bots that no longer exist
    vector<int>         black_list;
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
                        autoFindBots,
                        showReticle;

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

    //--------------------------------------------------------------
    TCPThread tcpThread;
    ofxOscSender sender;
    int portNumber;

};
