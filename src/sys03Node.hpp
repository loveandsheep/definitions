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
	
	bool initSender;
	ofxOscSender sender;
};

#endif /* sys03Node_hpp */
