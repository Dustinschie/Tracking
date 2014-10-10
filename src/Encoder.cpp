//
//  Encoder.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/29/14.
//
//

#include "Encoder.h"
Encoder::Encoder()
{
    
}
//--------------------------------------------------------------
vector<byte> Encoder::convertIntToByteVector(unsigned int bytes)
{
    vector<byte> encoded;
    encoded.push_back((byte)(bytes >> 8));
    encoded.push_back((byte)bytes);
    
    return encoded;
}
//--------------------------------------------------------------
vector<byte> Encoder::convertShortToByteVector(unsigned short bytes)
{
    vector<byte> brokenTime;
    for (int i = 0; i < 4; i++)
    {
        brokenTime.insert(brokenTime.begin(), (byte)bytes);
        bytes >>= 8;
    }
    return brokenTime;
}
//--------------------------------------------------------------
vector<byte> Encoder::convertofPointToByteVector(ofPoint &point)
{
    vector<byte> pts;
    vector<byte> coords[3];
    coords[0] = convertShortToByteVector((unsigned short)point.x);
    coords[1] = convertShortToByteVector((unsigned short)point.y);
    coords[2] = convertShortToByteVector((unsigned short)point.z);
    
    for (int i = 0; i < 3; i++)
    {
        pts.insert(pts.end(), coords[i].begin(), coords[i].end());
    }
    return pts;
}