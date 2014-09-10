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
#include <zmq.h>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
#endif


using namespace std;

typedef char byte;

class ZMQThreadedObject: public ofThread
{
public:
    ZMQThreadedObject(string port, string ip = "*");
    ZMQThreadedObject();
    void start();
    void stop();
    void threadedFunction();
    

private:
    string portAndIP;
};

#endif /* defined(__Tracking__ZMQThreadedObject__) */
