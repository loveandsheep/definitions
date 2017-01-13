#include "ofApp.h"

void ofApp::setup()
{
	disp.setup();
}

void ofApp::update()
{
	if (ofGetKeyPressed('a')) cam.enableOrtho();
	else cam.disableOrtho();
}

void ofApp::draw()
{
	disp.begin();
	ofClear(0, 255);
	
	cam.begin();
	for (int i = -3000;i < 3000;i+=100)
	{
		ofPushMatrix();
		ofNoFill();
		ofTranslate(i, 0, -500);
		ofRotate(ofGetFrameNum() + i, 1.0, 1.2, 2.3);
		ofDrawBox(0, 0, 0, 50, 50, 50);
		ofFill();
		ofPopMatrix();
	}
	cam.end();
	
	for (int x = 0;x <= def::scr_w; x+=200)
	{
		for (int y = 0;y<= def::scr_h; y+=200)
		{
			ofSetColor(255);
			int sc = 7;
			ofDrawLine(x-sc, y, x+sc, y);
			ofDrawLine(x, y-sc, x, y+sc);
		}
	}
	disp.end();
	
	disp.draw(0, 0, ofGetWidth(), ofGetWidth() / float(def::scr_w) * def::scr_h);
	
	ofRectangle src,dst;
	float ht = ofGetWidth() / (def::scr_w) * def::scr_h;
	src.set(1920, 0, 1920, 1080);
	dst.set(0, ofGetHeight() - 540, 960, 540);
	disp.draw(src, dst);
}

void ofApp::keyPressed(int key){

}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

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
