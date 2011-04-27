/*
 * Copyright © 2010 Lubosz Sarnecki
 * Shader.cpp
 *
 *  Created on: Mar 22, 2010
 */

#include "Material/Shader.h"

#include <QtCore>
#include <string>
#include <vector>
#include "System/Logger.h"
#include "System/Config.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderEngine.h"

Shader::Shader(string fileName, GLenum type, bool useTemplate) {
  LogDebug << "Creating Shader" << fileName;
  this->fileName = fileName;
  this->type = type;

  if (useTemplate)
    loadTemplate();
  else
    loadSource();
  compile();
}

Shader::Shader(string fileName, GLenum type, const vector<string> & defines) {
  LogDebug << "Creating Shader" << fileName;
  this->fileName = fileName;
  this->type = type;
  this->defines = defines;

  loadSource();
  compile();
}

Shader::~Shader() {
  glDeleteShader(shader);
  free(source);
}


void Shader::loadSource() {
  /* Read our shaders into the appropriate buffers */
  source = readFile(Config::Instance().value<string>("shaderDir") + fileName);

    /* Assign our handles a "name" to new shader objects */
    shader = glCreateShader(type);

    if (defines.size() > 0) {
      // set defines
      string defineString = "";

      foreach(string define, defines) {
        defineString += "#define " + define + "\n";
        LogDebug << "Shader Flags" << define;
      }
      const GLchar *sources[2] = { defineString.c_str(), source };
      glShaderSource(shader, 2, sources, NULL);
    } else {
      glShaderSource(shader, 1,  (const GLchar**)&source, NULL);
    }
}

void Shader::loadTemplate() {
  string shaderSource = TemplateEngine::Instance().render(fileName).toStdString();
//  printf("%s:\n\n %s\n", fileName.c_str(), shaderSource.c_str());
  source = const_cast<GLchar *>(shaderSource.c_str());
  /* Assign our handles a "name" to new shader objects */
  shader = glCreateShader(type);
  /* Set rendered template string as source */
  glShaderSource(shader, 1,  (const GLchar**)&source, NULL);
}

void Shader::compile() {
  LogDebug << "Compiling Shader#" << shader << "...";
  /* Compile our shader objects */
  // TODO(bmonkey): driver crashes :/
  glCompileShader(shader);
  printShaderInfoLog(shader);
  glError;
}

/* A simple function that will read a file
 * into an allocated char pointer buffer */
char* Shader::readFile(string filePath) {
    FILE *file;
    char *buffer;
    int64_t fileSize;

    file = fopen(filePath.c_str(), "r"); /* Open file for reading */
    if (!file)
        LogError << "File error: "<< filePath;

    // obtain file size:
    fseek(file, 0, SEEK_END); /* Seek to the end of the file */
    fileSize = ftell(file); /* Find out how many bytes into the file we are */
    rewind(file); /* Go back to the beginning of the file */

    /* Allocate a buffer for the entire
     * length of the file plus a null terminator */
    buffer = reinterpret_cast<char*>(malloc(sizeof(buffer) * fileSize));
    if (!buffer) {
        fputs("Memory error", stderr);
        exit(2);
    }

    /* Read the contents of the file in to the buffer */
    fread(buffer, fileSize, 1, file);

    /*
     // TODO(bmonkey): Occurs every time
     if (result != fileSize){
     cout << "File:" << filePath << "\n";
     fputs ("Reading error",stderr); exit (3);
     }
     */

    fclose(file); /* Close the file */
    buffer[fileSize] = 0; /* Null terminator */

    return buffer; /* Return the buffer */
}

void Shader::printShaderInfoLog(GLuint shader) {
  int infologLen = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 1) {
    GLchar * infoLog = reinterpret_cast<GLchar*>(malloc(infologLen));
    if (infoLog == NULL) {
      LogError << "Could not allocate InfoLog buffer";
    }
    int charsWritten = 0;
    glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
    string shaderlog = infoLog;
    free(infoLog);
    LogError << "Shader Log"<< shaderlog;
  } else {
    LogDebug << "Success";
  }
}

GLuint Shader::getReference() const {
  return shader;
}
