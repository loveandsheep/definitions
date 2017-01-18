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
	
	ofxOscSender sender;
};

#endif /* sys06Node_hpp */
