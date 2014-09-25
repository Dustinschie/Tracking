//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"

//#define VERBOSE
#define RESET_BG 0
#define START_TRACKING 1
#define ADD_BOT 2
#define GATHER_DIMENSIONS 3
#define OBTAIN_POSITION 4
#define GET_ALL_INFO 5


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
        string received = socket.recvAsString();
        ofSendMessage(received);

        
#ifdef VERBOSE
        std::cout << "Received: " << received << std::endl;
#endif
//        ofSleepMillis(delay * 1000);
        sendBotInformation(socket, received[0] - '0', received.size() > 1 ? received[1] - '0' : -1);
    }
}

bool ZMQThreadedObject::sendBotInformation(zmq::Socket &socket, int messageKey, int roboID)
{
    if(lock())
    {
//#define RESET_BG 0
//#define START_TRACKING 1
//#define ADD_BOT 2
//#define GATHER_DIMENSIONS 3
//#define OBTAIN_POSITION 4
//#define GET_ALL_INFO 5
        byte id = messageKey;
        switch (messageKey) {
            case RESET_BG:
            case START_TRACKING:
            {
                byte info = messageKey;
                startTime = ofGetElapsedTimeMillis();
                socket.send(info);
                break;
            }
            case ADD_BOT:
            {
                byte info[9];
                info[0] = id;
                info[1] = roboID;
                Bot bot = bots[roboID];
                ofPoint center = bot.getCenter();
                info[2] = (byte)((unsigned short)center.x);
                info[3] = (byte)((unsigned int)center.x >> 8);
                info[4] = (byte)((unsigned short)center.y);
                info[5] = (byte)((unsigned int)center.y >> 8);
                info[6] = (byte)((unsigned short)center.z);
                info[7] = (byte)((unsigned int)center.z >> 8);
                info[8] = (byte)((unsigned short)bot.getRadius());
                socket.sendByteArray(info, 9);
                break;
            }
            case OBTAIN_POSITION:
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
                break;
            }
            default:
                break;
        }

        
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
    unsigned int time = (unsigned int)(ofGetElapsedTimeMillis() - startTime);
    
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
        int sigmaRad = 0;
        for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++)
        {
            ofPoint dim = it->second.getSize();
            if (dim.x > dim.y)
            {
                sigmaRad += dim.x;
            } else
            {
                sigmaRad += dim.y;
            }
        }
        sigmaRad /= bots.size();
        averageRadius = sigmaRad;
        this->unlock();
    }
}

void ZMQThreadedObject::setFrameDimensions(ofPoint& dim)
{
    if(this->lock())
    {
        dimensions = ofPoint(dim);
    }
}