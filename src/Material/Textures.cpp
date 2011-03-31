/*
 * Copyright © 2010 Lubosz Sarnecki
 * Textures.cpp
 *
 *  Created on: Oct 4, 2010
 */
#include <math.h>
#include <vector>
#include <string>
#include "Material/Textures.h"
#include "System/GUI.h"
#include "System/Config.h"
#include "System/Logger.h"
#include <QImage>
#include <QDebug>

DepthTexture::DepthTexture(GLuint width, GLuint height, string name,
        GLenum glId) {
	glError;
    this->name = name;
    this->glId = glId;
    textureType = GL_TEXTURE_2D;

    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);
    Logger::Instance().message << "Creating FBO Depth texture #" << texture
            << " " << name;
    Logger::Instance().log("DEBUG", "DepthTexture");

    /*
     Shadowmap
     GL_LINEAR does not make sense for depth texture.
     However, next tutorial shows usage of GL_LINEAR and PCF

     */
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specifies the texture comparison mode for currently bound depth textures.
    // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
            GL_NONE
     );

    // shadowmap
    // No need to force GL_DEPTH_COMPONENT24,
    // drivers usually give you the max precision if available
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glBindTexture(textureType, 0);
}

ShadowTexture::ShadowTexture(GLuint width, GLuint height, string name,
        GLenum glId) {
  glError;
    this->name = name;
    this->glId = glId;
    textureType = GL_TEXTURE_2D;

    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);
    Logger::Instance().message << "Creating FBO Shadow texture #" << texture
            << " " << name;
    Logger::Instance().log("DEBUG", "ShadowTexture");

    /*
     Shadowmap
     GL_LINEAR does not make sense for depth texture.
     However, next tutorial shows usage of GL_LINEAR and PCF
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Specifies the texture comparison mode for currently bound depth textures.
    // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
            GL_COMPARE_REF_TO_TEXTURE
     );

    // Specifies the comparison operator used when GL_TEXTURE_COMPARE_MODE is
    // set to GL_COMPARE_REF_TO_TEXTURE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Remove artefact on the edges of the shadowmap
     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
     glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );



    // shadowmap
    // No need to force GL_DEPTH_COMPONENT24,
    // drivers usually give you the max precision if available
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glBindTexture(textureType, 0);
}

ColorTexture::ColorTexture(GLuint width, GLuint height, string name,
        GLenum glId) {
    this->name = name;
    this->glId = glId;
    textureType = GL_TEXTURE_2D;

    glGenTextures(1, &texture);
    Logger::Instance().message << "Creating FBO Color texture #" << texture
            << " " << name;
    Logger::Instance().log("DEBUG", "ColorTexture");
    glBindTexture(textureType, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
            GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);

    // FBO

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0);
    glBindTexture(textureType, 0);
}

TextureFile::TextureFile(string filename, GLenum glId, string name) {
    textureType = GL_TEXTURE_2D;

    this->glId = glId;
    this->name = name;

    string path = Config::Instance().value<string> ("textureDir") + filename;

    glGenTextures(1, &texture);
    Logger::Instance().message << "Creating texture from file #" << texture
            << " " << name;
    Logger::Instance().log("DEBUG", "TextureFile");

    glBindTexture(GL_TEXTURE_2D, texture);
    /*
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

#if USE_FREEIMAGE
    readFreeImage(GL_TEXTURE_2D,path);
#else
    readQImage(GL_TEXTURE_2D,path);
#endif

    glBindTexture(textureType, 0);
}

TextureQImage::TextureQImage(QImage * image, GLenum glId, string name) {
    textureType = GL_TEXTURE_2D;

    this->glId = glId;
    this->name = name;

    glGenTextures(1, &texture);
    Logger::Instance().message << "Creating texture from qimage #" << texture
            << " " << name;
    Logger::Instance().log("DEBUG", "QImage Texture");

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    loadQImage(GL_TEXTURE_2D,image);

    glBindTexture(textureType, 0);
}

SplatTexture::SplatTexture(GLenum glId, string name, int resolution) {
    this->glId = glId;
    this->name = name;

    unsigned char* data = createGaussianMap(resolution);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
#ifndef USE_GL3
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
#else
    glTexParameteri(GL_TEXTURE_2D, 0, GL_TRUE);
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resolution, resolution, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
}

/**
 * EvalHermite(float pA, float pB, float vA, float vB, float u)
 * @brief Evaluates Hermite basis functions for the specified coefficients.
 */
inline float SplatTexture::evalHermite(float pA, float pB, float vA, float vB,
        float u) {
    float u2 = (u * u), u3 = u2 * u;
    float B0 = 2 * u3 - 3 * u2 + 1;
    float B1 = -2 * u3 + 3 * u2;
    float B2 = u3 - 2 * u2 + u;
    float B3 = u3 - u;
    return (B0 * pA + B1 * pB + B2 * vA + B3 * vB);
}

unsigned char* SplatTexture::createGaussianMap(int N) {
    float *M = new float[2 * N * N];
    unsigned char *B = new unsigned char[4 * N * N];
    float X, Y, Y2, Dist;
    float Incr = 2.0f / N;
    int i = 0;
    int j = 0;
    Y = -1.0f;
    // float mmax = 0;
    for (int y = 0; y < N; y++, Y += Incr) {
        Y2 = Y * Y;
        X = -1.0f;
        for (int x = 0; x < N; x++, X += Incr, i += 2, j += 4) {
            Dist = static_cast<float>(sqrtf(X * X + Y2));
            if (Dist > 1)
                Dist = 1;
            M[i + 1] = M[i] = evalHermite(1.0f, 0, 0, 0, Dist);
            B[j + 3] = B[j + 2] = B[j + 1] = B[j]
                    = (unsigned char) (M[i] * 255);
        }
    }
    delete[] M;
    return (B);
}

CubeTextureFile::CubeTextureFile(string filename, GLenum glId, string name) {
    this->glId = glId;
    this->name = name;

    textureType = GL_TEXTURE_CUBE_MAP;

    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);

    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    string textureDir = Config::Instance().value<string> ("textureDir");
    vector<string> suffixes = Config::Instance().values<string> ("suffixes");

    for (int face = 0; face < 6; face++) {
      //TODO: jpeg hardcoded
      string path = textureDir + filename + suffixes[face] + ".jpg";

        glBindTexture(textureType, texture);
#if USE_FREEIMAGE
    readFreeImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,path);
#else
    readQImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,path);
#endif

//        QImage image;
//        image.load(path);
//
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA,
//                image.width(), image.height(), 0, GL_BGRA,
//                GL_UNSIGNED_BYTE, image.bits());
    }
}
