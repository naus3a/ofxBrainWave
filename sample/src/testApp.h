#pragma once

#include "ofMain.h"
#include "ofxBrainWave.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void exit();
	
	void onEeg(ofxEegEventArgs& eeg);
	void onESense(ofxESenseArgs& es);
	void onBlink(int& s);
	void onSigLevel(int& s);
	void onRawEeg(int& s);
	
	ofxBrainWave brainWave;
	
	ofVboMesh mshAtt;
	ofVboMesh mshMed;
	
	string eegNames[8];
	
	int eegVal[8];
	int rawEeg;
	int sigVal;
	int curAtt;
	int curMed;
	int blinks;
};
