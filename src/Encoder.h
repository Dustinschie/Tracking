//
//  Encoder.h
//  Tracking
//
//  Created by Dustin Schie on 9/29/14.
//
//

#ifndef __Tracking__Encoder__
#define __Tracking__Encoder__

#include <iostream>
#include <vector>
#include "ofMain.h"

typedef unsigned char byte;
class Encoder
{
public:
    Encoder();
    vector<byte> convertIntToByteVector(unsigned int bytes);
    vector<byte> convertShortToByteVector(unsigned short bytes);
    vector<byte> convertofPointToByteVector(ofPoint &point);
    
};

#endif /* defined(__Tracking__Encoder__) */
