//
//  Grid.h
//  Tracking
//
//  Created by Dustin Schie on 1/21/14.
//
//

#ifndef __Tracking__Grid__
#define __Tracking__Grid__

#include <iostream>
#include <vector>
#include "ofMain.h"
#include "ofxOpenCv.h"


using namespace std;
class Grid_stuff{
public:
    Grid_stuff();
    void sort_corners(vector<ofPoint>& corners, ofPoint center);
    
    void clear_old_blobs(vector<ofxCvBlob>& blobs, vector<ofxCvBlob>& old);
    ofPoint centroid(vector<ofPoint>& points);
    
    void update_blob_position(map<int, ofPoint>& prev_blob_pts, vector<ofPoint>& current_pts);
    
    
    int get_pt_distance(ofPoint& p1, ofPoint& p2);
    
    stringstream ss;
};



#endif /* defined(__Tracking__Grid__) */
