#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_SILENT);
    //we can find the lights and get thier ip addresses
    discoverTwinkly.setup();
    discoverTwinkly.discover();
    discoveryTimer = ofGetElapsedTimeMillis();
    
    minNumLEDs =  175;
    // lets give the devices 5 seconds to respond
    while(ofGetElapsedTimeMillis() - discoveryTimer < 500){
        discoverTwinkly.update();
    }
    
    if (discoverTwinkly.getIPAddressList().size() > 0) {
        for (int i = 0; i < discoverTwinkly.getIPAddressList().size(); i++) {
            cout<< "Found device at " + discoverTwinkly.getIPAddressList()[i] << endl;
            // Add a strip for each reply we got
            ofxTwinkly strip;
            strips.push_back(strip);
            strips[i].setup(discoverTwinkly.getIPAddressList()[i]);
            strips[i].setMode("rt");
            ofSetFrameRate(strips[0]._frame_rate);
            minNumLEDs = strips[0]._number_of_leds;
        }
    }
    else{
        cout<< "No twinkly devices found" << endl;
        
    }
    gui.setup("Strip control", "stripSettings.xml");
    
    for(int i = 0; i < STRIP_SIZE; i++){
        
        ofParameterGroup group;
        //+ strips[i]._deviceHardwareID
        group.setName("strip " + ofToString(i+1) + " " );
        ofParameter<ofColor> colorForeground;
        ofParameter<ofColor> colorBackground;
        ofParameter<int> fadeWidth;
        ofParameter<int> rampWidth;
        ofParameter<int> hold;
        ofParameter<float> speed;
        
        group.add(colorForeground.set("Pulse colour",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
        group.add(colorBackground.set("Background colour",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
        group.add(fadeWidth.set("Gradient fade Width",10,1,50));
        group.add(rampWidth.set("Gradient ramp Width",3,1,50));
        group.add(hold.set("Gradient hold",10,1,175));
        group.add(speed.set("Gradient speed",1,0.0001,10));
        stripsParameterGroups.push_back(group);
        gui.add(stripsParameterGroups[i]);
        
        int startGradient = 0;
        startGradients.push_back(startGradient);
        int masterFader;
        startGradients.push_back(masterFader);
        ofMesh gradientPath;
        gradientPaths.push_back(gradientPath);
        
        std::vector<int> frame;
        frames.push_back(frame);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < STRIP_SIZE; i++) {
        // strips[i].update();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0, 0, 0);
    
    for (int i = 0; i < STRIP_SIZE; i++) {
        makeDrawGradientRoutine(i);
        //strips[i].addFrame(frames[i]);
    }
    gui.draw();
    
}

void ofApp::makeDrawGradientRoutine(int strip){
   

    if(stripsParameterGroups[strip].get("Gradient speed").cast<float>() >= 1){
        startGradients[strip] += 1*stripsParameterGroups[strip].get("Gradient speed").cast<float>() ;
    }
    else{
        speedFloat[strip] += stripsParameterGroups[strip].get("Gradient speed").cast<float>() ;
        if (speedFloat[strip] > 1) {
            startGradients[strip] += 1;
            speedFloat[strip] = 0;
        }
    }
    //cout << "here "<< endl;
    if (startGradients[strip]>minNumLEDs) {
        startGradients[strip] = 0-stripsParameterGroups[strip].get("Gradient fade Width").cast<int>();
    }
    
    makeGradientFrame(strip, minNumLEDs);
    
    ofPushView();
    ofPushStyle();
    ofSetLineWidth(10.0);
    ofTranslate(240 + strip * 30, 10);
    gradientPaths[strip].draw();
    ofPopStyle();
    ofPopView();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1') {
        for (int i = 0; i < STRIP_SIZE; i++) {
            strips[i].setMode("demo");
        }
    }
    
    if (key == '2') {
        for (int i = 0; i < STRIP_SIZE; i++) {
            strips[i].setMode("movie");
        }
    }
    
    if (key == '3') {
        for (int i = 0; i < STRIP_SIZE; i++) {
            strips[i].setMode("off");
        }
    }
    
    if (key == '4') {
        for (int i = 0; i < STRIP_SIZE; i++) {
            strips[i].setMode("rt");
        }
    }
    
    if(key == OF_KEY_LEFT){
        for(int i = 0; i < STRIP_SIZE; i++){
            stripsParameterGroups[i].get("Gradient speed").cast<float>() -= 0.05;
            if (stripsParameterGroups[i].get("Gradient speed").cast<float>()<0.01) {
                stripsParameterGroups[i].get("Gradient speed").cast<float>() = 0.01;
            }
        }
        
    }
    if (key == OF_KEY_RIGHT) {
        for(int i = 0; i < STRIP_SIZE; i++){
            
            stripsParameterGroups[i].get("Gradient speed").cast<float>() += 0.05;
            if (stripsParameterGroups[i].get("Gradient speed").cast<float>() > 10.0) {
                stripsParameterGroups[i].get("Gradient speed").cast<float>() = 10.0;
            }
        }
    }
    if (key == OF_KEY_UP) {
        
        
        //        if (colorForeground.get().getBrightness() < 255) {
        //             ofColor tempColour1;
        //                   tempColour1.setHsb(colorForeground.get().getHue(), colorForeground.get().getSaturation(), colorForeground.get().getBrightness()+1);
        //                   colorForeground = tempColour1;
        //
        //        }
        //
        //        if ( colorBackground.get().getBrightness() < 255) {
        //
        //            ofColor tempColour2;
        //                   tempColour2.setHsb(colorBackground.get().getHue(), colorBackground.get().getSaturation(), colorBackground.get().getBrightness()+1);
        //                   colorBackground = tempColour2;
        //        }
        
    }
    if (key == OF_KEY_DOWN) {
        
        //        if (colorForeground.get().getBrightness() > 0) {
        //             ofColor tempColour1;
        //                   tempColour1.setHsb(colorForeground.get().getHue(), colorForeground.get().getSaturation(), colorForeground.get().getBrightness()-1);
        //                   colorForeground = tempColour1;
        //            }
        //        if ( colorBackground.get().getBrightness() > 0) {
        //         ofColor tempColour2;
        //         tempColour2.setHsb(colorBackground.get().getHue(), colorBackground.get().getSaturation(), colorBackground.get().getBrightness()-1);
        //         colorBackground = tempColour2;
        //        }
        
        
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


void ofApp::makeGradientFrame(int strip, int totalLeds){
    stripsParameterGroups[strip].get("Gradient fade Width").cast<int>() = stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  * 2 + stripsParameterGroups[strip].get("Gradient hold").cast<int>();
    frames[strip].clear();
    
    gradientPaths[strip].clear();
    gradientPaths[strip].setMode(OF_PRIMITIVE_LINES);
    for(int i = 0; i< totalLeds; i++){
        if (i < startGradients[strip]) {
            // cout<<"Before the gradient start position"<<endl;
            // before the gradient start position
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->r);
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->g);
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(stripsParameterGroups[strip].get("Background colour").cast<ofColor>().get());
        }
        else if (i >= startGradients[strip] && i < startGradients[strip] + stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  && i < totalLeds) {
            //gradient fade up time
            ofColor fadeColour = stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>().get();
            fadeColour = fadeColour.getLerped(stripsParameterGroups[strip].get("Background colour").cast<ofColor>(),  1 -((float(i)-(float(startGradients[strip])))/float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ))) ;
            frames[strip].push_back(fadeColour.r);
            frames[strip].push_back(fadeColour.g);
            frames[strip].push_back(fadeColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadeColour);
            ofLog(OF_LOG_VERBOSE)<<"gradient ramp up " + ofToString(((float(i)-(float(startGradients[strip])))/float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ))) + " " + ofToString(fadeColour) <<endl;
            
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + startGradients[strip] && i < stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] && i < totalLeds) {
            //gradient hold
            frames[strip].push_back(stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>()->r);
            frames[strip].push_back(stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>()->g);
            frames[strip].push_back(stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>()->b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>().get());
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] && i < stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] + (stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  * 2) && i < totalLeds) {
            //gradient ramp down
            ofColor fadeColour = stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>().get();
            fadeColour = fadeColour.getLerped(stripsParameterGroups[strip].get("Background colour").cast<ofColor>(), (((float(i) - (float(startGradients[strip]) + float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + float(stripsParameterGroups[strip].get("Gradient hold").cast<int>())))) / stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ));
            frames[strip].push_back(fadeColour.r);
            frames[strip].push_back(fadeColour.g);
            frames[strip].push_back(fadeColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadeColour);
            ofLog(OF_LOG_VERBOSE)<<"gradient ramp down " + ofToString((stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  - (float(i) - (float(startGradients[strip]) + float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + float(stripsParameterGroups[strip].get("Gradient hold").cast<int>())))) / stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + " " + ofToString(fadeColour)<<endl;
            
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] + (stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  * 2) && i < totalLeds) {
            //after gradient
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->r);
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->g);
            frames[strip].push_back(stripsParameterGroups[strip].get("Background colour").cast<ofColor>()->b);
            gradientPaths[strip].addVertex({0, i*2, 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(stripsParameterGroups[strip].get("Background colour").cast<ofColor>().get());
        }
    }
    
}
