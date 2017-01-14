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
	
}

void nodeManager::update()
{
	for (auto it : nodes) it->update();
}

void nodeManager::draw()
{
	for (auto it : nodes) it->draw();
}

void nodeManager::addNewNode(ofVec2f pos, int type)
{
	ofPtr<node> nd = make_shared<node>();
	nd->setup(pos);
	nodes.push_back(nd);
}
