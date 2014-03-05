//
//  TCPThread.h
//  Tracking
//
//  Created by Dustin Schie on 2/14/14.
//
//

#ifndef Tracking_TCPThread_h
#define Tracking_TCPThread_h
#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxOscParameterSync.h"

#include <map>


class TCPThread : public ofThread{
public:
    
    TCPThread();
    void start();
    void stop();
    void threadedFunction();
protected:
    
    string getOscMsgAsString(ofxOscMessage m);
    
    
    map<int, string> info;
    stringstream botInfo;
    ofxTCPServer server;
    
    ofxOscReceiver messageReciever;
    int messageRecvPort;
    string message;
    
};


#endif
