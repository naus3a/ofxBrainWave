#ifndef emptyExample_brainWave_h
#define emptyExample_brainWave_h

#include "ofxJSONElement.h"
#include "ofxNetwork.h"
#include "ofMain.h"
//#include "ofThread.h"
//#include "ofEvents.h"
//#include <list>

class ofxEegEventArgs : public ofEventArgs{
public:
	float delta;
	float theta;
	float lowAlpha;
	float highAlpha;
	float lowBeta;
	float highBeta;
	float lowGamma;
	float highGamma;
};

class ofxESenseArgs : public ofEventArgs{
public:
	int attention;
	int meditation;
};

class EegData {
    public :
    EegData() { delta = theta = lowAlpha = highAlpha = lowBeta = highBeta = lowGamma = highGamma = attention = meditation = 0;}
    float delta;
    float theta;
    float lowAlpha;
    float highAlpha;
    float lowBeta;
    float highBeta;
    float lowGamma;
    float highGamma;
    float attention;
    float meditation;
};

class EegPower{
public:
	EegPower(){delta = theta = lowAlpha = highAlpha = lowBeta = highBeta = lowGamma = highGamma = 0;}
	float delta;
	float theta;
	float lowAlpha;
	float highAlpha;
	float lowBeta;
	float highBeta;
	float lowGamma;
	float highGamma;
};

class ESense{
public:
	//int 0-100
	ESense(){attention = meditation = 0;}
	int attention;
	int meditation;
};

class ofxBrainWave : public ofThread {
    private:
        string host;
        int port;
        ofxTCPClient tcpClient;
        ofxJSONElement json;
        //EegData eegData;

    protected:
        void threadedFunction();
        void checkStream();
    
    public:
        ofxBrainWave();
        ~ofxBrainWave();
        void setHost(string h);
        void setPort(int p);
        void setup(bool bRawData);
        void stop();
        void start();
		void resetConnection();
    
        //EegData getEegData();
	
	//events
	ofEvent <ofxEegEventArgs> eegEvent;
	ofEvent <ofxESenseArgs> eSenseEvent;
	ofEvent <int> blinkEvent;
	ofEvent <int> sigLevEvent;
	ofEvent <int> rawEegEvent;
	
	template <class L>
	void addListener(L* listener){
		ofAddListener(eegEvent, listener, &(L::onEeg));
		ofAddListener(eSenseEvent, listener, &(L::onESense));
		ofAddListener(blinkEvent, listener, &(L::onBlink));
		ofAddListener(sigLevEvent, listener, &(L::onSigLevel));
		ofAddListener(rawEegEvent, listener, &(L::onRawEeg));
	}
	
	template <class L>
	void removeListener(L* listener){
		ofRemoveListener(eegEvent, listener, &(L::onEeg));
		ofRemoveListener(eSenseEvent, listener, &(L::onESense));
		ofRemoveListener(blinkEvent, listener, &(L::onBlink));
		ofRemoveListener(sigLevEvent, listener, &(L::onSigLevel));
		ofRemoveListener(rawEegEvent, listener, &(L::onRawEeg));
	}
	
	void notifyEeg();
	void notifyESense();
	void notifyRawEeg();
	void notifySigLevel();
	void notifyBlinkStrength();
	
	EegPower getEegPower();
	ESense getESense();
	int getRawEeg();
	int getSignalLevel();
	int getBlinkStrength();
	
	EegPower eegPower;
	ESense eSense;
	float rawEeg;
	int poorSignalLevel; //0-200
	int blinkStrength;
	
	bool bGetRaw;
};


class ofxBrainWaveScopedLock {
public:
    ofxBrainWaveScopedLock(ofMutex & _mutex): mutex(_mutex){
        mutex.lock();
    };
    ~ofxBrainWaveScopedLock(){
        mutex.unlock();
    };
    ofMutex & mutex;
};

#endif
