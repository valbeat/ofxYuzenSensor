#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    #ifdef _USE_LIVE_VIDEO
        camera.listDevices();
        camera.setVerbose(true);
        camera.setDeviceID(0);
        camera.initGrabber(camWidth, camHeight);
    #else
        //video.loadMovie();
        //video.play();
    #endif
    camera.initGrabber(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    camera.draw(0,0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
