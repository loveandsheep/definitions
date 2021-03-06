//
//  sys06Node.cpp
//  definitions
//
//  Created by Ovis aries on 2017/01/18.
//
//

#include "sys06Node.hpp"

void sys06Node::setup(bool isC)
{
	senderSetup = true;
	HWPos = (isC ? ofVec2f(1060 + 1920 * 2, 930) : ofVec2f(1660 + 1920 * 2, 930));
	targType = isC ? node::TYPE_POP_A : node::TYPE_POP_B;
	
	try{
		sender.setup(isC ? def::ADDR_SYS06_A : def::ADDR_SYS06_B, 12400);
	} catch (std::runtime_error) {
		senderSetup = false;
		cout << "osc error" << endl;
	}

}

bool sys06Node::setTarget(ofPtr<node> targ)
{
	if (targ->type == targType)
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

void sys06Node::update()
{
	
	if (bDefault && senderSetup)
	{
		if (ofGetFrameNum() % 15 == 0)
		{
			ofxOscMessage m;
			m.setAddress("/mode");
			m.addIntArg(0);
			sender.sendMessage(m);
		}
		return;

	}else if (senderSetup){
		if (ofGetFrameNum() % 15 == 0)
		{
			ofxOscMessage m;
			m.setAddress("/mode");
			m.addIntArg(2);
			sender.sendMessage(m);
		}
	}
	
	if (previousNode)
	{
		previousFrame++;
		float lerp = ofxeasing::map_clamp(previousFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		
		previousNode->pos_base = HWPos.getInterpolated(previousOld, lerp);
		if (lerp == 1) previousNode.reset();
	}
	
	if (targNode)
	{
		
		targFrame++;
		float lerp = ofxeasing::map_clamp(targFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		targNode->pos_base = oldPos.getInterpolated(HWPos, lerp);
		
		if (targNode->needErase)
		{
			targNode.reset();
			return;
		}
		
		if (targFrame > 120)
		{
			if (targNode->type == node::TYPE_POP_A)
			{
				if (targNode->bangTarg >= 0)
				{
					if (senderSetup)
					{
						ofxOscMessage m;
						m.setAddress("/bang");
						m.addIntArg(targNode->bangTarg / 3);
						m.addIntArg(targNode->bangTarg % 3);
						sender.sendMessage(m);
					}
					targNode->bangTarg = -1;
				}
			}
			if (targNode->type == node::TYPE_POP_B)
			{
				if (targNode->bangTarg >= 0)
				{
					if (senderSetup)
					{
						ofxOscMessage m;
						m.setAddress("/bang");
						m.addIntArg(0);
						m.addIntArg(targNode->bangTarg);
						sender.sendMessage(m);
					}
					targNode->bangTarg = -1;
				}
			}
		}
	}
}
