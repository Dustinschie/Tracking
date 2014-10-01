//
//  Encoder.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/29/14.
//
//

#include "Encoder.h"
vector<byte> Encoder::convertToByteVector(unsigned int bytes)
{
    vector<byte> encoded;
    encoded.push_back((byte)(bytes >> 8));
    encoded.push_back((byte)bytes);
    
    return encoded;
}
vector<byte> Encoder::convertToByteVector(unsigned short bytes)
{
    vector<byte> brokenTime;
    for (int i = 0; i < 4; i++)
    {
        brokenTime.insert(brokenTime.begin(), (byte)bytes);
        bytes >>= 8;
    }
    return brokenTime;
}