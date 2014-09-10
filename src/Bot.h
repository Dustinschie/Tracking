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
    
    void update_position(ofxCvBlob& a_blob);
    int update_position(vector<ofxCvBlob>& blobs);
    
    string infoString();
    ofPoint get_center();
    ofxCvBlob get_blob();
    
//    vector<ofPoint> path;
    ofPolyline path;
    ofColor color;
    ofRectangle rect;
    
private:
    int my_id;
    int velocity;
    bool isNew;
    int calculate_velocity(ofVec2f& pF, ofVec2f& p0, long tF, long t0);
    ofxCvBlob blob;
    unsigned int timeStamp;
    
};

#endif /* defined(__Tracking__Bot__) */
