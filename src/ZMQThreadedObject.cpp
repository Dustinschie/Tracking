//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"

//#define VERBOSE


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
    

    int delay = 1;
    while (isThreadRunning())
    {
        //  Wait for next request from client
        ofSendMessage(socket.recvAsString());
        
#ifdef VERBOSE
        std::cout << "Received: " << received << std::endl;
#endif
        ofSleepMillis(delay * 1000);
        sendBotInformation(socket);
    }
}

bool ZMQThreadedObject::sendBotInformation(zmq::Socket &socket)
{
    if(lock())
    {
        vector<byte>info = getBotsInformation();
        unsigned char sendingInfo[info.size()];
        std::copy(info.begin(), info.end(), sendingInfo);
#ifdef VERBOSE
        for (int i = 0; i < info.size(); i++)
        {
            cout << "\t"<< bitset<8>(sendingInfo[i]) << "\t" << (short)sendingInfo[i] << "\n";
            
        }
        
        cout << "\t|" << info.size() <<"\t" << (info[info.size() - 1] == 0) << endl;
        cout << "Sending: "<< sendingInfo << endl;
#endif
        socket.sendByteArray(sendingInfo, info.size());
        
        unlock();
    }
}

vector<byte> ZMQThreadedObject::getBotsInformation()
{
    vector<byte> info;
    vector<byte> brokenTimeStamp = breakupTimeStamp();

    // push time stamp
    info.insert(info.end(), brokenTimeStamp.begin(), brokenTimeStamp.end());
    
    // push number of bots
    info.push_back((byte)bots.size());
    
    
    //  push bot info
    for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++)
    {
        Bot b = it->second;
        
        byte id = (byte)b.getId();
        //  push bot ID
        info.push_back(id);
        
        ofPoint p = b.getCenter();
        vector<byte> points[3];
        points[0] = shortToByteVector((unsigned short) p.x);
        points[1] = shortToByteVector((unsigned short) p.y);
        points[2] = shortToByteVector((unsigned short) p.z);
        
        //  push X, Y, Z coordinates
        for (int i = 0; i < 3; i++)
            info.insert(info.end(), points[i].begin(), points[i].end());
    }
    
    return info;
}

vector<byte> ZMQThreadedObject::shortToByteVector(unsigned short theShort)
{
    vector<byte> byteVector;

    byteVector.push_back((byte)(theShort >> 8));
    byteVector.push_back((byte)theShort);
    

    return byteVector;
}

vector<byte> ZMQThreadedObject::breakupTimeStamp()
{
    unsigned int time = ofGetUnixTime();
    vector<byte> brokenTime;
    for (int i = 0; i < 4; i++) {
        brokenTime.insert(brokenTime.begin(), (byte)time);
        time >>= 8;
    }
    return brokenTime;
}

void ZMQThreadedObject::setBots(map<int, Bot> &aBots)
{
    if(this->lock())
    {
        bots.clear();
        bots = map<int, Bot>(aBots.begin(), aBots.end());
        this->unlock();
    }
}