//
//  node.cpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#include "node.hpp"

void node::setup(ofVec2f pos_)
{
	pos = pos_;
	frame = 0;
	
	int iCt = ofRandom(1, 10);
	int oCt = ofRandom(1, 10);
	for (int i = 0;i < iCt;i++) addInlet("t"+ofToString(i));
	for (int i = 0;i < oCt;i++) addOutlet("t"+ofToString(i));
}

void node::update()
{
	frame++;
}

void node::draw()
{
	ofPushMatrix();
	ofTranslate(pos);
	
	ofSetColor(255);
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	float time_opening = ofxeasing::map_clamp(frame, 0, 6, 0.0, 1.0, ofxeasing::quint::easeOut);
	float time_opening_b = ofxeasing::map_clamp(frame, 10, 40, 0.0, 1.0, ofxeasing::quint::easeOut);
	
	ofNoFill();
	ofDrawRectangle(0, 0, time_opening * 200, time_opening_b * 95 + 5);
	ofFill();
	
	ofSetColor(255, 0, 0);
	for (int i = 0;i < manager.inlets.size();i++)
	{
		ofDrawBitmapString(manager.inlets[i]->label, i * 30, 130);
		ofDrawCircle(i * 30, 100, 5);
	}
	
	ofSetColor(0, 0, 255);
	for (int i = 0;i < manager.outlets.size();i++)
	{
		ofDrawCircle(i * 30, -100, 5);
		ofDrawBitmapString(manager.outlets[i]->label, i * 30, -130);
	}
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	ofPopMatrix();
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
	if (index == -1)
		for (auto it : inlets) it->disConnect();
	else
	{
		index = ofClamp(index, 0, inlets.size() - 1);
		inlets[index]->disConnect();
	}
}

void nodeIOManager::disconnectOutlets(int index)
{
	if (index == -1)
		for (auto it : outlets) it->targ.reset();
	else
	{
		index = ofClamp(index, 0, outlets.size() - 1);
		outlets[index]->targ.reset();
	}

}

void nodeIOManager::connectTo(int index, ofPtr<nodeOutlet> out)
{
	index = ofClamp(index, 0, outlets.size() - 1);
	if (!inlets[index]->targ)
	{
		inlets[index]->targ = out;
		out->targ = inlets[index];
	}
}

void nodeInlet::disConnect()
{
	targ->targ.reset();
	targ.reset();
}
