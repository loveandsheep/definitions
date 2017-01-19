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
#include "ofxEasing.h"

#include "ofxKsmrAgileEye.h"
#include "arm04Sim.hpp"

class nodeOutlet;
class nodeInlet;
class node;

class nodeInlet{
public:
	ofVec2f absPos;
	string label;
	float param = 0;
	ofPtr<nodeOutlet> targ;
	void disConnect();
};

class nodeOutlet{
public:
	ofVec2f absPos;
	string label;
	float param = 0;
	bool connected;
	int connectFrame;
	
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
	void connectTo(int index, ofPtr<nodeOutlet> out);
	void disconnectInlets(int index = -1);
	void disconnectOutlets(int index = -1);
};


class node{
public:
	
	void setup(ofVec2f pos_, int tp = TYPE_AGILE);
	void update();
	void draw();
	
	void draw_inlets();
	void draw_outlets();
	
	void addInlet(string label, float def = 0.0);
	void addOutlet(string label);
	
	float getInletValue(string label);
	void setOutletValue(string name, float param);
	
	ofVec2f pos;
	ofVec2f pos_base;
	ofFloatColor bgColor;
	float	area_scale;
	float	seed;
	int		frame;
	
	bool	isClosing = false;
	bool	needErase;
	int		closeFrame;
	
	//actions
	void appear();
	void disappear();
	
	nodeIOManager manager;
	
	static const int TYPE_AGILE = 0;
	static const int TYPE_ARM = 1;
	static const int TYPE_POP_A = 2;
	static const int TYPE_POP_B = 3;
	static const int TYPE_CIRCLE = 4;
	static const int current_type_count = 5;
	
	int type = TYPE_AGILE;
	
	ofxKsmrAgileEye agileEye;
	arm04Sim arm;
	ofVec3f armTarg;
	ofVec3f arm_ease;
	
	int popStat[10];
	int popCount[10];
	int bangFrame[10];
	int bangTarg = -1;
	float bangDelay = 0.0;
	bool hwConnected = false;
	
	float circle_phase = 0;
};

#endif /* node_hpp */
