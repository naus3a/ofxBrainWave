#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofBackground(255, 255, 255);
	
	rawEeg=0;
	sigVal=0;
	for(int i=0;i<8;i++){	
		eegVal[i]=0;
	}
	
	eegNames[0]="low alpha";
	eegNames[1]="high alpha";
	eegNames[2]="low beta";
	eegNames[3]="high beta";
	eegNames[4]="low gamma";
	eegNames[5]="high gamma";
	eegNames[6]="delta";
	eegNames[7]="theta";
	
	curAtt=0;
	curMed=0;
	mshAtt.setUsage(GL_DYNAMIC_DRAW);
	mshMed.setUsage(GL_DYNAMIC_DRAW);
	mshAtt.setMode(OF_PRIMITIVE_LINE_STRIP);
	mshMed.setMode(OF_PRIMITIVE_LINE_STRIP);
	for(int i=0;i<400;i++){
		mshAtt.addVertex(ofVec3f(i,0,0));
		mshMed.addVertex(ofVec3f(i,0,0));
		mshAtt.addColor(ofColor::red);
		mshMed.addColor(ofColor::cyan);
	}
	
	blinks = 0;
	
	brainWave.addListener(this);
    brainWave.setup(false);
	brainWave.start();
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushStyle();
	ofSetColor(ofColor::black);
	ofDrawBitmapString("Raw EEG: "+ofToString(rawEeg), 10,10);
	ofDrawBitmapString("Sig Quality: "+ofToString(sigVal), 10,30);
	ofDrawBitmapString("Blinks: "+ofToString(blinks), 200,10);
	
	for(int i=0;i<8;i++){
		float yy = 50+i*20;
		ofDrawBitmapString(eegNames[i], 10, yy);
		ofLine(150, yy, 150+(eegVal[i]/1000), yy);
	}
	
	ofNoFill();
	
	ofPushMatrix();
	ofTranslate(10, 400, 0);
	ofScale(1, -1, 1);
	ofLine(0, 0, 400, 0);
	ofLine(0, 0, 0, 200);
	mshAtt.draw();
	mshMed.draw();
	ofPopMatrix();
	
	ofSetColor(ofColor::black);
	ofRect(150, 15, 200, 20);
	ofFill();
	ofSetColor(ofColor::green);
	ofRect(150, 15, 200-sigVal, 20);
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case 'r':
			brainWave.resetConnection();
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}

void testApp::onEeg(ofxEegEventArgs& eeg){
	eegVal[0]=eeg.lowAlpha;
	eegVal[1]=eeg.highAlpha;
	eegVal[2]=eeg.lowBeta;
	eegVal[3]=eeg.highBeta;
	eegVal[4]=eeg.lowGamma;
	eegVal[5]=eeg.highGamma;
	eegVal[6]=eeg.delta;
	eegVal[7]=eeg.theta;
}

void testApp::onESense(ofxESenseArgs& es){
	mshAtt.setVertex(curAtt, ofVec3f(curAtt,es.attention,0));
	mshMed.setVertex(curMed, ofVec3f(curMed,es.meditation,0));
	
	curAtt=curAtt<(mshAtt.getNumVertices()-1)?curAtt+1:0;
	curMed=curMed<(mshMed.getNumVertices()-1)?curMed+1:0;
}

void testApp::onBlink(int& s){
	blinks++;
}

void testApp::onSigLevel(int& s){
	sigVal=s;
}

void testApp::onRawEeg(int& s){
	rawEeg=s;
}

void testApp::exit(){
	brainWave.removeListener(this);
}