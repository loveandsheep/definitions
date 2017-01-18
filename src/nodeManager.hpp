//
//  nodeManager.hpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#ifndef nodeManager_hpp
#define nodeManager_hpp

#include "ofMain.h"
#include "node.hpp"
#include "sys03Node.hpp"
#include "sys04Node.hpp"
#include "sys06Node.hpp"

class nodeManager{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void addNewNode(ofVec2f pos, int type);
	vector< ofPtr<node> > nodes;
	
	sys03Node sys03node;
	sys04Node sys04node_c;
	sys04Node sys04node_p;
	sys06Node sys06node_a;
	sys06Node sys06node_b;
};

#endif /* nodeManager_hpp */
