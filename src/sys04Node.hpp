//
//  sys04Node.hpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#ifndef sys04Node_hpp
#define sys04Node_hpp

#include "node.hpp"
#include "ofxOsc.h"
#include "difinitions_const.h"

class sys04Node{
public:
	bool setTarget(ofPtr<node> targ);
	
	void setup(bool isC);
	void update();
	
	ofPtr<node> targNode;
	
	ofxOscSender sender;
	bool senderSetup;
};

#endif /* sys04Node_hpp */
