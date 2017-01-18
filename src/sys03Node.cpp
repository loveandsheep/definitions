//
//  sys03Node.cpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#include "sys03Node.hpp"

void sys03Node::setup()
{
	sender.setup(def::ADDR_SYS03, 54503);
}

bool sys03Node::setTarget(ofPtr<node> targ)
{
	if (targ->type == node::TYPE_AGILE)
	{
		if (targNode) targNode->bgColor.set(1.0,1.0,1.0, 0.2);
		targNode = targ;
		targNode->bgColor.set(1.0, 0.3, 0.3, 0.2);
		return true;
	}
	
	return false;
}

void sys03Node::update()
{

	if (targNode)
	{
		if (ofGetFrameNum() % 10 == 0)
		{
			float px, py;
			px = ofMap(targNode->getInletValue("pos-X"), 0, 1, -100, 100, true);
			py = ofMap(targNode->getInletValue("pos-Y"), 0, 1, -100, 100, true);
			
			ofxOscMessage m;
			m.setAddress("/system03/point");
			m.addFloatArg(px);
			m.addFloatArg(py);
			m.addFloatArg(0);
			
			ofxOscMessage m2;
			m2.setAddress("/system03/gpio");
			m2.addIntArg(targNode->getInletValue("drawing") > 0.5);
			
			sender.sendMessage(m);
			sender.sendMessage(m2);
		}
	}
}
