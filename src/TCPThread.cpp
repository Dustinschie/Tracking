//
//  TCPThread.cpp
//  Tracking
//
//  Created by Dustin Schie on 2/14/14.
//
//

#include "TCPThread.h"

TCPThread::TCPThread(){
    server.setup(11235);
    messageRecvPort = 12345;
    messageReciever.setup(messageRecvPort);
    
    
}
//--------------------------------------------------------------
void TCPThread::start(){
    startThread(true, false);
}
//--------------------------------------------------------------
void TCPThread::stop(){
    stopThread();
}
//--------------------------------------------------------------
void TCPThread::threadedFunction(){
    stringstream ss;
    while (isThreadRunning() != 0) {
        if (lock()) {
            while (messageReciever.hasWaitingMessages()) {
                ofxOscMessage m;
                messageReciever.getNextMessage(&m);
                if (m.getAddress() == "/botInfo" && m.getNumArgs() > 0) {
                    if (m.getArgType(0) == OFXOSC_TYPE_INT32 && m.getArgType(1) == OFXOSC_TYPE_STRING) {
                        int key = m.getArgAsInt32(0);
                        string val = m.getArgAsString(1);
                        info.insert(pair<int, string>(key, val));
                    }
                }
                else if (m.getAddress() == "/clear"){
                    botInfo.clear();
                    break;
                }
            }
            for (int i = 0; i < server.getLastID(); i++) {
                if (server.isClientConnected(i)) {
                    server.send(i, botInfo.str());
                    //  read from client
                    string s = server.receive(i);
                }
            }
            unlock();
            ofSleepMillis(1000);
        }
    }
}
//--------------------------------------------------------------
string TCPThread::getOscMsgAsString(ofxOscMessage m){
    string msg_string;
	msg_string = m.getAddress();
	msg_string += ":";
	for(int i = 0; i < m.getNumArgs(); i++){
		// get the argument type
		msg_string += " " + m.getArgTypeName(i);
		msg_string += ":";
		// display the argument - make sure we get the right type
		if(m.getArgType(i) == OFXOSC_TYPE_INT32){
			msg_string += ofToString(m.getArgAsInt32(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
			msg_string += ofToString(m.getArgAsFloat(i));
		}
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
			msg_string += m.getArgAsString(i);
		}
		else{
			msg_string += "unknown";
		}
	}
	return msg_string;
}