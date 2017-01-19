//
//  sys06Node.hpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#ifndef sys06Node_hpp
#define sys06Node_hpp

#include "node.hpp"
#include "ofxOsc.h"
#include "difinitions_const.h"

class sys06Node{
public:
	
	bool setTarget(ofPtr<node> targ);
	
	void setup(bool isC);
	void update();
	
	ofPtr<node> targNode;
	ofPtr<node> previousNode;
	
	ofxOscSender sender;
	bool senderSetup;
	
	ofVec2f oldPos;
	ofVec2f previousOld;
	int targFrame;
	int previousFrame;
	ofVec2f HWPos = ofVec2f(600, 800);
	int targType;
	bool bDefault;
};

#endif /* sys06Node_hpp */
