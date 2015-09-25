#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false); //フレーム更新時に塗り潰しを無効化
    ofBackground(0, 0, 0); //背景色を黒に設定
    
    sender.setup(HOST, PORT);
    
    ofToggleFullscreen();
    
    camWidth = ofGetWidth();
    camHeight = ofGetHeight();
    
    camera.listDevices();
    camera.setVerbose(true);
    camera.setDeviceID(0);
    camera.initGrabber(camWidth,camHeight);
    
    video.loadMovie("cv_incam.mov");
    video.play();
    
    fbo.allocate(camWidth, camHeight, GL_RGB);
    
    
    //背景の学習を設定
    background.setLearningTime(900);
    background.setThresholdValue(20);

    
    //ボタンの動作設定
    resetBackgroundButton.addListener(this, &ofApp::resetBackgroundPressed);
    
    //GUIの設定
    gui.setup();
    gui.add(*new ofParameter<String>(""));
    gui.add(medianScale.setup("median Blur Size", 7, 0, 50));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(learnBgFlag.setup("Learn BG", true));
    gui.add(bgThresh.setup("background thresh", 80, 0, 250));
    gui.add(resetBackgroundButton.setup("reset background"));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(contourThresh.setup("contour finder thresh", 10, 0, 250));
    gui.add(minRad.setup("Min Rad", 10, 0, 100));
    gui.add(maxRad.setup("Max Rad", 100, 0, 1000));
    gui.add(minArea.setup("Min Area", 0.01, 0, 0.1));
    gui.add(maxArea.setup("Max Area", 1, 0, 1));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(useFarneback.setup("use Farnback",true));
    gui.add(pyrScale.setup("pyrScale", 0.5, 0, 1));
    gui.add(levels.setup("levels", 4, 1, 8));
    gui.add(winSize.setup("winsize", 8, 4, 64));
    gui.add(iterations.setup("iterations", 2, 1, 8));
    gui.add(polyN.setup("polyN", 7, 5, 10));
    gui.add(polySigma.setup("polySigma", 1.5, 1.1, 2));
    gui.add(OPTFLOW_FARNEBACK_GAUSSIAN.setup("OPTFLOW_FARNEBACK_GAUSSIAN",false));
    gui.add(flowScale.setup("flowScale", 0.05, 0.0, 1.0));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(qualityLevel.setup("quality level", 0.01, 0.001, 0.02));
    gui.add(maxLevel.setup("max level", 3,0,8));
    gui.add(maxFeatures.setup("max features", 200,1,1000));
    gui.add(minDistance.setup("minDistance", 4, 1, 16));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(cameraFlag.setup("camera image",true));
    gui.add(diffFlag.setup("diff image",true));
    gui.add(contourFlag.setup("contour image",true));
    gui.add(flowFlag.setup("flow image",true));
    gui.add(bgFlag.setup("background image",false));
    
    gui.add(*new ofParameter<String>(""));
    gui.add(liveVideoFlag.setup("use live video",true)); //カメラを使うかどうか
    gui.add(fullScreenToggle.setup("full screen",true));
    gui.add(guiFlag.setup("gui",true));
    
    gui.loadFromFile("settings.xml");

}

//--------------------------------------------------------------
void ofApp::update(){
    bool isNewFrame = false;
    if(liveVideoFlag) {
        camera.update();
        isNewFrame = camera.isFrameNew();
    } else {
        video.update();
        isNewFrame = video.isFrameNew();
    }
    
    if(isNewFrame) {
        if (liveVideoFlag) {
            fbo.begin();
            camera.draw(0, 0, fbo.getWidth(), fbo.getHeight());
            fbo.end();
        } else {
            fbo.begin();
            video.draw(0, 0, fbo.getWidth(), fbo.getHeight());
            fbo.end();
        }
        
        ofPixels source;
        fbo.readToPixels(source);
        ofxCv::medianBlur(source, source, medianScale); //ノイズがあるので平滑化
        if (learnBgFlag) {
            // 背景差分を取る
            background.setThresholdValue(bgThresh);
            background.update(source, diffImg);
            diffImg.update();
        } else {
            // フレーム差分を取る
            
        }
        //輪郭の設定
        contourFinder.setSortBySize(true);
        contourFinder.setThreshold(contourThresh);
        contourFinder.setMinAreaRadius(minRad);
        contourFinder.setMaxAreaRadius(maxRad);
        contourFinder.setMinArea(minArea * minArea * camWidth * camHeight);
        contourFinder.setMaxArea(maxArea * maxArea * camWidth * camHeight);
        contourFinder.findContours(diffImg);
        sendContourPosition();
        
        if(flowFlag) {
            if (useFarneback) {
                //密なオプティカルフロー
                curFlow = &farneback;
                farneback.setPyramidScale(pyrScale);
                farneback.setNumLevels(levels);
                farneback.setWindowSize(winSize);
                farneback.setNumIterations(iterations);
                farneback.setPolySigma(polySigma);
                farneback.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
                farneback.calcOpticalFlow(source);
            } else {
                //疎なオプティカルフロー
                curFlow = &pyrLk;
                pyrLk.setMaxFeatures(maxFeatures);
                pyrLk.setQualityLevel(qualityLevel);
                pyrLk.setMinDistance(minDistance);
                pyrLk.setWindowSize(winSize);
                pyrLk.setMaxLevel(maxLevel);
                pyrLk.calcOpticalFlow(source);
            }
        }

    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    
    if (cameraFlag) {
        ofSetColor(255);
        fbo.draw(0, 0, camWidth, camHeight);
    }
    if (bgFlag) {
        bgImg.draw(0, 0, camWidth, camHeight);
    }
    if (diffFlag) {
        ofSetColor(255,100);
        diffImg.draw(0, 0, camWidth, camHeight);
    }
    if (contourFlag) {
        ofSetColor(100,255,255);
        contourFinder.draw();
    }
    if (guiFlag) {
        gui.draw();
    }
    if (flowFlag) {
        ofSetColor(255);
        if (useFarneback) farneback.draw(0, 0, camWidth, camHeight);
        else pyrLk.draw(0, 0, camWidth, camHeight);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '0':
            if (!liveVideoFlag)
                video.firstFrame();
            break;
        case '5':
            if (!liveVideoFlag) {
                if (video.isPaused()) video.play();
                else video.stop();
            }
            break;
        case '6':
            if (!liveVideoFlag)
                video.nextFrame();
            break;
        case '4':
            if (!liveVideoFlag)
                video.previousFrame();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            background.reset();
            break;
        case 'g':
            (guiFlag == false) ? guiFlag = true : guiFlag = false;
//        case 's':
//            gui.saveToFile("settings.xml");
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
void ofApp::toggleLiveVideoPressed() {
    background.reset();
    if (!liveVideoFlag) video.firstFrame();
}
//--------------------------------------------------------------
void ofApp::dumpOSC(ofxOscMessage m) {
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
//--------------------------------------------------------------
void ofApp::sendContourPosition() {
    float x,y,z;
    for (int i = 0; i < contourFinder.size(); i++) {
        // 座標を重心に設定する
        cv::Rect rect = contourFinder.getBoundingRect(i);
        // z方向は面積から取得
        float area = (rect.width / 10) * rect.height;
        float z = ofMap((2.25 / (area /10000)) / 2, 0, 50,-50, 10);
        float x = ofMap(contourFinder.getCentroid(i).x,0, ofGetWidth(),0, 1);
        float y = ofMap(contourFinder.getCentroid(i).y,0, ofGetHeight(),1, 0);
        ofxOscMessage m;
        m.setAddress("/user/position");
        m.addIntArg(i);
        m.addFloatArg(x);
        m.addFloatArg(y);
        m.addFloatArg(z);
//        m.addIntArg(oscCount);
        sender.sendMessage(m);
        dumpOSC(m);
        oscPositionCount++;
        oscCount++;
    }
}
//--------------------------------------------------------------
void ofApp::sendFlowVector(){
    // TODO:flowベクターを送る
    
    ofxOscMessage m;
    m.setAddress("/user/flow");
    m.addIntArg(oscCount);
    sender.sendMessage(m);
    dumpOSC(m);
    oscFlowCount;
    oscCount++;
    
}