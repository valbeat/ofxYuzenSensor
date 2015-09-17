#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    #ifdef _USE_LIVE_VIDEO
        camera.listDevices();
        camera.setVerbose(true);
        camera.setDeviceID(0);
        camera.initGrabber(ofGetWidth(), ofGetHeight());
    #else
        //video.loadMovie();
        //video.play();
    #endif
    
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false); //フレーム更新時に塗り潰しを無効化
    ofBackground(0, 0, 0); //背景色を黒に設定
    
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
    gui.add(contourThresh.setup("contour finder thresh", 100, 0, 255));
    gui.add(resetBackgroundButton.setup("reset background"));
    gui.add(fullScreenToggle.setup("full screen"));
    gui.add(diffFlag.setup("diff",true));
    gui.add(contourFlag.setup("contour",true));
    gui.add(bgFlag.setup("background",false));
    gui.add(cameraFlag.setup("camera",true));
    gui.add(guiFlag.setup("gui",true));
//
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    if(camera.isFrameNew()) {
        background.setThresholdValue(bgThresh);
        background.update(camera, threshold);
        threshold.update();
        contourFinder.setThreshold(contourThresh);
        contourFinder.findContours(threshold);
    }
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    
    if (cameraFlag) {
        camera.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    if (bgFlag) {
        bgImg.draw(0, 0, ofGetWidth(),ofGetHeight());
    }
    if (diffFlag) {
        ofSetColor(255);
        threshold.draw(0,0);
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