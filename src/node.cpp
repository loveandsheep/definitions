//
//  node.cpp
//  Definitions
//
//  Created by Ovis aries on 2017/01/13.
//
//

#include "node.hpp"

void node::setup(ofVec2f pos_, int tp)
{
	type		= tp;
	pos_base	= pos_;
	frame		= 0;
	isClosing	= false;
	needErase	= false;
	closeFrame	= 0;
	area_scale = ofRandom(100, 200);
	seed = ofRandomuf();
	bgColor.set(1.0, 1.0, 1.0, 0.2);
	
	if (type == TYPE_AGILE)
	{
		addInlet("pos-X");
		addInlet("pos-Y");
		addInlet("drawing");
		addOutlet("arm-A");
		addOutlet("arm-B");
		addOutlet("arm-C");
		agileEye.setup();
		area_scale = 170;
	}
	if (type == TYPE_ARM)
	{
		arm.setup();
		addInlet("pos-X", 0.5);
		addInlet("pos-Y", 0.5);
		addInlet("pos-Z", 0.5);
		
		armTarg = ofVec3f(ofMap(getInletValue("pos-X"), 0, 1, -50, 50),
						  ofMap(getInletValue("pos-Y"), 0, 1, -100, -200),
						  ofMap(getInletValue("pos-Z"), 0, 1, -50, 50));
		arm.work.setGlobalPosition(armTarg);
		arm.update();
		area_scale = 170;
	}
	
	if (type == TYPE_CIRCLE)
	{
		addInlet("speed", 0.5);
		addOutlet("sin");
		addOutlet("cos");
		area_scale = 100;
	}
	
	if ((type == TYPE_POP_A) ||
		(type == TYPE_POP_B))
	{
		addInlet("pop");
		addOutlet("pop");
		area_scale = 170;
	}

}

void node::update()
{
	if (type == TYPE_AGILE)
	{
		agileEye.update(ofVec3f((getInletValue("pos-X") - 0.5) * 300,
								(getInletValue("pos-Y") - 0.5) * 300,
								300));
	}
	if (type == TYPE_ARM)
	{
		if (ofGetFrameNum() % 120 == 0)
		{
			armTarg = ofVec3f(ofMap(getInletValue("pos-X"), 0, 1, -50, 50),
							  ofMap(getInletValue("pos-Y"), 0, 1, -100, -200),
							  ofMap(getInletValue("pos-Z"), 0, 1, -50, 50));
			
		}
		arm_ease.interpolate(armTarg, 0.03);
		arm.work.setGlobalPosition(arm_ease);
		arm.update();
	}
	
	if (type == TYPE_POP_A)
	{
		bangDelay /= 1.5;
		setOutletValue("pop", bangDelay);
		int pt = -1;
		for (int i = 0;i < 9;i++)
		{
			if (ofRandomuf() < 0.005) popStat[i] = 1;
			if (popStat[i] == 1)
			{
				popCount[i]++;
				if (pt == -1) pt = i;
				pt = (popCount[pt] < popCount[i] ? i : pt);
			}
			else popCount[i] = 0;
		}
		
		if ((ofGetFrameNum() % int(100 + seed * 100) == 0) &&
			(pt >= 0))
		{
			if (getInletValue("pop") > 0.5)
			{
				popCount[pt] = 0;
				bangDelay = 1.0;
				bangFrame[pt] = 30;
				popStat[pt] = 0;
				bangTarg = pt;
			}
		}
	}
	if (type == TYPE_POP_B)
	{
		bangDelay /= 1.5;
		setOutletValue("pop", bangDelay);
		int pt = -1;
		for (int i = 0;i < 5;i++)
		{
			if (ofRandomuf() < 0.005) popStat[i] = 1;
			if (popStat[i] == 1)
			{
				popCount[i]++;
				if (pt == -1) pt = i;
				pt = (popCount[pt] < popCount[i] ? i : pt);
			}
			else popCount[i] = 0;
		}
		
		if ((ofGetFrameNum() % int(100 + seed * 100) == 0) &&
			(pt >= 0))
		{
			if (getInletValue("pop") > 0.5)
			{
				popCount[pt] = 0;
				bangDelay = 1.0;
				bangFrame[pt] = 30;
				popStat[pt] = 0;
				bangTarg = pt;
			}
		}
	}
	
	pos = pos_base + ofVec2f(ofSignedNoise(seed * 342.31 + ofGetFrameNum() / 234.5) * 5,
							 ofSignedNoise(seed * 135.32 + ofGetFrameNum() / 256.1) * 5);
	
	frame++;
	
	setOutletValue("arm-A", ofMap(agileEye.arm[0].rootPan, -30, 30, 0.0, 1.0, true));
	setOutletValue("arm-B", ofMap(agileEye.arm[1].rootPan, -30, 30, 0.0, 1.0, true));
	setOutletValue("arm-C", ofMap(agileEye.arm[2].rootPan, -30, 30, 0.0, 1.0, true));
	
//	if (frame == 600) isClosing = true;
	
	if (isClosing) closeFrame++;
	
	if (closeFrame == 30)
	{
		manager.disconnectInlets();
		needErase = true;
	}
}

void node::draw()
{
	ofPushMatrix();
	ofTranslate(pos);
	
	ofSetColor(255);
	
	ofSetRectMode(OF_RECTMODE_CENTER);
	float time_opening	 = ofxeasing::map_clamp(frame, 0, 15, 0.0, 1.0, ofxeasing::quint::easeOut) - ofxeasing::map_clamp(closeFrame, 0, 30, 0.0, 1.0, ofxeasing::quint::easeInOut);
	float time_opening_b = ofxeasing::map_clamp(frame, 10, 40, 0.0, 1.0, ofxeasing::quint::easeInOut) - ofxeasing::map_clamp(closeFrame, 0, 15, 0.0, 1.0, ofxeasing::quint::easeInOut);
	float time_object =  ofxeasing::map_clamp(frame, 30, 60, 0.0, 1.0, ofxeasing::quint::easeInOut) - ofxeasing::map_clamp(closeFrame, 0, 15, 0.0, 1.0, ofxeasing::quint::easeInOut);
	
	ofPushMatrix();
	ofRotateZ(45 * (seed < 0.5 ? -1.0 : 1.0));
	
	ofSetColor(bgColor);
	ofDrawRectangle(0, 0, time_opening * area_scale, time_opening_b * area_scale);
	
	ofNoFill();
	ofVec2f pts[8];
	float l = 10 * time_opening_b;
	pts[0] = ofVec2f(time_opening * (area_scale + 20) / 2, time_opening_b * (area_scale + 20) / 2);
	pts[1] = pts[0] + ofVec2f(-l, 0);
	pts[2] = pts[0];
	pts[3] = pts[0] + ofVec2f(0, -l);
	
	pts[4] = ofVec2f(-time_opening * (area_scale + 20) / 2, -time_opening_b * (area_scale + 20) / 2);
	pts[5] = pts[4] + ofVec2f(l, 0);
	pts[6] = pts[4];
	pts[7] = pts[4] + ofVec2f(0, l);
	
	vector<ofVec2f> grid;
	for (int i = -time_opening * area_scale / 2.0;i < time_opening * area_scale / 2.0; i+=10)
	{
		float as = area_scale / 2.0 * (time_opening + time_opening_b) * 0.5;
		float tm = i * time_opening_b;
		grid.push_back(ofVec2f(-as, tm));
		grid.push_back(ofVec2f( as, tm));
		grid.push_back(ofVec2f( tm,-as));
		grid.push_back(ofVec2f( tm, as));
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	ofSetColor(255,100);
	glVertexPointer(2, GL_FLOAT, 0, pts);
	glDrawArrays(GL_LINES, 0, 8);
	ofSetColor(255,30);
	glVertexPointer(2, GL_FLOAT, 0, &grid[0]);
	glDrawArrays(GL_LINES, 0, grid.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	ofFill();
	
	ofPopMatrix();
	
	ofSetRectMode(OF_RECTMODE_CORNER);

	
	ofPushMatrix();
	{
		
		for (int i = 0;i < 10;i++)
		{
			bangFrame[i] = MAX(0, bangFrame[i] - 1);
		}
		
		if (type == TYPE_POP_A)
		{
			for (int x = 0;x < 3;x++)
			{
				for (int y = 0;y < 3;y++)
				{
					ofPushMatrix();
					ofTranslate((x-1) * 35, (y-1) * 35);
					ofSetColor(255, 40 + (popStat[x * 3 + y] == 1 ? 100 : 0));
					ofDrawCircle(0, 0, time_object * 12);
					
					float rad = ofxeasing::map_clamp(bangFrame[x * 3 + y], 0.0, 30.0, 1.0, 0.0,
													 ofxeasing::quint::easeIn);
					ofSetColor(255, 255 * (1.0 - rad));
					ofDrawCircle(0, 0, rad * 10 + 12);
					ofPopMatrix();
				}
			}
		}

		if (type == TYPE_POP_B)
		{
			for (int x = 0;x < 5;x++)
			{
				ofPushMatrix();
				ofTranslate((x-2)*35, 0);
				ofSetColor(255, 40 + (popStat[x] == 1 ? 100 : 0));
				ofDrawCircle(0, 0, time_object * 12);
				
				float rad = ofxeasing::map_clamp(bangFrame[x], 0.0, 30.0, 1.0, 0.0, ofxeasing::quint::easeIn);
				
				ofSetColor(255, 255 * (1.0 - rad));
				ofDrawCircle(0, 0, rad * 10 + 12);
				ofPopMatrix();
			}
		}
		
		glScaled(time_object, time_object, 1.0);
		
		if (type == TYPE_AGILE)
		{
			ofRotateY(seed < 0.5 ? 45 : -45);
			agileEye.draw();
		}
		if (type == TYPE_ARM)
		{
			ofScale(0.5, 0.5, 0.5);
			ofRotateZ(180);
			ofTranslate(0, 30, 0);
			ofRotateY(45);
			arm.draw();
		}
		if (type == TYPE_CIRCLE)
		{
			circle_phase += getInletValue("speed") / 10.0;
			ofSetColor(255);
			ofNoFill();
			ofDrawCircle(0, 0, area_scale / 2.2);
			ofFill();
			ofPushMatrix();
			ofTranslate(sin(circle_phase) * (area_scale / 2.8),
						cos(circle_phase) * (area_scale / 2.8));
			ofSetColor(0, 255, 150);
			ofDrawCircle(0, 0, 5);
			ofPopMatrix();
			
			setOutletValue("sin", ofMap(sin(circle_phase), -1.0, 1.0, 0.0, 1.0));
			setOutletValue("cos", ofMap(cos(circle_phase), -1.0, 1.0, 0.0, 1.0));
		}

	}
	ofPopMatrix();
	
	ofPopMatrix();
	
	draw_inlets();
	draw_outlets();
	
}

void node::draw_inlets()
{
	int numIl = manager.inlets.size();
	int counter = ofMap(frame, 0, 30, 0, numIl, true) - ofMap(closeFrame, 0, 30, 0, numIl, true);
	
	for (int i = 0;i < counter;i++)
	{
		ofPtr<nodeInlet> il = manager.inlets[i];
		
		float ps = (i - numIl / 2.0 + 0.5) * 15;
		il->absPos.set(pos + ofVec2f(-ps - area_scale / 2.0 * 0.7, ps - area_scale / 2.0 * 0.7));
		
		ofColor grn = ofColor(0, 255, 100);
		ofColor gry = ofColor(100, 100, 100);
		ofSetColor(gry.lerp(grn, il->param));
		
		ofPushMatrix();
		ofTranslate(il->absPos);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRotateZ(45);
		float scale = 6 + il->param * 5.0;
		ofDrawRectangle(0, 0, scale, scale);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofPopMatrix();
	}
}

void node::draw_outlets()
{
	int numOl = manager.outlets.size();
	
	int counter = ofMap(frame, 0, 30, 0, numOl, true) - ofMap(closeFrame, 0, 30, 0, numOl, true);
	for (int i = 0;i < counter;i++)
	{
		ofPtr<nodeOutlet> ol = manager.outlets[i];

		float ps = (i - numOl / 2.0 + 0.5) * 15;
		ol->absPos.set(pos + ofVec2f(-ps + area_scale / 2.0 * 0.7, ps + area_scale / 2.0 * 0.7));
		
		ofColor grn = ofColor(0, 255, 100);
		ofColor gry = ofColor(100, 100, 100);
		ofSetColor(gry.lerp(grn, ol->param));

		ofPushMatrix();
		ofTranslate(ol->absPos);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofRotateZ(45);
		float scale = 6 + ol->param * 5.0;
		ofDrawRectangle(0, 0, scale, scale);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofPopMatrix();
		
		ofSetColor(70);
		
		if (ol->targ)
		{
			ofVec2f pi = ol->targ->absPos;
			ofVec2f po = ol->absPos;
			ofVec2f pc = ofVec2f(po.x, pi.y);
			
			ofVec2f verts[4];
			vector<ofVec2f> vertVec;
			verts[0] = po;
			verts[1] = pc + (po - pc).getNormalized() * 0;
			verts[2] = pc + (pi - pc).getNormalized() * 0;
			verts[3] = pi;
			
			for (int i = 0;i < 3;i++)
			{
				vertVec.push_back(verts[i]);
				
				float length = verts[i].distance(verts[i+1]);
				ofVec2f nm = (verts[i+1] - verts[i]).getNormalized();
				
				for (float d = 0;d < length;d+=5)
				{
					vertVec.push_back(verts[i] + nm * d);
				}
			}
			
			glLineWidth(2);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, &vertVec[0]);
			float pct = ofxeasing::map_clamp(frame, 10, 90 + ofNoise(i * 34.13) * 15, 0.0, 1.0, ofxeasing::quint::easeInOut) - ofxeasing::map_clamp(closeFrame, 0, 30, 0.0, 1.0, ofxeasing::quint::easeInOut);
			glDrawArrays(GL_LINES, 0, vertVec.size() * pct);
			glDisableClientState(GL_VERTEX_ARRAY);
			glLineWidth(1);
			
		}
	}
	
}

float node::getInletValue(string name)
{
	for (int i = 0;i < manager.inlets.size();i++)
	{
		ofPtr<nodeInlet> tg = manager.inlets[i];
		if (tg->label == name) return tg->param;
	}
	
	return 0.0;
}

void node::setOutletValue(string name, float param)
{
	ofPtr<nodeOutlet> tg;
	for (int i = 0;i < manager.outlets.size();i++)
	{
		ofPtr<nodeOutlet> t = manager.outlets[i];
		if (t->label == name)
		{
			tg = t;
			break;
		}
	}
	
	if (tg)
	{
		tg->param = param;
		if (tg->targ) tg->targ->param = param;
	}
}

void node::addInlet(string label, float def)
{
	ofPtr<nodeInlet> il = ofPtr<nodeInlet>(new nodeInlet);
	il->label = label;
	il->param = def;
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
		index = index % inlets.size();
		inlets[index]->disConnect();
	}
}

void nodeIOManager::disconnectOutlets(int index)
{
	if (index == -1)
		for (auto it : outlets) it->targ.reset();
	else
	{
		index = index % outlets.size();
		outlets[index]->targ.reset();
	}

}

void nodeIOManager::connectTo(int index, ofPtr<nodeOutlet> out)
{
	if (inlets.size() == 0) return;
	index = index % inlets.size();
	if (!inlets[index]->targ)
	{
		inlets[index]->targ = out;
		out->targ = inlets[index];
	}
}

void nodeInlet::disConnect()
{
	if (targ) targ->targ.reset();
	targ.reset();
}
