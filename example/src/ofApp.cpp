#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_SILENT);
    //we can find the lights and get thier ip addresses
    discoverTwinkly.setup();
    discoverTwinkly.discover();
    discoveryTimer = ofGetElapsedTimeMillis();
    
    minNumLEDs =  175;
    masterFader = 1.0;
    // lets give the devices 5 seconds to respond
    while(ofGetElapsedTimeMillis() - discoveryTimer < 50){
        discoverTwinkly.update();
    }
    minNumLEDs = 175;
    if (discoverTwinkly.getIPAddressList().size() > 0) {
        for (int i = 0; i < discoverTwinkly.getIPAddressList().size(); i++) {
            cout<< "Found device at " + discoverTwinkly.getIPAddressList()[i] << endl;
            // Add a strip for each reply we got
            ofxTwinkly strip;
            strips.push_back(strip);
            strips[i].setup(discoverTwinkly.getIPAddressList()[i]);
            strips[i].setMode("rt");
            ofSetFrameRate(strips[0]._frame_rate);
            if (strips[i]._number_of_leds < minNumLEDs) {
                minNumLEDs = strips[i]._number_of_leds;
            }
           
        }
    }
    else{
        cout<< "No twinkly devices found" << endl;
        
    }
    gui.setup("Strip control", "stripSettings.xml");
    gui.add(masterFader.set("Master Fader", 1.0, 0.0, 1.0));

    for(int i = 0; i < strips.size(); i++){
        
        ofParameterGroup group;
        //+ strips[i]._deviceHardwareID
        group.setName("strip " + ofToString(i+1) + " "  );
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
        group.add(speed.set("Gradient speed",1.0,0.0001,20.0));
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
        float speedys;
        speedFloat.push_back(speedys);
    }
    gui.loadFromFile("stripSettings_1.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < strips.size(); i++) {
         strips[i].update();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0, 0, 0);
    
    for (int i = 0; i < strips.size(); i++) {
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
    
    if(key == OF_KEY_LEFT){
        for(int i = 0; i < strips.size(); i++){
            stripsParameterGroups[i].get("Gradient speed").cast<float>() -= 0.05;
            if (stripsParameterGroups[i].get("Gradient speed").cast<float>()<0.01) {
                stripsParameterGroups[i].get("Gradient speed").cast<float>() = 0.01;
            }
        }
    }
    if (key == OF_KEY_RIGHT) {
        for(int i = 0; i < strips.size(); i++){
            
            stripsParameterGroups[i].get("Gradient speed").cast<float>() += 0.05;
            if (stripsParameterGroups[i].get("Gradient speed").cast<float>() > 20.0) {
                stripsParameterGroups[i].get("Gradient speed").cast<float>() = 10.0;
            }
        }
    }
    if (key == OF_KEY_UP) {
        masterFader += 0.01;
        if(masterFader > 1.0){
            masterFader = 1.0;
        }
    }
    if (key == OF_KEY_DOWN) {
        
       masterFader -= 0.01;
        if(masterFader < 0.0){
            masterFader = 0.0;
        }
    }
    
    if(key == '!'){
        gui.saveToFile("stripSettings_1.xml");
        //gui.setup("Strip control", "stripSettings_1.xml");

    }
    if(key == '@'){
        gui.saveToFile("stripSettings_2.xml");
    }
    if(key == '#'){
        gui.saveToFile("stripSettings_3.xml");
    }
    if(key == '$'){
        gui.saveToFile("stripSettings_4.xml");
    }
    if(key == '%'){
        gui.saveToFile("stripSettings_5.xml");
    }
    
    if(key == '1'){
        gui.loadFromFile("stripSettings_1.xml");
    }
    if(key == '2'){
        gui.loadFromFile("stripSettings_2.xml");
    }
    if(key == '3'){
        gui.loadFromFile("stripSettings_3.xml");
    }
    if(key == '4'){
        gui.loadFromFile("stripSettings_4.xml");
    }
    if(key == '5'){
        gui.loadFromFile("stripSettings_5.xml");
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
            // before the gradient start position
            ofColor fadedColour = stripsParameterGroups[strip].get("Background colour").cast<ofColor>();
            fadedColour.a = fadedColour.a * masterFader;
            frames[strip].push_back(fadedColour.r);
            frames[strip].push_back(fadedColour.g);
            frames[strip].push_back(fadedColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadedColour);
        }
        else if (i >= startGradients[strip] && i < startGradients[strip] + stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  && i < totalLeds) {
           ofColor fadedColour = stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>().get();
            ofColor backgroundFaded = stripsParameterGroups[strip].get("Background colour").cast<ofColor>().get();
            fadedColour.a = fadedColour.a * masterFader;
            backgroundFaded.a = backgroundFaded.a * masterFader;
            
            fadedColour = fadedColour.getLerped(backgroundFaded,  1 -((float(i)-(float(startGradients[strip])))/float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ))) ;
            frames[strip].push_back(fadedColour.r);
            frames[strip].push_back(fadedColour.g);
            frames[strip].push_back(fadedColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadedColour);
            ofLog(OF_LOG_VERBOSE)<<"gradient ramp up " + ofToString(((float(i)-(float(startGradients[strip])))/float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ))) + " " + ofToString(fadedColour) <<endl;
            
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + startGradients[strip] && i < stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] && i < totalLeds) {
            //gradient hold
            ofColor fadedColour = stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>();
            fadedColour.a = fadedColour.a * masterFader;
            frames[strip].push_back(fadedColour.r);
            frames[strip].push_back(fadedColour.g);
            frames[strip].push_back(fadedColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadedColour);
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  + stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] && i < stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] + (stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  * 2) && i < totalLeds) {
            //gradient ramp down
            ofColor fadedColour = stripsParameterGroups[strip].get("Pulse colour").cast<ofColor>().get();
            ofColor backgroundFaded = stripsParameterGroups[strip].get("Background colour").cast<ofColor>().get();
            fadedColour.a = fadedColour.a * masterFader;
            backgroundFaded.a = backgroundFaded.a * masterFader;

            fadedColour = fadedColour.getLerped(backgroundFaded, (((float(i) - (float(startGradients[strip]) + float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + float(stripsParameterGroups[strip].get("Gradient hold").cast<int>())))) / stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ));
            frames[strip].push_back(fadedColour.r);
            frames[strip].push_back(fadedColour.g);
            frames[strip].push_back(fadedColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadedColour);
            ofLog(OF_LOG_VERBOSE)<<"gradient ramp down " + ofToString((stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  - (float(i) - (float(startGradients[strip]) + float(stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + float(stripsParameterGroups[strip].get("Gradient hold").cast<int>())))) / stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>() ) + " " + ofToString(fadedColour)<<endl;
            
        }
        else if (i >= stripsParameterGroups[strip].get("Gradient hold").cast<int>() + startGradients[strip] + (stripsParameterGroups[strip].get("Gradient ramp Width").cast<int>()  * 2) && i < totalLeds) {
            //after gradient
            ofColor fadedColour = stripsParameterGroups[strip].get("Background colour").cast<ofColor>();
            fadedColour.a = fadedColour.a * masterFader;
            frames[strip].push_back(fadedColour.r);
            frames[strip].push_back(fadedColour.g);
            frames[strip].push_back(fadedColour.b);
            gradientPaths[strip].addVertex({0, i*2 , 0});
            gradientPaths[strip].addIndex(i);
            gradientPaths[strip].addColor(fadedColour);
        }
    }
    
}
