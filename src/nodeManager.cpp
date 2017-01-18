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
	sys04node_c.setup(true);
	sys04node_p.setup(false);
	sys06node_a.setup(true);
}

void nodeManager::update()
{
	
	sys03node.update();
	sys04node_c.update();
	sys04node_p.update();
	sys06node_a.update();
	
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
	
	for (int i = 0;i < nodes.size();i++)
	{
		for (int j = i;j < nodes.size();j++)
		{
			float dist = nodes[i]->pos_base.squareDistance(nodes[j]->pos_base);
			if (dist < powf(200, 2))
			{
				nodes[i]->pos_base += (nodes[i]->pos_base - nodes[j]->pos_base).getNormalized() * 10000.0 / dist;
				nodes[j]->pos_base += (nodes[j]->pos_base - nodes[i]->pos_base).getNormalized() * 10000.0 / dist;
			}
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
		
		if (nodes.size() > 3)
			sys04node_c.setTarget(nodes[int(ofRandom(100)) % nodes.size()]);
		
		if (nodes.size() > 3)
			sys04node_p.setTarget(nodes[int(ofRandom(100)) % nodes.size()]);
		
		if (nodes.size() > 3)
			sys06node_a.setTarget(nodes[int(ofRandom(100)) % nodes.size()]);
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
			
			if (nd->manager.outlets.size() > 0)
			{
				randomConnect->manager.connectTo(ofRandom(5),
												 nd->manager.outlets[int(ofRandom(10)) % nd->manager.outlets.size()]);
			}
		}
	}
}
