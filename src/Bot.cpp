//
//  Bot.cpp
//  Tracking
//
//  Created by Dustin Schie on 1/26/14.
//
//

#include "Bot.h"
Bot::Bot(int bot_id)
{
    my_id = bot_id;
    color.r = ofRandom(255);
    color.g = ofRandom(255);
    color.b = ofRandom(255);
    isNew = true;
    path = ofPolyline();
}

//--------------------------------------------------------------
Bot::Bot(ofxCvBlob& a_blob, int bot_id, int r = ofRandom(255), int g = ofRandom(255), int b = ofRandom(255))
{
    my_id = bot_id;
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
    color.r = r;
    color.g = g;
    color.b = b;
    timeStamp = ofGetUnixTime();
    path = ofPolyline();
}

//--------------------------------------------------------------
Bot::Bot(ofxCvBlob& a_blob, int bot_id, ofColor a_color)
{
    my_id = bot_id;
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
    color = a_color;
    path = ofPolyline();
    timeStamp = ofGetUnixTime();
}

//--------------------------------------------------------------
void Bot::updatePosition(ofxCvBlob& a_blob)
{
    blob = a_blob;
    rect = ofRectangle(blob.boundingRect);
//    path.push_back(a_blob.centroid);
    path.addVertex(a_blob.centroid);
    
}

//--------------------------------------------------------------
int Bot::updatePosition(vector<ofxCvBlob>& blobs)
{
    for (vector<ofxCvBlob>::iterator it = blobs.begin(); it != blobs.end(); it++)
    {
        if (rect.inside(it->centroid))
        {
            unsigned int newTime = ofGetUnixTime();
            int t = newTime - timeStamp;
            blob = *it;
            rect = blob.boundingRect;
            timeStamp = newTime;
            int elapse = ofGetElapsedTimeMillis() / 250.0;
            if (elapse % 2 == 0)
            {
//                path.push_back(it->centroid);
                path.addVertex(it->centroid);
            }
            return it - blobs.begin();
        }
    }
    return -1;
}

//--------------------------------------------------------------
ofPoint Bot::getCenter()
{
    return ofPoint(blob.centroid);
}
//--------------------------------------------------------------
ofxCvBlob Bot::getBlob()
{
    return ofxCvBlob(blob);
}


//--------------------------------------------------------------
int Bot::calculateVelocity(ofVec2f& pF, ofVec2f& p0,  long tF,  long t0)
{
    long delta_t = tF - t0;
    if (delta_t != 0)
    {
        double distance = p0.distance(pF);
        return abs((distance / delta_t));
    }
    return 0;
    
}

//--------------------------------------------------------------
int Bot::getId()
{
    return my_id;
}

//--------------------------------------------------------------
unsigned int Bot::getTimeStamp()
{
    return timeStamp;
}
//--------------------------------------------------------------
ofPoint Bot::getSize()
{
    int h = (int)(blob.boundingRect.height + 0.5),
        w = (int)(blob.boundingRect.width + 0.5);
    return ofPoint(w, h);
}
//--------------------------------------------------------------
int Bot::getRadius()
{
    ofPoint size = getSize();
    if (size.x > size.y)
        return size.x;
    return size.y / 2;
}
//--------------------------------------------------------------
string Bot::infoString()
{
    stringstream ss;
    ofPoint centroid = blob.centroid;
    ss << my_id << "\t" << (int)centroid.x << "," << (int)centroid.y;
    string s = ss.str();
    ss.str("");
     return s;
}