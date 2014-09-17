//
//  Bot.h
//  Tracking
//
//  Created by Dustin Schie on 1/26/14.
//
//

#ifndef __Tracking__Bot__
#define __Tracking__Bot__

#include <iostream>
#include <vector>
#include "ofMain.h"
#include "ofxOpenCv.h"

class Bot
{
public:
    Bot(int bot_id);
    Bot(ofxCvBlob& a_blob, int bot_id, int r, int g, int b);
    Bot(ofxCvBlob& a_blob, int bot_id, ofColor a_color);
    
    void updatePosition(ofxCvBlob& a_blob);
    int updatePosition(vector<ofxCvBlob>& blobs);
    
    string infoString();
    ofPoint getCenter();
    ofxCvBlob getBlob();
    int getId();
    unsigned int getTimeStamp();
    
//    vector<ofPoint> path;
    ofPolyline path;
    ofColor color;
    ofRectangle rect;
    
private:
    int my_id;
    int velocity;
    bool isNew;
    int calculateVelocity(ofVec2f& pF, ofVec2f& p0, long tF, long t0);
    ofxCvBlob blob;
    unsigned int timeStamp;
    
};

#endif /* defined(__Tracking__Bot__) */
