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
	buffer.allocate(def::scr_w, def::scr_h, GL_RGBA);
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
