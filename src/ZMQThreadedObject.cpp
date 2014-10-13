//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"


#define VERBOSE
#define RESET_BG 0
#define START_TRACKING 1
#define ADD_BOT 2
#define GATHER_DIMENSIONS 3
#define OBTAIN_POSITIONS 4
#define OBTAIN_BLOBS_INFO 5


void ZMQThreadedObject::start()
{
    startThread(false, false);
    encoder = Encoder();
    
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
    

    float delay = 250;
    while (isThreadRunning())
    {
        //  Wait for next request from client
        string received = socket.recvAsString();
        ofSendMessage(received);

        ofSleepMillis(delay);
#ifdef VERBOSE
        std::cout << "Received: " << (int)received[0] <<"\t" << (int)received[1] << std::endl;
#endif
        int messageKey = (int)received[0];
        int robotID = (messageKey == 2 ? (int)received[1] : -1);
            sendBotInformation(socket, messageKey, robotID);
    }
}

bool ZMQThreadedObject::sendBotInformation(zmq::Socket &socket, int messageKey, int roboID)
{
    if(lock())
    {
        byte id = messageKey;
        switch ((unsigned int)messageKey) {
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
                if (bots.find(roboID) != bots.end())
                {
                    Bot bot = bots.find(roboID)->second;
                    ofPoint center = bot.getCenter();
                    info[2] = (byte)((unsigned int)center.x >> 8);
                    info[3] = (byte)((unsigned short)center.x);
                    info[4] = (byte)((unsigned int)center.y >> 8);
                    info[5] = (byte)((unsigned short)center.y);
                    info[6] = (byte)((unsigned int)center.z >> 8);
                    info[7] = (byte)((unsigned short)center.z);

                    info[8] = (byte)((unsigned short)bot.getRadius());
                } else
                {
                    info[2] = (byte)((unsigned short)0);
                    info[3] = (byte)((unsigned short)0);
                    info[4] = (byte)((unsigned short)0);
                    info[5] = (byte)((unsigned short)0);
                    info[6] = (byte)((unsigned short)0);
                    info[7] = (byte)((unsigned short)0);
                    info[8] = (byte)((unsigned short)0);
                }
#ifdef VERBOSE
                for (int i = 0; i < 9; i++)
                    cout << "\t"<< bitset<8>(info[i]) << "\t" << (short)info[i] << "\n";
                
                cout << "\t|" << 9 << "\n" << endl;
                cout << "Sending: "<< info << endl;
#endif
                socket.sendByteArray(info, 9);
                break;
            }
            case GATHER_DIMENSIONS:
            {
                byte info[5];
                info[0] = (byte)GATHER_DIMENSIONS;
                info[1] = (byte)((unsigned short) dimensions[0]>> 8);
                info[2] = (byte)((unsigned short) dimensions[0]);
                info[3] = (byte)((unsigned short) dimensions[1] >> 8);
                info[4] = (byte)((unsigned short) dimensions[0]);


                socket.sendByteArray(info, 5);
#ifdef VERBOSE
                for (int i = 0; i < 5; i++)
                    cout << "\t"<< bitset<8>(info[i]) << "\t" << (short)info[i] << "\n";
                
                cout << "\t|" << 5 << "\n" << endl;
                cout << "Sending: "<< info << endl;
#endif
                break;
            }
            case OBTAIN_POSITIONS:
            {
                vector<byte>info = getBotsInformation();
                unsigned char sendingInfo[info.size()];
                std::copy(info.begin(), info.end(), sendingInfo);
#ifdef VERBOSE
                for (int i = 0; i < info.size(); i++)
                    cout << "\t"<< bitset<8>(sendingInfo[i]) << "\t" << (short)sendingInfo[i] << "\n";
                
                cout << "\t|" << info.size() <<"\t" << (info[info.size() - 1] == 0) << endl;
                cout << "Sending: "<< sendingInfo << endl;
#endif
                socket.sendByteArray(sendingInfo, info.size());
                break;
            }
            default:
                socket.send(messageKey);
                break;
        }

        
        unlock();
    }
}
//--------------------------------------------------------------
// intttt{rxxyyzz * n}
vector<byte> ZMQThreadedObject::getBotsInformation()
{
    vector<byte> info;
    unsigned int time = (unsigned int)(ofGetElapsedTimeMillis() - startTime);
    vector<byte> brokenTimeStamp = encoder.convertIntToByteVector(time);
    // push i
    info.push_back((byte)((unsigned short)OBTAIN_POSITIONS));
    // push number of bots
    info.push_back((byte)bots.size());
    // push time stamp
    info.insert(info.end(), brokenTimeStamp.begin(), brokenTimeStamp.end());
    

    //  push bot info
    for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++)
    {
        Bot b = it->second;
        
        byte id = (byte)b.getId();
        //  push bot ID
        info.push_back(id);
        
        ofPoint p = b.getCenter();
        vector<byte> points[3];
        points[0] = encoder.convertShortToByteVector((unsigned short) p.x);
        points[1] = encoder.convertShortToByteVector((unsigned short) p.y);
        points[2] = encoder.convertShortToByteVector((unsigned short) p.z);
        cout << p.x << "\t" << p.y << "\t" << p.z << "\t" << endl;
        
        //  push X, Y, Z coordinates
        for (int i = 0; i < 3; i++)
            info.insert(info.end(), points[i].begin(), points[i].end());
    }
    
    return info;
}

//--------------------------------------------------------------
// intttt{xxyyzzwwhh * n} or intttt{xxyyzzq{xxyyzz * q} * n}
vector<byte> ZMQThreadedObject::getBlobInformation(bool verbose)
{
    vector<byte> bytes;
    //  i
    bytes.push_back((byte)OBTAIN_BLOBS_INFO);
    //  n
    bytes.push_back((byte)blobs.size());
    //  tttt
    unsigned int time = (unsigned int)(ofGetElapsedTimeMillis() - startTime);
    vector<byte> brokenTimeStamp = encoder.convertIntToByteVector(time);
    bytes.insert(bytes.end(), brokenTimeStamp.begin(), brokenTimeStamp.end());
    
    // {xxyyzzwwhh * n} or {xxyyzzq{xxyy * q} * n}
    for(vector<ofxCvBlob>::iterator it = blobs.begin(); it != blobs.end(); it++)
    {
        // xxyyzz
        ofPoint center = it->centroid;
        vector<byte> ctrPts = encoder.convertofPointToByteVector(center);
        bytes.insert(bytes.end(), ctrPts.begin(), ctrPts.end());
        
        if(verbose)
        {
            
            vector<ofPoint> points = it->pts;
            //  q
            bytes.push_back(points.size());
            //  {xxyyzz * q}
            for (vector<ofPoint>::iterator it = points.begin(); it != points.end(); it++)
                vector<byte> pt = encoder.convertofPointToByteVector(*it);
        }
        else
        {
            // wwhh
            ofRectangle rect = it->boundingRect;
            vector<byte> size[2];
            size[0] = encoder.convertShortToByteVector((unsigned short)rect.width);
            size[1] = encoder.convertShortToByteVector((unsigned short)rect.height);
            for (int i = 0; i < 2; i++)
            {
                vector<byte> temp = size[i];
                bytes.insert(bytes.end(), temp.begin(), temp.end());
            }
        }
    }
    return bytes;
}

#pragma mark - Setters for the main thread to use
//--------------------------------------------------------------
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
        int size = bots.size();
        if (size > 0)
        {
            sigmaRad /= bots.size();
            averageRadius = sigmaRad;
        }
        else
        {
            averageRadius = 0;
        }
;
        this->unlock();
    }
}

//--------------------------------------------------------------
void ZMQThreadedObject::setBlobs(vector<ofxCvBlob> &someBlobs)
{
    if (this->lock()) {
        blobs.clear();
        blobs = vector<ofxCvBlob>(someBlobs.begin(), someBlobs.end());
        this->unlock();
    }
}
//--------------------------------------------------------------
void ZMQThreadedObject::setFrameDimensions(ofPoint& dim)
{
    if (this->lock()) {
//        cout << "updated" << endl;
        dimensions = ofPoint(dim);
        this->unlock();
    }

}