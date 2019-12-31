#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"


class ofxTwinklyDiscovery
{

public:

	ofxTwinklyDiscovery();
	~ofxTwinklyDiscovery();
    
    ofxUDPManager discoveryUDPConnection;
    vector<unsigned char> udpDiscoveryMessage;
	
    void discover();
    string sendDiscover;
    void setup();
    void update();
    std::vector<string> discoveredIPAddresses;
    std::vector<string> getIPAddressList();
    int discoveredDeviceCount;
    
private:


};
