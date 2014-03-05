//
//  Grid.cpp
//  Tracking
//
//  Created by Dustin Schie on 1/21/14.
//
//

#include "Grid.h"
using namespace std;

Grid_stuff::Grid_stuff(){}
//--------------------------------------------------------------

/**
 *  Determine top-left, bottom-left, top-right, and bottom-right corner
 *  Given the four corner points, we have to determine the top-left, bottom-left, top-right, and bottom-right corner so we can apply the perspective
 *  correction.
 *  To determine the top-left, bottom-left, top-right, and bottom right corner, we’ll use the simplest method:
 *      Get the mass center.
 *      Points that have lower y-axis than mass center are the top points, otherwise they are bottom points.
 *      Given two top points, the one with lower x-axis is the top-left. The other is the top-right.
 *      Given two bottom points, the one with lower x-axis is the bottom-left. The other is the bottom-right.
 */
void Grid_stuff::sort_corners(vector<ofPoint>& corners, ofPoint center){
    vector<ofPoint> top, bot;
    
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }
    
    ofPoint tl = top[0].x > top[1].x ? top[1] : top[0];
    ofPoint tr = top[0].x > top[1].x ? top[0] : top[1];
    ofPoint bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    ofPoint br = bot[0].x > bot[1].x ? bot[0] : bot[1];
    
    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}

//--------------------------------------------------------------
/** returns the center of a group of points */
ofPoint Grid_stuff::centroid(vector<ofPoint>& points){
    ofPoint center(0,0);
    for (vector<ofPoint>::iterator it = points.begin(); it != points.end(); it++) {
        center.x += it->x;
        center.y += it->y;
    }
    center.x /= points.size();
    center.y /= points.size();
    return center;
}

//--------------------------------------------------------------
/** 
 *  3 scenarios:
 *      1.  same number of blobs in previous and current 
 *      2.  previous > current
 *      3.  previous < current
 */
void Grid_stuff::update_blob_position(map<int, ofPoint>& prev_blob_pts, vector<ofPoint>& current_pts){
    
    
}


//--------------------------------------------------------------
void Grid_stuff::clear_old_blobs(vector<ofxCvBlob>& blobs, vector<ofxCvBlob>& old){
    
}

//--------------------------------------------------------------
int Grid_stuff::get_pt_distance(ofPoint& p1, ofPoint& p2){
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
