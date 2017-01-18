#include "ofApp.h"

void ofApp::setup()
{
	disp.setup();
	nodeMn.setup();
	
	gui.setup();
	gui.add(singleView.set("singleView", true));
	isDebug = false;
	
	cam.setPosition(def::scr_w / 2.0, def::scr_h/2.0, 1000);
	cam.setVFlip(true);
	cam.enableOrtho();
}

void ofApp::update()
{
	nodeMn.update();
	
	if (ofGetFrameNum() < 30)
	{
		if (ofGetFrameNum() % 2 == 0)
		{
			nodeMn.addNewNode(ofVec2f(ofRandomuf() * def::scr_w,
									  ofRandomuf() * def::scr_h), 0);
		}
	}
}

void ofApp::draw()
{
	ofClear(0);
	
	disp.begin();
	
	ofClear(0, 255);

	cam.begin();
	ofPushMatrix();
	ofTranslate(0, 0);
	
	nodeMn.draw();
	
	for (int x = 0;x <= def::scr_w; x+=200)
	{
		for (int y = 0;y<= def::scr_h; y+=200)
		{
			ofSetColor(255);
			int sc = 3;
			ofDrawLine(x-sc, y-sc, x+sc, y+sc);
			ofDrawLine(x+sc, y-sc, x-sc, y+sc);
		}
	}

	ofPopMatrix();
	cam.end();
	disp.end();
	
	disp.drawWarp();
	
	if (isDebug) gui.draw();
	
}

void ofApp::keyPressed(int key)
{
	if (key == 'd') isDebug ^= true;
	if (key == 'f') ofToggleFullscreen();
	if (key == 's')
	{
		for (int i = 0;i < 3;i++)
		{
			disp.warper[i].save("warp"+ofToString(i)+".xml");
		}
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

	nodeMn.addNewNode(ofVec2f(x, y), 0);
	
}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::mouseEntered(int x, int y){

}


void ofApp::mouseExited(int x, int y){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
