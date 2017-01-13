//
//  node.hpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#ifndef node_hpp
#define node_hpp

#include "ofMain.h"

class nodeOutlet;
class nodeInlet;
class node;

class nodeInlet{
public:
	string label;
	float param;
	bool connected;
	
	ofPtr<nodeOutlet> targ;
};

class nodeOutlet{
public:
	string label;
	float param;
	bool connected;
	
	ofPtr<nodeInlet> targ;
};

class nodeIOManager
{
public:
	
	void update();
	
	bool wannaConnect;
	vector<ofPtr<nodeInlet> > inlets;
	vector<ofPtr<nodeOutlet> > outlets;
	
	void beWannaConnect(){wannaConnect = true;}
	void disconnectInlets(int index = -1);
	void disconnectOutlets(int index = -1);
};


class node{
public:
	
	void setup();
	void update();
	void draw();
	
	void addInlet(string label);
	void addOutlet(string label);
	
	ofVec2f pos;
	int		frame;
	
	//actions
	void appear();
	void disappear();
	
	nodeIOManager manager;
};

#endif /* node_hpp */
