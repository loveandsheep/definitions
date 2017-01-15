//
//  dispManager.hpp
//  Definitions
//
//  Created by Ovis aries on 2016/12/31.
//
//

#ifndef dispManager_hpp
#define dispManager_hpp

#include "ofMain.h"
#include "difinitions_const.h"
#include "ofxKsmrFragmentFx.h"

class dispManager{
public:
	
	void setup();
	void begin(){buffer.begin();}
	void end();
	void draw(int x, int y, int w, int h);
	void drawCenter(int x, int y, int w, int h);
	void draw(ofRectangle src, ofRectangle dst);
	ofFbo buffer;
	
	ofxKsmrFragmentFx fx;
};

#endif /* dispManager_hpp */
