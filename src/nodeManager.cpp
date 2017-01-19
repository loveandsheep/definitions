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
	sys06node_b.setup(false);
}

void nodeManager::update()
{
	
	sys03node.update();
	sys04node_c.update();
	sys04node_p.update();
	sys06node_a.update();
	sys06node_b.update();
	
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
	
	static int step_delete = ofRandom(10, 30);
	if ((ofGetFrameNum() % step_delete == 0) &&
		(nodes.size() > 10))
	{
		step_delete = ofRandom(120, 600);
		
		for (int i = 0;i < 100;i++)
		{
			ofPtr<node> tg = nodes[int(ofRandom(100)) % nodes.size()];
			if ((tg.use_count() <= 2) && (tg->frame > 120))
			{
				tg->isClosing = true;
				break;
			}
		}
	}
	
	static int step = ofRandom(10, 30);
	if ((ofGetFrameNum() % step == 0) &&
		(nodes.size() < max_node_num))
	{//最も少ないタイプを増やす
		step = ofRandom(30, 150);
		
		int typeCount[node::current_type_count];
		
		for (int i = 0;i < node::current_type_count;i++)
			typeCount[i] = -1;
		
		for (int i = 0;i < nodes.size();i++)
		{
			int tp = nodes[i]->type;
			if (typeCount[tp] == -1) typeCount[tp] = 0;
			typeCount[tp]++;
		}
		
		int minType = 0;
		for (int i = 0;i < node::current_type_count;i++)
		{
			if (typeCount[minType] > typeCount[i])
				minType = i;
		}
		
		if (ofRandomuf() < 0.40)
		{
			addNewNode(ofVec2f(ofRandom(def::scr_w),
							   ofRandom(def::scr_h - 600)),
					   ofRandomuf() < 0.5 ? node::TYPE_BLINKER : node::TYPE_CIRCLE);
		}else{
			addNewNode(ofVec2f(ofRandom(def::scr_w),
							   ofRandom(def::scr_h - 600)), int(ofRandom(100)) % node::current_type_count);
		}
	}
	
	//定期的に接続を行う
	static int step_connection = ofRandom(10, 30);
	if ((ofGetFrameNum() % step_connection == 0))
	{
		step_connection = ofRandom(30, 300);
		
		ofPtr<node> outNode, inNode;
		ofPtr<nodeInlet> il;
		ofPtr<nodeOutlet> ol;
		bool bBreak = false;
		for (int i = 0;i < 300;i++)
		{
			ofPtr<node> tg = nodes[i % nodes.size()];
			
			if (tg->manager.inlets.size() > 0)
			{
				for (int j = 0;j < 300;j++)
				{
					il = tg->manager.inlets[j % tg->manager.inlets.size()];
					inNode = tg;
					if (!il->targ)
					{
						bBreak = true;
						break;
					}
				}
			}
			
			if (bBreak)
			{
				bBreak = false;
				break;
			}
		}
		for (int i = 0;i < 300;i++)
		{
			ofPtr<node> tg = nodes[i % nodes.size()];
			
			if (tg->manager.outlets.size() > 0)
			{
				for (int j = 0;j < 300;j++)
				{
					ol = tg->manager.outlets[j % tg->manager.outlets.size()];
					outNode = tg;
					if ((outNode != inNode) && (!ol->targ))
					{
						bBreak = true;
						break;
					}
				}
			}
			if (bBreak) break;
		}
		
		if ((!ol->targ) && (!il->targ) && (inNode != outNode))
		{
			il->targ = ol;
			ol->targ = il;
			ol->connectFrame = 0;
			cout << "Connect success :" << ofGetFrameNum() << endl;
		}else{
			cout << "Connect Failed :" << ofGetFrameNum() << endl;
			if (inNode == outNode) cout << "same Node" << endl;
			if (ol->targ) cout << "ol has targ" << endl;
			if (il->targ) cout << "il has targ" << endl;
		}
		
//		ofPtr<node> randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
//		while (randomConnect == nd)
//		{
//			randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
//		}
//		
//		if (nd->manager.outlets.size() > 0)
//		{
//			randomConnect->manager.connectTo(ofRandom(5),
//											 nd->manager.outlets[int(ofRandom(10)) % nd->manager.outlets.size()]);
//		}

	}

	
	//距離感を保つ
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
			nodes[i]->pos_base.x = ofClamp(nodes[i]->pos_base.x, 100, def::scr_w - 100);
			nodes[i]->pos_base.y = ofClamp(nodes[i]->pos_base.y, 100, def::scr_h - 600);
			nodes[j]->pos_base.x = ofClamp(nodes[j]->pos_base.x, 100, def::scr_w - 100);
			nodes[j]->pos_base.y = ofClamp(nodes[j]->pos_base.y, 100, def::scr_h - 600);
			
		}
	}
}

void nodeManager::draw()
{
	for (auto it : nodes)
	{
		it->draw();

//参照カウント
//		ofVec2f tp = it->pos_base + ofVec2f(50, 50);
//		ofSetColor(255);
//		ofDrawBitmapString(ofToString(it.use_count()), tp.x ,tp.y);
		
	}
	
	int targStep = 100;
	
	int tgs = int(ofRandom(100)) % 5;
	static int lastFrame = 0;
	if (((ofGetFrameNum() % targStep) == 0) &&
		(ofGetFrameNum() - lastFrame) > 300)
	{
		lastFrame = ofGetFrameNum();
		targStep = ofRandom(100, 600);
		int tp;
		if (tgs == 0) tp = node::TYPE_AGILE;
		if (tgs == 1) tp = node::TYPE_ARM;
		if (tgs == 2) tp = node::TYPE_ARM;
		if (tgs == 3) tp = node::TYPE_POP_A;
		if (tgs == 4) tp = node::TYPE_POP_B;
		
		ofPtr<node> nd;
		for (int j = 0;j < 100;j++)
		{
			nd = nodes[int(ofRandom(100)) % nodes.size()];
			if ((nd->type == tp) &&
				(!nd->hwConnected))
			{
				if (tgs == 0) sys03node.setTarget(nd);
				if (tgs == 1) sys04node_c.setTarget(nd);
				if (tgs == 2) sys04node_p.setTarget(nd);
				if (tgs == 3) sys06node_a.setTarget(nd);
				if (tgs == 4) sys06node_b.setTarget(nd);
				break;
			}
		}
		
	}
	
	vector<ofVec2f> vec;
	for (int i = 0;i <= 360;i+=90)
	{
		vec.push_back(ofVec2f(cos(ofDegToRad(i)) * 145,
							  sin(ofDegToRad(i)) * 145));
		vec.push_back(ofVec2f(cos(ofDegToRad(i)) * 140,
							  sin(ofDegToRad(i)) * 140));
		
	}
	for (int i = 0;i < 5;i++)
	{
		ofPushMatrix();
		if (i == 0) ofTranslate(sys03node.sys03HWPos);
		if (i == 1) ofTranslate(sys04node_c.HWPos);
		if (i == 2) ofTranslate(sys04node_p.HWPos);
		if (i == 3) ofTranslate(sys06node_a.HWPos);
		if (i == 4) ofTranslate(sys06node_b.HWPos);
		
		ofSetColor(80);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &vec[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vec.size());
		glDisableClientState(GL_VERTEX_ARRAY);

		ofPopMatrix();
	}
}

void nodeManager::addNewNode(ofVec2f pos, int type)
{
	ofPtr<node> nd = make_shared<node>();
	nd->setup(pos, type);
	nodes.push_back(nd);
	
	
//	if (nodes.size() > 3)
//	{
//		for (int i = 0;i < 10;i++)
//		{
//			ofPtr<node> randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
//			while (randomConnect == nd)
//			{
//				randomConnect = nodes[int(ofRandom(10000)) % nodes.size()];
//			}
//			
//			if (nd->manager.outlets.size() > 0)
//			{
//				randomConnect->manager.connectTo(ofRandom(5),
//												 nd->manager.outlets[int(ofRandom(10)) % nd->manager.outlets.size()]);
//			}
//		}
//	}
}
