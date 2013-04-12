#include <iostream>
#include "ofxBrainWave.h"

ofxBrainWave::ofxBrainWave() {
    host = "127.0.0.1";
    port = 13854;
}

ofxBrainWave::~ofxBrainWave() {
    if (this->isThreadRunning())
        stop();
    tcpClient.close();
}

void ofxBrainWave::setHost(string h) {
    host = h;
}

void ofxBrainWave::setPort(int p) {
    port = p;
}

void ofxBrainWave::setup(bool bRawData) {
    bGetRaw = bRawData;
	tcpClient.setup(host, port);
    if(tcpClient.isConnected()){
		cout << "connected to ThinkGear Connector..." <<endl;
		tcpClient.setMessageDelimiter("\r");
		tcpClient.send("{""appName"": ""DanseNeurAle"", ""appKey"": ""0000000000000000000000000000000000000000""}");
		ofSleepMillis(1000);
		string sRD = bRawData?"true":"false";
		tcpClient.send("{""enableRawOutput"": "+sRD+", ""format"": ""Json""}");
	}
	//tcpClient.setMessageDelimiter("\r");
    //tcpClient.send("{""enableRawOutput"": true, ""format"": ""Json""}");
}

void ofxBrainWave::resetConnection(){
	if(this->isThreadRunning()){
		stop();
	}
	tcpClient.close();
	
	ofSleepMillis(100);
	setup(bGetRaw);
	start();
}

void ofxBrainWave::stop() {
    stopThread();
}

void ofxBrainWave::start() {
    startThread();
}

void ofxBrainWave::threadedFunction() {
    while (this->isThreadRunning()) {
        this->checkStream();
    } 
}

void ofxBrainWave::checkStream() {
    ofxBrainWaveScopedLock scopedLock(mutex);
	
    if (tcpClient.isConnected()) {
        string val = tcpClient.receive();
		if(val.length()>0 && json.parse(val)){
			if(json.isMember("eegPower")){
				//cout << "tcp->eeg: " << val << endl;
				eegPower.delta = json["eegPower"]["delta"].asInt();
				eegPower.theta = json["eegPower"]["theta"].asInt();
				eegPower.lowAlpha = json["eegPower"]["lowAlpha"].asInt();
				eegPower.highAlpha = json["eegPower"]["highAlpha"].asInt();
				eegPower.lowBeta = json["eegPower"]["lowBeta"].asInt();
				eegPower.highBeta = json["eegPower"]["highBeta"].asInt();
				eegPower.lowGamma = json["eegPower"]["lowGamma"].asInt();
				eegPower.highGamma = json["eegPower"]["highGamma"].asInt();
				
				notifyEeg();
			}
			
			if(json.isMember("eSense")){
				//cout << "tcp->eSense: "<<val<<endl;
				eSense.attention = json["eSense"]["attention"].asInt();
				eSense.meditation= json["eSense"]["meditation"].asInt();
				
				notifyESense();
			}
			
			if(json.isMember("rawEeg")){
				//cout << "tcp->raw: "<<val<<endl;
				rawEeg=json["rawEeg"].asInt();
				
				notifyRawEeg();
			}
			
			if(json.isMember("blinkStrength")){
				//cout << "tcp->blink: "<<val<<endl;
				blinkStrength=json["blinkStrength"].asInt();
				
				notifyBlinkStrength();
			}
			if(json.isMember("poorSignalLevel")){
				//cout << "tcp->signal: "<<val<<endl;
				poorSignalLevel=json["poorSignalLevel"].asInt();
				
				notifySigLevel();
			}
			
		}
		
        /*if (val.length() > 0 && json.parse(val)) {
            if (json.isMember("eegPower")) {
                eegData.delta = json["eegPower"]["delta"].asInt();
                eegData.theta = json["eegPower"]["theta"].asInt();
                eegData.lowAlpha = json["eegPower"]["lowAlpha"].asInt();
                eegData.highAlpha = json["eegPower"]["highAlpha"].asInt();
                eegData.lowBeta = json["eegPower"]["lowBeta"].asInt();
                eegData.highBeta = json["eegPower"]["highBeta"].asInt();
                eegData.lowGamma = json["eegPower"]["lowGamma"].asInt();
                eegData.highGamma = json["eegPower"]["highGamma"].asInt();
            }
            
            if (json.isMember("eSense")) {
                eegData.attention = json["eSense"]["attention"].asInt();
                eegData.meditation = json["eSense"]["meditation"].asInt();
            }
			
        }*/
    }
}

void ofxBrainWave::notifyEeg(){
	//ofxBrainWaveScopedLock scopedLock(mutex);
	ofxEegEventArgs args;
	args.delta = eegPower.delta;
	args.theta = eegPower.theta;
	args.lowAlpha = eegPower.lowAlpha;
	args.highAlpha = eegPower.highAlpha;
	args.lowBeta = eegPower.lowBeta;
	args.highBeta = eegPower.highBeta;
	args.lowGamma = eegPower.lowGamma;
	args.highGamma = eegPower.highGamma;
	ofNotifyEvent(eegEvent, args);
}

void ofxBrainWave::notifyESense(){
	ofxESenseArgs args;
	args.attention = eSense.attention;
	args.meditation= eSense.meditation;
	ofNotifyEvent(eSenseEvent, args);
}

void ofxBrainWave::notifyRawEeg(){
	int arg = rawEeg;
	ofNotifyEvent(rawEegEvent, arg);
}

void ofxBrainWave::notifySigLevel(){
	int arg = poorSignalLevel;
	ofNotifyEvent(sigLevEvent, arg);
}

void ofxBrainWave::notifyBlinkStrength(){
	int arg = blinkStrength;
	ofNotifyEvent(blinkEvent, arg);
}

EegPower ofxBrainWave::getEegPower(){
	ofxBrainWaveScopedLock scopedLock(mutex);
	return eegPower;
}

ESense ofxBrainWave::getESense(){
	ofxBrainWaveScopedLock scopedLock(mutex);
	return eSense;
}

int ofxBrainWave::getRawEeg(){
	ofxBrainWaveScopedLock scopedLock(mutex);
	return rawEeg;
}

int ofxBrainWave::getSignalLevel(){
	ofxBrainWaveScopedLock scopedLock(mutex);
	return poorSignalLevel;
}

int ofxBrainWave::getBlinkStrength(){
	ofxBrainWaveScopedLock scopedLock(mutex);
	return blinkStrength;
}

/*EegData ofxBrainWave::getEegData() {
    ofxBrainWaveScopedLock scopedLock(mutex);
    return eegData;
}*/
