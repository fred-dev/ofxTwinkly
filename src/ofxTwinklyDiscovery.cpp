#include "ofxTwinklyDiscovery.h"

ofxTwinklyDiscovery::ofxTwinklyDiscovery(){
    
}

ofxTwinklyDiscovery::~ofxTwinklyDiscovery(){
    
}


void ofxTwinklyDiscovery::setup(){
    ofxUDPSettings udpDiscoverSettngs;
    udpDiscoverSettngs.sendTo("255.255.255.255", 5555);
    udpDiscoverSettngs.blocking = false;
    udpDiscoverSettngs.reuse = true;
    udpDiscoverSettngs.broadcast = true;
    discoveryUDPConnection.Setup(udpDiscoverSettngs);
    
    udpDiscoveryMessage.clear();
    udpDiscoveryMessage.push_back(0x01);
    udpDiscoveryMessage.push_back('d');
    udpDiscoveryMessage.push_back('i');
    udpDiscoveryMessage.push_back('s');
    udpDiscoveryMessage.push_back('c');
    udpDiscoveryMessage.push_back('o');
    udpDiscoveryMessage.push_back('v');
    udpDiscoveryMessage.push_back('e');
    udpDiscoveryMessage.push_back('r');
    
    
    for (int i = 0; i < udpDiscoveryMessage.size(); i++) {
        sendDiscover+=udpDiscoveryMessage[i];
    }
    ofLogNotice("ofxTwinklyDiscovery::setup") << "Broadcasting discovery message";
}

void ofxTwinklyDiscovery::update(){
    
    char udpMessage[10000];
    discoveryUDPConnection.Receive(udpMessage,10000);
    string message;
    message = udpMessage;
    if(message!=""){
        ofLogNotice("ofxTwinklyDiscovery::update") << "total incoming message: " + message;
        string ipAddress;
        int ipParts[4];
        memcpy (&ipParts[3], &udpMessage[0], 1 );
        memcpy (&ipParts[2], &udpMessage[1], 1 );
        memcpy (&ipParts[1], &udpMessage[2], 1 );
        memcpy (&ipParts[0], &udpMessage[3], 1 );
        ipAddress = ofToString(ipParts[0]) + "." + ofToString(ipParts[1]) + "." + ofToString(ipParts[2]) + "." + ofToString(ipParts[3]);
        bool exists = false;
        
        for (int i = 0; i < discoveredIPAddresses.size(); i++) {
            if (discoveredIPAddresses[i] == ipAddress) {
                exists = true;
                ofLogNotice("ofxTwinklyDiscovery::update") << "Twinkly device already logged at address: " + ipAddress;
            }
        }
        if (!exists) {
            discoveredIPAddresses.push_back(ipAddress);
            discoveredDeviceCount ++;
            ofLogNotice("ofxTwinklyDiscovery::update") << "Twinkly device found at address: " + ipAddress;
        }
    }
}
std::vector<string> ofxTwinklyDiscovery::getIPAddressList(){
    return discoveredIPAddresses;
}
void ofxTwinklyDiscovery::discover(){
    discoveryUDPConnection.Send(sendDiscover.c_str(), sendDiscover.size());
}

