#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false); //フレーム更新時に塗り潰しを無効化
    ofBackground(0, 0, 0); //背景色を黒に設定
    
    sender.setup(HOST, PORT);
    
    camWidth = ofGetWidth();
    camHeight = ofGetHeight();
    
    #ifdef _USE_LIVE_VIDEO
        camera.listDevices();
        camera.setVerbose(true);
        camera.setDeviceID(0);
        camera.initGrabber(camWidth,camHeight);
    #else
        //video.loadMovie();
        //video.play();
    #endif
    

    //輪郭の設定
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    
    //背景の学習を設定
    background.setLearningTime(900);
    background.setThresholdValue(20);
    isLearnBg = true;
    
    //ボタンの動作設定
    resetBackgroundButton.addListener(this, &ofApp::resetBackgroundPressed);
    
    //GUIの設定
    gui.setup();
    gui.add(bgThresh.setup("background thresh", 50, 0, 100));
    gui.add(contourThresh.setup("contour finder thresh", 500, 0, 1000));
    gui.add(resetBackgroundButton.setup("reset background"));
    gui.add(fullScreenToggle.setup("full screen"));
    gui.add(diffFlag.setup("diff image",true));
    gui.add(contourFlag.setup("contour image",true));
    gui.add(bgFlag.setup("background image",false));
    gui.add(cameraFlag.setup("camera image",true));
    gui.add(guiFlag.setup("gui",true));
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    if(camera.isFrameNew()) {
        background.setThresholdValue(bgThresh);
        background.update(camera, diffImg);
        diffImg.update();
        contourFinder.setThreshold(contourThresh);
        contourFinder.findContours(diffImg);
        float x,y,z;
        for (int i = 0; i < contourFinder.size(); i++) {
            x = contourFinder.getBoundingRect(i).x;
            y = contourFinder.getBoundingRect(i).y;
            z = contourFinder.getBoundingRect(i).area();
            z = z / 1000;
            ofxOscMessage m;
            m.setAddress( "/user" );
            m.addIntArg(i);
            m.addIntArg(x);
            m.addIntArg(y);
            m.addIntArg(z);
            sender.sendMessage(m);
            String msg_string;
            msg_string = m.getAddress();
            for (int i=0; i<m.getNumArgs(); i++ ) {
                msg_string += " ";
                if(m.getArgType(i) == OFXOSC_TYPE_INT32)
                    msg_string += ofToString( m.getArgAsInt32(i));
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT)
                    msg_string += ofToString( m.getArgAsFloat(i));
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING)
                    msg_string += m.getArgAsString(i);
            }
            cout << msg_string << endl;
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    
    if (cameraFlag) {
        camera.draw(0, 0, camWidth, camHeight);
    }
    if (bgFlag) {
        bgImg.draw(0, 0, camWidth, camHeight);
    }
    if (diffFlag) {
        ofSetColor(255);
        diffImg.draw(0,0);
    }
    if (contourFlag) {
        ofSetColor(0, 255, 255);
        contourFinder.draw();
    }
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            background.reset();
            break;
        case 'g':
            (guiFlag == false) ? guiFlag = true : guiFlag = false;
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
void ofApp::windowResized(int w, int h){
    camWidth = ofGetWidth();
    camHeight = ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void ofApp::resetBackgroundPressed() {
    background.reset();
}
//--------------------------------------------------------------
void ofApp::toggleFullScreenPressed() {
    ofToggleFullscreen();
}