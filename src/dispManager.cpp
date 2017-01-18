//
//  dispManager.cpp
//  Definitions
//
//  Created by Ovis aries on 2016/12/31.
//
//

#include "dispManager.hpp"

void dispManager::setup()
{
	ofFbo::Settings setting;
	setting.width = def::scr_w;
	setting.height = def::scr_h;
	setting.depthStencilAsTexture = false;
	setting.internalformat = GL_RGBA;
	setting.minFilter = GL_LINEAR;
	setting.maxFilter = GL_LINEAR;
	setting.numSamples = 4;
	setting.useDepth = true;
	setting.useStencil = true;

	buffer.allocate(setting);
	fx.setup(&buffer, setting);
	
	for (int i = 0;i < 3;i++)
	{
		warper[i].setSourceRect(ofRectangle(i * 1920, 0, 1920, 1080));
		warper[i].setTargetRect(ofRectangle(i * 1920 + 400, 500, 600, 500));
		warper[i].setup();
		warper[i].load("warp"+ofToString(i)+".xml");
		warper[i].enableMouseControls();
	}
}

void dispManager::end()
{
	buffer.end();
	
	fx.getfxUnit(KSMR_FRAGFX_NOISE)->u_Volume = 0.00;
	fx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable = true;
	fx.getfxUnit(KSMR_FRAGFX_FRINGE)->u_Volume = 0.002;
	fx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable = false;
//	fx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable = ofRandomuf() < 0.03;
//	fx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable = ofRandomuf() < 0.002;
//	fx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable = ofRandomuf() < 0.002;
	
	
	fx.applyFx();
}

void dispManager::drawWarp()
{
	for (int i = 0;i < 3;i++)
	{
		ofMatrix4x4 mat = warper[i].getMatrix();
		ofPushMatrix();
		ofMultMatrix(mat);
		buffer.getTexture().bind();
		ofVec2f vt[4];
		vt[0] = warper[i].getSourcePoints()[0];
		vt[1] = warper[i].getSourcePoints()[1];
		vt[2] = warper[i].getSourcePoints()[3];
		vt[3] = warper[i].getSourcePoints()[2];
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vt);
		glTexCoordPointer(2, GL_FLOAT, 0, vt);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		buffer.getTexture().unbind();
		ofPopMatrix();
		
		if (ofGetKeyPressed('w'))
		{
			warper[i].drawQuadOutline();
			warper[i].enableMouseControls();
		}
		else
		{
			warper[i].disableMouseControls();
		}
	}
}

void dispManager::draw(int x, int y, int w, int h)
{
	buffer.draw(x, y, w, h);
}

void dispManager::drawCenter(int x, int y, int w, int h)
{
	buffer.draw(x - w / 2.0, y - h / 2.0, w, h);
}

void dispManager::draw(ofRectangle src, ofRectangle dst)
{
	GLfloat src_[] = {	src.x, src.y,
						src.x + src.width, src.y,
						src.x, src.y + src.height,
						src.x + src.width, src.y + src.height,
	};
	GLfloat	dst_[] = {	dst.x, dst.y,
						dst.x + dst.width, dst.y,
						dst.x, dst.y + dst.height,
						dst.x + dst.width, dst.y + dst.height,
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer		(2	, GL_FLOAT, 0, dst_);
	glTexCoordPointer	(2, GL_FLOAT, 0, src_);
	
	buffer.getTexture().bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	buffer.getTexture().unbind();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
