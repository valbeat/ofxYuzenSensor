#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#define _USE_LIVE_VIDEO

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        //カメラ
        ofVideoGrabber camera;
        int camWidth;
        int camHeight;
    
        //OpenCV
        ofxCv::ContourFinder contourFinder; //輪郭抽出
        ofxCv::RunningBackground background; //背景
    
        bool isLearnBg; // 背景学習中かどうか
    
        // 画像
        ofImage bgImg; //背景画像
        ofImage diffImg; //差分画像
    
        //GUI
        ofxPanel gui;
    
        ofxFloatSlider bgThresh; //2値化閾値制御
        ofxFloatSlider contourThresh; //輪郭閾値制御
    
        ofxButton resetBackgroundButton;
        ofxButton fullScreenToggle;
    
        ofxToggle diffFlag;
        ofxToggle contourFlag;
        ofxToggle bgFlag;
        ofxToggle cameraFlag;
        ofxToggle guiFlag;
    
    
        //ボタンの動作
        void resetBackgroundPressed();
        void toggleFullScreenPressed();
};
