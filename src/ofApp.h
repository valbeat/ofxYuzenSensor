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
        ofVideoPlayer video;
        int camWidth;
        int camHeight;
    
        //OpenCV
        ofxCv::ContourFinder contourFinder; //輪郭抽出
        ofxCv::RunningBackground background; //背景
        ofxCv::Flow* curFlow;
        ofxCv::FlowFarneback farneback;//密なオプティカルフロー
        ofxCv::FlowPyrLK pyrLk; //疎なオブティカルフロー
    
        // 画像
        ofImage bgImg; //背景画像
        ofImage diffImg; //差分画像
        ofImage prev1Img; //1つ前の画像
        ofImage prev2Img; //2つ前の画像
        ofImage prev3Img; //3つ前の画像
    
        //GUI
        ofxPanel gui;
        ofxToggle diffFlag;
        ofxToggle contourFlag;
        ofxToggle bgFlag;
        ofxToggle cameraFlag;
        ofxToggle flowFlag;
        ofxToggle guiFlag;
        ofxToggle learnBgFlag;
        ofxToggle liveVideoFlag;
        // median filter
        ofxIntSlider medianScale; // メディアンフィルタのぼかしの大きさ
        // contourFinder
        ofxFloatSlider minRad; // 最小半径
        ofxFloatSlider maxRad; // 最大半径
        ofxFloatSlider minArea; // 最小面積比率
        ofxFloatSlider maxArea; // 最大面積比率
        ofxFloatSlider contourThresh; //輪郭閾値制御
        // optical flow
        ofxToggle useFarneback;
        ofxToggle OPTFLOW_FARNEBACK_GAUSSIAN;
        ofxFloatSlider flowScale;
        ofxFloatSlider pyrScale;
        ofxIntSlider iterations;
        ofxIntSlider polyN;
        ofxIntSlider winSize;
        ofxIntSlider levels;
        ofxIntSlider maxLevel;
        ofxFloatSlider polySigma;
        ofxFloatSlider maxFeatures;
        ofxFloatSlider qualityLevel;
        ofxFloatSlider minDistance;
        // basic
        ofxFloatSlider bgThresh; //2値化閾値制御
        ofxButton resetBackgroundButton;
        ofxButton fullScreenToggle;

        //ボタンの動作
        void resetBackgroundPressed();
        void toggleLiveVideoPressed();
        void toggleFullScreenPressed();
    
    private:
        ofxOscSender sender;
        ofFbo fbo;
        int oscCount,oscPositionCount,oscFlowCount;
        void sendContourPosition();
        void sendOpticalFlow();
        void sendFlowVector();
        void dumpOSC(ofxOscMessage m);
};
