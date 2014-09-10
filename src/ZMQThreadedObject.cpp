//
//  ZMQThreadedObject.cpp
//  Tracking
//
//  Created by Dustin Schie on 9/9/14.
//
//

#include "ZMQThreadedObject.h"
ZMQThreadedObject::ZMQThreadedObject(string port, string ip)
{
    portAndIP = "tcp://" + ip + ":" + port;
}
ZMQThreadedObject::ZMQThreadedObject()
{
    portAndIP = "tcp://*:5555";
}
void ZMQThreadedObject::start()
{
    startThread(true, false);
}
void ZMQThreadedObject::stop()
{
    stopThread();
}
void ZMQThreadedObject::threadedFunction()
{

    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    
    assert (rc == 0);
    int delay = 1, i = 95;
    while (isThreadRunning())
    {
        if (lock())
        {
            byte *recieved;
            //  Wait for next request from client
            zmq_recv(responder, recieved, 5, 0);
            cout << "Received:\t" << recieved << endl;
            
            //  Do some 'work'
#ifndef _WIN32
                sleep(1);
#else
//                sleep(1);
                Sleep(1000);
#endif
            
            //  Send reply back to client
            byte sending[5] = {(byte)i, (byte)(i + 1), (byte)(i + 2), (byte)(i + 3), '\0'};
            std::cout << "Sending:\t" << sending <<"\t and " << i << std::endl;
            i += 4;
            zmq_send(responder, sending, 5, 0);
            
            unlock();
        }
    }
}