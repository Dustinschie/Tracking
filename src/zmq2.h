//
//  zmq2.h
//  Tracking
//
//  Created by Dustin Schie on 9/17/14.
//
//

#ifndef Tracking_zmq2_h
#define Tracking_zmq2_h
/**
 * Extending zmq::socket_t to send/receive multipart messages.
 * This is the first version, where Frames is a std::vector, now deprecated.
 * In the second version Frames is a std::deque, see: http://thisthread.blogspot.com/2012/06/zeromq-multipart-message-as-deque.html
 *
 * More information here: http://thisthread.blogspot.com/2012/04/extending-zmqsockett.html
 * Improved multipart send discussed here: http://thisthread.blogspot.com/2012/05/improved-sending-for-zmqsocket.html
 * Int messages: http://thisthread.blogspot.com/2012/05/sendingreceiving-ints-over-zeromq.html
 * Inspired by czmq, as described in the ZGuide: http://zguide.zeromq.org/page:all#A-High-Level-API-for-MQ
 */

#pragma once

//#include <cstdint>
#include <vector>
#include <zmq.hpp>

namespace zmq
{
    typedef std::vector<std::string> Frames;
    
    class Socket : public socket_t
    {
    public:
        Socket(context_t& context, int type) : socket_t(context, type) {}
        
        Socket(context_t& context, int type, const std::string& id) : socket_t(context, type)
        {
            zmq_setsockopt(this->operator void*(), ZMQ_IDENTITY, id.c_str(), id.length());
        }
        
        bool send(const std::string& frame, int flags =0)
        {
            return send(frame.c_str(), frame.length(), flags);
        }
        
        bool send(const char* frame, int flags =0)
        {
            return send(frame, strlen(frame), flags);
        }
        bool sendByteArray(const unsigned char* frame, int size, int flags =0)
        {
            
            zmq::message_t msg(size);
            memcpy(msg.data(), frame, size);
            return socket_t::send(msg, flags);
        }
        
        bool send(const std::string& frame1, const std::string& frame2)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame2);
        }
        
        bool send(const char* frame1, const char* frame2)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame2);
        }
        
        bool send(const std::string& frame1, const std::string& frame2, const std::string& frame3)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            if(!send(frame2, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame3);
        }
        
        bool send(const char* frame1, const char* frame2, const char* frame3)
        {
            if(!send(frame1, ZMQ_SNDMORE))
                return false;
            if(!send(frame2, ZMQ_SNDMORE))
                return false;
            // last frame
            return send(frame3);
        }
        
        bool send(const Frames& frames)
        {
            if(!frames.size())
                throw error_t();
            
            // all frames but last one
            for(unsigned int i = 0; i < frames.size() - 1; ++i)
                if(!send(frames[i], ZMQ_SNDMORE))
                    return false;
            // last frame
            return send(frames.back());
        }
        
        bool send(int value, int flags =0)
        {
            zmq::message_t msg(sizeof(int));
            memcpy(msg.data(), &value, sizeof(int));
            return socket_t::send(msg, flags);
        }
        
        bool send(short value, int flags = 0)
        {
            zmq::message_t msg(sizeof(short));
            memcpy(msg.data(), &value, sizeof(short));
            return socket_t::send(msg, flags);
        }
        
        bool send(unsigned short value, int flags = 0)
        {
            zmq::message_t msg(sizeof(unsigned short));
            memcpy(msg.data(), &value, sizeof(unsigned short));
            return socket_t::send(msg, flags);
        }
        
        bool send(char value, int flags = 0)
        {
            zmq::message_t msg(sizeof(char));
            memcpy(msg.data(), &value, sizeof(char));
            return socket_t::send(msg, flags);
        }
        
        bool send(unsigned char value, int flags = 0)
        {
            zmq::message_t msg(sizeof(unsigned char));
            memcpy(msg.data(), &value, sizeof(unsigned char));
            return socket_t::send(msg, flags);
        }
        
        bool send(unsigned char value[], int flags =0)
        {
            zmq::message_t msg(sizeof(value));
            memcpy(msg.data(), &value, sizeof(value));
            
            return socket_t::send(msg, flags);
        }
        
        
        /*
         n: expected number of frames, including separators
         */
        Frames blockingRecv(int n, bool checked =true)
        {
            Frames frames;
            frames.reserve(n);
            
            do {
                zmq::message_t message;
                if(!socket_t::recv(&message, 0))
                    throw error_t();
                
                const char* base = static_cast<const char*>(message.data());
                frames.push_back(std::string(base, base + message.size()));
            } while(sockopt_rcvmore());
            
            if(checked && frames.size() != n)
                throw error_t();
            
            return frames;
        }
        
        std::string recvAsString(int flags =0)
        {
            zmq::message_t message;
            if(!socket_t::recv(&message, flags))
                throw error_t();
            
            const char* base = static_cast<const char*>(message.data());
            return std::string(base, base + message.size());
        }
        
        int recvAsInt(int flags =0)
        {
            zmq::message_t message;
            if(!socket_t::recv(&message, flags))
                throw error_t();
            
            return *(static_cast<int*>(message.data()));
        }
        
        short recvAsShort(int flags = 0)
        {
            zmq::message_t message;
            if (!socket_t::recv(&message, flags))
                throw error_t();
            
            return *(static_cast<short*>(message.data()));
        }
        unsigned short recvAsUnsignedShort(int flags = 0)
        {
            zmq::message_t message;
            if (!socket_t::recv(&message, flags))
                throw error_t();
            
            return *(static_cast<unsigned short*>(message.data()));
        }
        
        char recvAsChar(int flags = 0)
        {
            zmq::message_t message;
            if (!socket_t::recv(&message, flags))
                throw error_t();
            
            return *(static_cast<char*>(message.data()));
        }
        
        unsigned char recvAsUnsignedChar(int flags = 0)
        {
            zmq::message_t message;
            if (!socket_t::recv(&message, flags))
                throw error_t();
            
            return *(static_cast<unsigned char*>(message.data()));
        }
        
        
    private:
        Socket(const Socket&);
        void operator=(const Socket&);
        
        bool sockopt_rcvmore()
        {
            int64_t rcvmore;
            size_t type_size = sizeof(int64_t);
            getsockopt(ZMQ_RCVMORE, &rcvmore, &type_size);
            return rcvmore ? true : false;
        }
        
        bool send(const char* frame, size_t len, int flags =0)
        {
            zmq::message_t msg(len);
            memcpy(msg.data(), frame, len);
            return socket_t::send(msg, flags);
        }
    };
}


#endif
