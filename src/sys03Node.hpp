//
//  sys03Node.hpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#ifndef sys03Node_hpp
#define sys03Node_hpp

#include "node.hpp"
#include "ofxOsc.h"
#include "difinitions_const.h"

class sys03Node{
public:
	
	bool setTarget(ofPtr<node> targ);

	void setup();
	void update();
	
	ofPtr<node> targNode;
	ofPtr<node> previousNode;
	
	bool initSender;
	ofxOscSender sender;
	bool senderSetup;
	
	ofVec2f oldPos;
	ofVec2f previousOld;
	int targFrame;
	int previousFrame;
	const ofVec2f sys03HWPos = ofVec2f(700, 930);
	
	bool bDefault;
};

#endif /* sys03Node_hpp */
