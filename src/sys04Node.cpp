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
	HWPos = (isC ? ofVec2f(460 + 1920, 930) : ofVec2f(1660 + 1920, 930));
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
		if (targNode)
		{
			targNode->bgColor.set(1.0,1.0,1.0, 0.1);
			targNode->hwConnected = false;
			previousNode = targNode;
			previousOld = oldPos;
			previousFrame = 0;
		}
		
		if (targNode) targNode->bgColor.set(1.0, 1.0, 1.0, 0.2);
		targNode = targ;
		targNode->hwConnected = true;
		targNode->bgColor.set(1.0, 0.3, 0.3, 0.2);
		oldPos = targNode->pos_base;
		targFrame = 0;
		
		return true;
	}
	
	return false;
}

void sys04Node::update()
{
	if (ofGetFrameNum() % 10 == 0)
	{
		ofxOscMessage m;
		m.setAddress("/manual");
		m.addIntArg(1);
		sender.sendMessage(m);
	}
	
	if (bDefault)
	{
		if (ofGetFrameNum() % 10 == 0)
		{
			ofxOscMessage m;
			m.setAddress("/pos");
			m.addFloatArg(0.0);
			m.addFloatArg(-200);
			m.addFloatArg(0.0);
			sender.sendMessage(m);
		}
		return;
	}
	
	if (previousNode)
	{
		previousFrame++;
		float lerp = ofxeasing::map_clamp(previousFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		
		previousNode->pos_base = HWPos.getInterpolated(previousOld, lerp);
		if (previousFrame > 120)
		{
			previousNode.reset();
		}
	}
	
	
	if (targNode)
	{
		if (targNode->needErase)
		{
			targNode.reset();
			return;
		}
		
		targFrame++;
		float lerp = ofxeasing::map_clamp(targFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		targNode->pos_base = oldPos.getInterpolated(HWPos, lerp);

		
		if ((targFrame > 120) && (ofGetFrameNum() % 120 == 0))
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
