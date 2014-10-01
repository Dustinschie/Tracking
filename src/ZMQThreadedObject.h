//
//  ZMQThreadedObject.h
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#ifndef __Tracking__ZMQThreadedObject__
#define __Tracking__ZMQThreadedObject__

#include <iostream>

#include "ofMain.h"

//#include <zmq.hpp>
#include "zmq2.h"

#include "Bot.h"

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
#endif



using namespace std;

typedef unsigned char byte;

class ZMQThreadedObject: public ofThread
{
public:
    ZMQThreadedObject(string port, string ip = "*");
    ZMQThreadedObject()
    {
        portAndIP = "tcp://*:5555";
        //  Prepare our context and socket
        zmq::context_t context(1);
    }
    void start();
    void stop();
    void threadedFunction();
    
    void setBots(map <int, Bot> &bots);
    void setFrameDimensions(ofPoint& dim);
private:
    
    zmq::context_t  context;
    string          portAndIP;
    map<int, Bot>   bots;
    unsigned long long    elapsedMilliseconds, startTime;
    ofPoint dimensions;
    unsigned short averageRadius;
    
    bool sendBotInformation(zmq::Socket &socket, int messageKey, int roboID = -1);
    vector<byte> getBotsInformation();
    vector<byte> breakupTimeStamp();
    vector<byte> shortToByteVector(unsigned short theShort);
    
};

#endif /* defined(__Tracking__ZMQThreadedObject__) */
