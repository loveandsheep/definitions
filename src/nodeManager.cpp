//
//  nodeManager.cpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#include "nodeManager.hpp"

void nodeManager::setup()
{
	sys03node.setup();
}

void nodeManager::update()
{
	
	sys03node.update();
	
	vector<ofPtr<node> >::iterator it = nodes.begin();
	
	while (it != nodes.end())
	{
		(*it)->update();
		if ((*it)->needErase)
		{
			it = nodes.erase(it);
		}
		else{
			++it;
		}
	}
}

void nodeManager::draw()
{
	for (auto it : nodes) it->draw();
	
	if (ofGetFrameNum() % 60 == 0)
	{
		if (nodes.size() > 3)
			sys03node.setTarget(nodes[int(ofRandom(100)) % nodes.size()]);
	}
}

void nodeManager::addNewNode(ofVec2f pos, int type)
{
	ofPtr<node> nd = make_shared<node>();
	nd->setup(pos, type);
	nodes.push_back(nd);
	
	
	if (nodes.size() > 3)
	{
		for (int i = 0;i < 10;i++)
		{
			ofPtr<node> randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
			while (randomConnect == nd)
			{
				randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
			}
			randomConnect->manager.connectTo(ofRandom(5),
											 nd->manager.outlets[int(ofRandom(10)) % nd->manager.outlets.size()]);			
		}
	}
}
