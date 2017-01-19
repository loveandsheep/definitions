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
	senderSetup = true;
	try {
		sender.setup(def::ADDR_SYS03, 54503);
	} catch (std::runtime_error) {
		cout << "osc error" << endl;
		senderSetup = false;
	}
	
}

bool sys03Node::setTarget(ofPtr<node> targ)
{
	if (targ->type == node::TYPE_AGILE)
	{
		if (targNode)
		{
			targNode->bgColor.set(1.0,1.0,1.0, 0.1);
			targNode->hwConnected = false;
			previousNode = targNode;
			previousOld = oldPos;
			previousFrame = 0;
		}
		targNode = targ;
		targNode->hwConnected = true;
		targNode->bgColor.set(1.0, 0.3, 0.3, 0.2);
		oldPos = targNode->pos_base;
		targFrame = 0;
		return true;
	}
	
	return false;
}

void sys03Node::update()
{
	if (bDefault)
	{
		if (ofGetFrameNum() % 5 == 0)
		{
			ofxOscMessage m;
			m.setAddress("/system03/manual");
			m.addIntArg(0);
			
			ofxOscMessage m2;
			m2.setAddress("/system03/default");
			m2.addIntArg(1);
			
			ofxOscMessage m3;
			m3.setAddress("/system03/gpio");
			m3.addIntArg(ofGetFrameNum() % 10 == 0);
			
			sender.sendMessage(m);
			sender.sendMessage(m2);
			sender.sendMessage(m3);
		}
		return;
	}else{
		if (ofGetFrameNum() % 5 == 0)
		{
			ofxOscMessage m2;
			m2.setAddress("/system03/manual");
			m2.addIntArg(1);
			
			ofxOscMessage m;
			m.setAddress("/system03/default");
			m.addIntArg(0);
			
			sender.sendMessage(m);
			sender.sendMessage(m2);
		}
	}

	if (previousNode)
	{
		previousFrame++;
		float lerp = ofxeasing::map_clamp(previousFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		
		previousNode->pos_base = sys03HWPos.getInterpolated(previousOld, lerp);
		if (lerp == 1) previousNode.reset();
	}
	
	if (targNode)
	{
		targFrame++;
		float lerp = ofxeasing::map_clamp(targFrame, 0, 120, 0, 1,
										  ofxeasing::quint::easeInOut);
		targNode->pos_base = oldPos.getInterpolated(sys03HWPos, lerp);
		
		if ((targFrame > 120) && (ofGetFrameNum() % 10 == 0))
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
			
			if (senderSetup){
				sender.sendMessage(m);
				sender.sendMessage(m2);
			}
		}
	}
}
