//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"

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
    while (isThreadRunning())
    {
        zmq::message_t request;
        //  Wait for next request from client
        short received = socket.recvAsShort();
//        socket.recv (&request);
//        byte* received = (byte *)request.data();
//        string receivedString((char *)received);
        std::cout << "Received: " << received << std::endl;
        
            
            ofSleepMillis(delay * 1000);
        if(lock())
        {
            //  Send reply back to client
            zmq::message_t reply (sizeof(byte));
//            byte sending[5] = { (byte)i,
//                                (byte)(i + 1),
//                                (byte)(i + 2),
//                                (byte)(i + 3),
//                                '\0'};
            
//            sendBotInformation(0);
//            byte b = (byte)bots.size();
//            byte sending = *(static_cast<int *>(bots.size());
//            memcpy((void *)reply.data(), &sending, sizeof(byte));
            
            i += 4;
            cout << "Sending: "<< bots.size() << endl;
            socket.send((short)bots.size());
            
//            zmq::message_t numOfBots(1);
//            byte* b = (byte *)((int)bots.size());
//            cout << b << endl;
//            memcpy((void *) numOfBots.data(), b, 1);
//            socket.send(numOfBots);
            unlock();
        }
    }
}

//void ZMQThreadedObject::sendNumberOfBots(zmq::socket_t &socket)
//{
////    if(lock())
////    {
//        zmq::message_t numOfBots(1);
//        byte* b = (byte *)((int)bots.size());
////        cout << b << endl;
////        memcpy((void *) numOfBots.data(), b, sizeof(b));
////        socket.send(numOfBots);
//////        unlock();
//////    }
////}
//
//void ZMQThreadedObject::sendBotInformation(int bot_id)
//{
//    int size = 5;
//    byte* uTime =(byte*) ofGetUnixTime();
//    try
//    {
//        Bot bot = bots.at(bot_id);
//        
//        byte* botTimeStamp = (byte *)bot.getTimeStamp();
//        byte butTimeStampPieces[4];
//        memcpy(butTimeStampPieces, botTimeStamp, 4);
//        
//        cout << butTimeStampPieces << endl;
//        byte x   = (byte) bot.getCenter().x,
//             y   = (byte) bot.getCenter().y,
//             id  = (byte) bot.getId();
//        
//        
////        = {uTime, id, x, y, botTimeStamp, '\0'};
////        zmq::message_t info(11);
//        
//    }
//    catch(int exception)
//    {
//        
//    }
//}

void ZMQThreadedObject::setBots(map<int, Bot> &aBots)
{
    if(this->lock())
    {
        bots.clear();
        bots = map<int, Bot>(aBots.begin(), aBots.end());
        this->unlock();
    }
}

//bool ZMQThreadedObject::send(int value, int flags =0)
//{
//                zmq::message_t msg(sizeof(int));
//                memcpy(msg.data(), &value, sizeof(int));
//                return socket_t::send(msg, flags);
//}
