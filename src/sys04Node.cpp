//
//  sys04Node.cpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#include "sys04Node.hpp"

void sys04Node::setup(bool isC)
{
	senderSetup = true;
	try{
		sender.setup(isC ? def::ADDR_SYS04_C : def::ADDR_SYS04_P, 12400);
	} catch (std::runtime_error) {
		cout << "osc error" << endl;
		senderSetup = false;
	}

}

bool sys04Node::setTarget(ofPtr<node> targ)
{
	if (targ->type == node::TYPE_ARM)
	{
		if (targNode) targNode->bgColor.set(1.0, 1.0, 1.0, 0.2);
		targNode = targ;
		targNode->bgColor.set(1.0, 0.3, 0.3, 0.2);
		return true;
	}
	
	return false;
}

void sys04Node::update()
{
	if (ofGetFrameNum() % 3 == 0)
	{
		ofxOscMessage m;
		m.setAddress("/manual");
		m.addIntArg(1);
	}
	
	if (targNode)
	{
		if (ofGetFrameNum() % 120 == 0)
		{
			float px, py, pz;
			px = ofMap(targNode->getInletValue("pos-X"), 0, 1, -50, 50, true);
			py = ofMap(targNode->getInletValue("pos-Y"), 0, 1, -100, -200, true);
			pz = ofMap(targNode->getInletValue("pos-Z"), 0, 1, -50, 50, true);
			
			ofxOscMessage m;
			m.setAddress("/pos");
			m.addFloatArg(px);
			m.addFloatArg(py);
			m.addFloatArg(pz);
			
			if (senderSetup) sender.sendMessage(m);
			
		}
	}
}
