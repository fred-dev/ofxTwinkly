#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxNetwork.h"


class ofxTwinkly
{

public:

	ofxTwinkly();
	~ofxTwinkly();
    void setup(string ipAddress);
    bool connect();
    bool verify();
    bool getDetails();
    bool setMode(string mode);
    void addFrame(std::vector<int> frame);
    int sendUpdFrame();
    void update();
    std::string _authentication_token;
    std::string  _challenge_response;
    std::string _token;
    std::string _deviceName;
    std::string _deviceHardwareID;
    int _number_of_leds;
    int _frame_rate;
    std::string _led_version;
    std::string _product_code;
    std::string _ipAddress;
    bool twinklyGetDetails();
    vector<unsigned char> udpMessage;
    std::string sendString;
    bool realtimeMode;
    
    ofxUDPManager udpConnection;

    
private:


};
