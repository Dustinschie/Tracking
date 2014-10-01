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
    vector<byte> convertToByteVector(unsigned int bytes);
    vector<byte> convertToByteVector(unsigned short bytes);
    
};

#endif /* defined(__Tracking__Encoder__) */
