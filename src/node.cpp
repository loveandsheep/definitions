//
//  node.cpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#include "node.hpp"

void node::setup()
{
	frame = 0;
}

void node::update()
{
	frame++;
}

void node::draw()
{
	
}

void node::addInlet(string label)
{
	ofPtr<nodeInlet> il = ofPtr<nodeInlet>(new nodeInlet);
	il->label = label;
	manager.inlets.push_back(il);
}

void node::addOutlet(string label)
{
	ofPtr<nodeOutlet> ol = ofPtr<nodeOutlet>(new nodeOutlet);
	ol->label = label;
	manager.outlets.push_back(ol);
}

void nodeIOManager::disconnectInlets(int index)
{
	for (auto it : inlets) it->targ->targ.reset();
}

void nodeIOManager::disconnectOutlets(int index)
{
	for (auto it : outlets) it->targ.reset();
}
