#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#define _USE_LIVE_VIDEO

#define HOST "localhost" //送信先ホストのIPを設定
#define PORT 6666 //ポート番号

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
        ofxCv::FlowFarneback farneback;
        ofxCv::Flow* curFlow; //密なオプティカルフロー
        ofxCv::FlowPyrLK pyrLk; //疎なオブティカルフロー
    
        // 画像
        ofImage bgImg; //背景画像
        ofImage diffImg; //差分画像
        ofImage prev1Img; //1つ前の画像
        ofImage prev2Img; //2つ前の画像
        ofImage prev3Img; //3つ前の画像
    
        //GUI
        ofxPanel gui;
        ofxIntSlider medianScale;
        ofxFloatSlider minRad;
        ofxFloatSlider maxRad;
        ofxFloatSlider minArea;
        ofxFloatSlider maxArea;
        ofxFloatSlider flowScale;
        ofxFloatSlider pyrScale;
        ofxIntSlider levels;
        ofxIntSlider winSize;
        ofxIntSlider maxLevel;
        ofxIntSlider iterations;
        ofxIntSlider polyN;
        ofxFloatSlider polySigma;
        ofxFloatSlider bgThresh; //2値化閾値制御
        ofxFloatSlider contourThresh; //輪郭閾値制御
        ofxFloatSlider qualityLevel;
        ofxFloatSlider minDistance;
    
        ofxButton resetBackgroundButton;
        ofxButton fullScreenToggle;
    
        ofxToggle diffFlag;
        ofxToggle contourFlag;
        ofxToggle bgFlag;
        ofxToggle cameraFlag;
        ofxToggle guiFlag;
        ofxToggle learnBgFlag;
        ofxToggle useFarneback;
        ofxToggle OPTFLOW_FARNEBACK_GAUSSIAN;
    
        //ボタンの動作
        void resetBackgroundPressed();
        void toggleFullScreenPressed();

    
    private:
        ofxOscSender sender;
        void sendContourPosition();
        void sendFlowVector();
        void dumpOSC(ofxOscMessage m);
        int oscCount;
};
