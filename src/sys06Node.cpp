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
	sender.setup(isC ? def::ADDR_SYS06_A : def::ADDR_SYS06_B, 12400);
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
		if (targNode->type == node::TYPE_POP_A)
		{
			for (int i = 0;i < 9;i++)
			{
				if (targNode->manager.outlets[i]->param == 1)
				{
					ofxOscMessage m;
					m.setAddress("/bang");
					m.addIntArg(i / 3);
					m.addIntArg(i % 3);
					sender.sendMessage(m);
				}
			}
			
		}
		
		ofxOscMessage m;
		m.setAddress("/bang");
//		m.addIntArg(<#std::int32_t argument#>)
		
	}
}
