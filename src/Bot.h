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
struct info{
    ofVec2f centroid;
    string direction;
    int angle;
    int velocity;
};


class Bot{
public:
    Bot(int bot_id);
    Bot(ofxCvBlob& a_blob, int bot_id, int r, int g, int b);
    Bot(ofxCvBlob& a_blob, int bot_id, ofColor a_color);
    
    void update_position(ofxCvBlob& a_blob);
    int update_position(vector<ofxCvBlob>& blobs);
    string infoString();
    ofPoint get_center();
    ofxCvBlob get_blob();
    vector<ofPoint> path;
    map<long, info> timed_data;
    ofColor color;
        ofRectangle rect;
private:
    int my_id;
    ofVec2f centroid;
    string direction;
    int angle;
    int velocity;
    int calculate_angle(ofVec2f& pt);
    string calculate_direction(ofVec2f& pF, ofVec2f& p0);
    int calculate_velocity(ofVec2f& pF, ofVec2f& p0, long tF, long t0);
    ofxCvBlob blob;

    ofPoint prev_point;
    
    
    
};

#endif /* defined(__Tracking__Bot__) */
