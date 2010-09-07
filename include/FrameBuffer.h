/*
 * FrameBuffer.h
 *
 *  Created on: Aug 31, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Texture.h"
#include "Material.h"

class FrameBuffer {
public:
	FrameBuffer(GLuint width, GLuint height);
	virtual ~FrameBuffer();
    void bind();
    void unBind();
    void bindTexture();
private:
	GLuint fboId;                       // ID of FBO
	GLuint rboId;                       // ID of Renderbuffer object
	Material * fboMaterial;
	Texture * fboTexture;
	unsigned width, height;


	void printFramebufferInfo();
	string getTextureParameters(GLuint id);
	string getRenderbufferParameters(GLuint id);
	string convertInternalFormatToString(GLenum format);
	bool checkFramebufferStatus();
};
