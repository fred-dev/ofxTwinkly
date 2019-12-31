#include "ofxTwinkly.h"

ofxTwinkly::ofxTwinkly(){
    
}

ofxTwinkly::~ofxTwinkly(){
    
}


void ofxTwinkly::setup(std::string ipAddress){
    _ipAddress = ipAddress;
    ofxUDPSettings settings;
    settings.sendTo(ipAddress, 7777);
    settings.blocking = false;
    udpConnection.Setup(settings);
    connect();
    verify();
    getDetails();
}

bool ofxTwinkly::connect(){
    bool returnStatus;
    std::string uri = "http://"+  _ipAddress + "/xled/v1/login/";
    ofJson json;
    json["challenge"] = "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8=";
    // Create a client.
    ofxHTTP::Client client;
    ofxHTTP::JSONRequest request(uri);
    request.setJSON(json);
    
    try
    {
        auto response = client.execute(request);
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            ofLogNotice("ofxTwinkly::connect") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            ofJson responseJson = response->json();
            ofLogVerbose("ofxTwinkly::connect") << "Content Begin";
            ofLogVerbose("ofxTwinkly::connect") << responseJson.dump(4);
            ofLogVerbose("ofxTwinkly::connect") << "Content End";
            
            _challenge_response = responseJson["challenge-response"];
            ofLogVerbose("ofxTwinkly::connect") << "_challenge_response = " + _challenge_response;
            _token = responseJson["authentication_token"];
            ofLogVerbose("ofxTwinkly::connect") << "authentication_token = " + _token;
            returnStatus = true;
        }
        else
        {
            ofLogError("ofxTwinkly::connect") << response->getStatus() << " " << response->getReason();
            ofLogError("ofxTwinkly::connect") << "Connection Failed";
            returnStatus = false;
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofxTwinkly::connect") << exc.displayText();
        ofLogVerbose("ofxTwinkly::connect") << "Connection Failed";
        returnStatus = false;
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofxTwinkly::connect") << exc.what();
        ofLogVerbose("ofxTwinkly::connect") << "Connection Failed";
        returnStatus = false;
    }
    return returnStatus;
}
bool ofxTwinkly::verify(){
    bool returnStatus;
    std::string uri = "http://"+  _ipAddress + "/xled/v1/verify";
    ofJson json;
    json["challenge-response"] = _challenge_response;
    ofxHTTP::Client client;
    ofxHTTP::JSONRequest request(uri);
    request.setJSON(json);
    request.add("X-Auth-Token" ,_token);
    try
    {
        auto response = client.execute(request);
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            ofLogNotice("ofxTwinkly::verify") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            ofJson responseJson = response->json();
            ofLogVerbose("ofxTwinkly::verify") << "Content Begin";
            ofLogVerbose("ofxTwinkly::verify")  << responseJson.dump(4);
            ofLogVerbose("ofxTwinkly::verify") << "Content End";
            ofLogVerbose("ofxTwinkly::verify") << "Verify successful";

        }
        else
        {
            ofLogError("ofxTwinkly::verify") << response->getStatus() << " " << response->getReason();
            ofLogVerbose("ofxTwinkly::verify") << "verify Failed";
            returnStatus = false;
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofxTwinkly::verify") << exc.displayText();
        ofLogVerbose("ofxTwinkly::verify") << "verify Failed";
        returnStatus = false;
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofxTwinkly::verify") << exc.what();
        ofLogVerbose("ofxTwinkly::verify") << "verify Failed";
        returnStatus = false;
    }
    
    return returnStatus;
}
bool ofxTwinkly::getDetails(){
    bool returnStatus;
    std::string uri = "http://"+  _ipAddress + "/xled/v1/gestalt";
    
    ofJson json;
    ofxHTTP::Client client;
    ofxHTTP::JSONRequest request(uri);
    request.setJSON(json);
    request.add("X-Auth-Token" ,_token);
    
    try
    {
        auto response = client.execute(request);
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            ofLogVerbose("ofxTwinkly::getDetails") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            ofJson responseJson = response->json();
            ofLogVerbose("ofxTwinkly::getDetails") << "Content Begin";
            
            _deviceName =responseJson["device_name"];
            ofLogVerbose("ofxTwinkly::getDetails") << "device name " << _deviceName;
            
            _number_of_leds =responseJson["number_of_led"];
            ofLogVerbose("ofxTwinkly::getDetails") << "Number of Leds " << ofToString(_number_of_leds);
            
            _frame_rate =responseJson["frame_rate"];
            ofLogVerbose("ofxTwinkly::getDetails") << "Frame Rate " << ofToString(_frame_rate);
            
            _led_version = responseJson["led_version"];
            ofLogVerbose("ofxTwinkly::getDetails") << "Led Version " << ofToString(_led_version);
            
            _product_code= responseJson["product_code"];
            ofLogVerbose("ofxTwinkly::getDetails") << "Product code " << ofToString(_product_code);
            
            _deviceHardwareID = responseJson["hw_id"];
            ofLogVerbose("ofxTwinkly::getDetails") << "hardware ID " << ofToString(_deviceHardwareID);
            ofLogVerbose("ofxTwinkly::getDetails") << responseJson.dump(4);
            ofLogVerbose("ofxTwinkly::getDetails") << "Content End";
            ofLogVerbose("ofxTwinkly::getDetails") << "get Details succesful";
            returnStatus = true;
        }
        else
        {
            ofLogError("ofxTwinkly::getDetails") << response->getStatus() << " " << response->getReason();
            ofLogVerbose("ofxTwinkly::verify") << "get Details Failed";
            returnStatus = false;
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofxTwinkly::getDetails") << exc.displayText();
        ofLogVerbose("ofxTwinkly::verify") << "get Details Failed";
        returnStatus = false;
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofxTwinkly::getDetails") << exc.what();
        ofLogVerbose("ofxTwinkly::verify") << "Verify Failed";
        returnStatus = false;
    }
    return returnStatus;
}
bool ofxTwinkly::setMode(string mode){
    bool returnStatus;
    std::string uri = "http://"+  _ipAddress + "/xled/v1/led/mode";
    
    ofJson json;
    json["mode"] = mode;
    ofxHTTP::Client client;
    ofxHTTP::JSONRequest request(uri);
    request.setJSON(json);
    request.add("X-Auth-Token" ,_token);

    try
    {
        auto response = client.execute(request);
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            ofLogVerbose("ofxTwinkly::setMode") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            ofJson responseJson = response->json();
            ofLogVerbose("ofxTwinkly::setMode") << "Content Begin";
            ofLogVerbose("ofxTwinkly::setMode") << responseJson.dump(4);
            ofLogVerbose("ofxTwinkly::setMode") << "Content End";
            
            if(mode == "rt"){
                realtimeMode = true;
            }
            else{
                realtimeMode = false;
            }
            ofLogVerbose("ofxTwinkly::setMode") << "Set Mode success, mode is: " + mode;
            returnStatus = true;
        }
        else
        {
            ofLogError("ofxTwinkly::setMode") << response->getStatus() << " " << response->getReason();
            ofLogVerbose("ofxTwinkly::setMode") << "Set Mode unsuccessful";
            returnStatus = false;
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofxTwinkly::setMode") << exc.displayText();
        ofLogVerbose("ofxTwinkly::setMode") << "Set Mode unsuccessful";
        returnStatus = false;
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofxTwinkly::setMode") << exc.what();
        ofLogVerbose("ofxTwinkly::setMode") << "Set Mode unsuccessful";
        returnStatus = false;
    }
    return returnStatus;
}
void ofxTwinkly::addFrame(std::vector<int> frame){
    if(realtimeMode){
        udpMessage.clear();
        sendString.clear();
        udpMessage.push_back(0x01);
        
        ofxIO::ByteBuffer encoded(_token);
        ofxIO::ByteBuffer decoded;
        ofxIO::Base64Encoding base64;
        std::size_t size = base64.decode(encoded, decoded);
        for (std::size_t i = 0; i < decoded.size(); i++) {
            udpMessage.push_back(decoded[i]);
        }
        
        udpMessage.push_back(_number_of_leds);

        for (int i = 0; i < _number_of_leds * 3; i++) {
            unsigned char led = frame[i];
            udpMessage.push_back(led);
        }
        
        for (int i = 0; i < udpMessage.size(); i++) {
            sendString+=udpMessage[i];
        }
        ofLogVerbose("ofxTwinkly::createUDPFrame")  << "UDP message Size = " + ofToString(sendString.size()) + " with content: " + ofToString(sendString);
    }
    
}
int ofxTwinkly::sendUpdFrame(){
    return udpConnection.Send(sendString.c_str(),sendString.size());
    ofLogVerbose("ofxTwinkly::sendUpdFrame")  << "UDP frame message sent";

    
}
void ofxTwinkly::update(){
    if(realtimeMode){
        sendUpdFrame();
    }
}




