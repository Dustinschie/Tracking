//
//  Bot.cpp
//  Tracking
//
//  Created by Dustin Schie on 1/26/14.
//
//

#include "Bot.h"
Bot::Bot(int bot_id){
    my_id = bot_id;
    color.r = ofRandom(255);
    color.g = ofRandom(255);
    color.b = ofRandom(255);
    
}

//--------------------------------------------------------------
Bot::Bot(ofxCvBlob& a_blob, int bot_id, int r = ofRandom(255), int g = ofRandom(255), int b = ofRandom(255)){
    my_id = bot_id;
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
    color.r = r;
    color.g = g;
    color.b = b;
}

//--------------------------------------------------------------
Bot::Bot(ofxCvBlob& a_blob, int bot_id, ofColor a_color){
    my_id = bot_id;
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
    color = a_color;
}

//--------------------------------------------------------------
void Bot::update_position(ofxCvBlob& a_blob){
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
    path.push_back(a_blob.centroid);
    
}

//--------------------------------------------------------------
int Bot::update_position(vector<ofxCvBlob>& blobs){
    
    for (vector<ofxCvBlob>::iterator it = blobs.begin(); it != blobs.end(); it++) {
        if (rect.inside(it->centroid)) {
            blob = *it;
            rect = blob.boundingRect;
            path.push_back(it->centroid);
            
             long time = ofGetElapsedTimeMillis() / 1000;
            
            
            if (timed_data.size() >= 1) {
                timed_data.insert(pair< long, info>(time, info()));
                timed_data[time].centroid = blob.centroid;
                map< long, info>::reverse_iterator map_it = timed_data.rbegin();
                map_it--;
                ofVec2f pt(blob.centroid.x, blob.centroid.y);
                timed_data[time].direction = calculate_direction(pt, map_it->second.centroid);
                timed_data[time].angle = calculate_angle(pt);
                timed_data[time].velocity = calculate_velocity(pt, map_it->second.centroid,  time, map_it->first);
                direction = timed_data[time].direction;
                angle = timed_data[time].angle;
                velocity = timed_data[time].velocity;
                centroid = timed_data[time].centroid;
                if (timed_data.size() > 2) {
                    timed_data.erase(timed_data.begin());
                }
            }
            else{
                timed_data[time].direction = "N/A";
                timed_data[time].angle = 0.0;
                timed_data[time].velocity = 0.0;
                direction = timed_data[time].direction;
                angle = timed_data[time].angle;
                velocity = timed_data[time].velocity;
                centroid = timed_data[time].centroid;
            }
            return it - blobs.begin();
        }
    }
    
    return -1;
}

//--------------------------------------------------------------
ofPoint Bot::get_center(){
    return ofPoint(blob.centroid);
}

//--------------------------------------------------------------
ofxCvBlob Bot::get_blob(){
    return ofxCvBlob(blob);
}

//--------------------------------------------------------------
string Bot::calculate_direction(ofVec2f& pF, ofVec2f& p0){
    int x = p0.x - pF.x,
        y = p0.y - pF.y;
    
    
    if (abs(x) > abs(y)) {
        if (x < 0)
            return "E";
    
        else
            return "W";
        
    }
    
    else if(abs(x) < abs(y)){
        if (y < 0)
            return "S";
        else
            return "N";
    }

    return "N/A";
}

//--------------------------------------------------------------
int Bot::calculate_angle(ofVec2f& pt){
    if (pt.x != 0)
        return (int)(atan(pt.y / pt.x) * 180 / PI);
    return 0;
}

//--------------------------------------------------------------
int Bot::calculate_velocity(ofVec2f& pF, ofVec2f& p0,  long tF,  long t0){
    long delta_t = tF - t0;
    if (delta_t != 0) {
        double distance = p0.distance(pF);
        return abs((distance / delta_t));
    }
    return 0;
    
}

//--------------------------------------------------------------
string Bot::infoString(){
    stringstream ss;
    ss << my_id << "\t" << (int)centroid.x << "," << (int)centroid.y << "\t" << direction << "\t" << angle << "\t" << velocity;
    string s = ss.str();
    ss.str("");
     return s;
}