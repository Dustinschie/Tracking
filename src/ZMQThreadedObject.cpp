//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"

#define RESET 0
#define NEXT  1
#define PREV  2

#define VERBOSE


void ZMQThreadedObject::start()
{
    startThread(false, false);
    
}
void ZMQThreadedObject::stop()
{
    context.close();
    stopThread();
}
void ZMQThreadedObject::threadedFunction()
{
    
//    zmq::socket_t socket (context, ZMQ_REP);
//    socket.bind (portAndIP.c_str());
    
    zmq::Socket socket (context, ZMQ_REP);
    socket.bind(portAndIP.c_str());
    

    int delay = 1, i = 0;
    queue<unsigned short> info = getBotsInformation();
    while (isThreadRunning())
    {
        zmq::message_t request;
        //  Wait for next request from client
        unsigned short received = socket.recvAsUnsignedShort(),
        nextToSend;
        hasAConnection = true;
        switch (received) {
            case RESET:
                info = getBotsInformation();
                nextToSend = info.front();
                info.pop();
                break;
            case NEXT:
                nextToSend = info.front();
                info.pop();
                if (info.size() == 0) {
                    hasAConnection = false;
                }
                break;
            default:
                break;
        }
#ifdef VERBOSE
        std::cout << "Received: " << received << std::endl;
#endif
            ofSleepMillis(delay * 1000);
        if(lock())
        {
#ifdef VERBOSE
            cout << "Sending: "<< nextToSend << endl;
#endif
            
            socket.send(nextToSend);
            unlock();
        }
    }
}

queue<unsigned short> ZMQThreadedObject::getBotsInformation()
{
    vector<byte> info;
    byte* brokenTimeStamp = breakupTimeStamp();

    // push time stamp
    for (deque<unsigned short>::iterator it = brokenTimeStamp.begin(); it != brokenTimeStamp.end(); it++) {
        unsigned short val = *it;
        byte vals[2]
        
        info.push_back(a);
        info.push_back(b);
    }
    
    // push number of bots
    info.push_back((byte)bots.size());
    
    //  push number bot information
    for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++) {
        Bot b = it->second;
        ofPoint p = b.getCenter();
        
        unsigned short x, y, z;
        byte id, x1, x2, y1, y2, z1, z2;

        x = (unsigned short) p.x;
        y = (unsigned short) p.y;
        z = (unsigned short) p.z;
        
        id = (byte) b.getId();
        
        
        info.push(id);
        info.push(x);
        info.push(y);
        info.push(z);
    }
    
    return info;
}

byte* ZMQThreadedObject::shortToByteArray(unsigned short theShort)
{
    static byte byteArray[2];
    byteArray[0] = (byte)theShort;
    byteArray[1] = (byte)theShort >> 8;
    return byteArray;
}

byte* ZMQThreadedObject::breakupTimeStamp()
{
    unsigned int time = ofGetUnixTime();
    static byte brokeTimeArray[4];
    for (int i = 0; i < 4; i++) {
        brokeTimeArray[3 - i] = (byte) time;
        time >> 8;
    }
    return brokeTimeArray;
}

void ZMQThreadedObject::setBots(map<int, Bot> &aBots)
{
    if (!hasAConnection) {
        if(this->lock())
        {
            bots.clear();
            bots = map<int, Bot>(aBots.begin(), aBots.end());
            this->unlock();
        }
    }
}