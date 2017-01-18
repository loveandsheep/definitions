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
	try{
		sender.setup(isC ? def::ADDR_SYS06_A : def::ADDR_SYS06_B, 12400);
	} catch (std::runtime_error) {
		senderSetup = false;
		cout << "osc error" << endl;
	}

}

bool sys06Node::setTarget(ofPtr<node> targ)
{
	if (targ->type == node::TYPE_POP_A ||
		targ->type == node::TYPE_POP_B)
	{
		if (targNode) targNode->bgColor.set(1.0, 1.0, 1.0, 0.2);
		targNode = targ;
		targNode->bgColor.set(1.0, 0.3, 0.3, 0.2);
		
		return true;
	}
	
	return false;
}

void sys06Node::update()
{
	if (targNode)
	{
		if (targNode->needErase)
		{
			targNode.reset();
			return;
		}
		
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
					m.addIntArg(targNode->bangTarg);
					m.addIntArg(0);
					sender.sendMessage(m);
				}
				targNode->bangTarg = -1;
			}
		}
		
	}
}
