#pragma once

#include "ofMain.h"
#include "ofxTwinkly.h"
#include "ofxTwinklyDiscovery.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxTwinklyDiscovery discoverTwinkly;
    std::vector<ofxTwinkly> strips;
    
    std::vector<std::vector<int>> frames;
    void createFrame();
    
    
//    std::vector<ofParameter<ofColor>> colorForegrounds;
//    std::vector<ofParameter<ofColor>> colorBackgrounds;
//    std::vector<ofParameter<int>> fadeWidths;
//    std::vector<ofParameter<int>> rampWidths;
//    std::vector<ofParameter<int>> holds;
//    std::vector<ofParameter<float>> speeds;
//    std::vector<ofParameter<float>> masterFaders;
    std::vector<ofParameterGroup> stripsParameterGroups;
    ofxPanel gui;
    
    std::vector<int> startGradients;
    std::vector<float> masterFaders;

    
    void makeGradientFrame(int strip, int totalLeds);
    void makeDrawGradientRoutine(int strip);
    std::vector<ofMesh> gradientPaths;
    std::vector<float> speedFloat;
    int minNumLEDs;
    int discoveryTimer;
    
    
};
