/*
 * GUI.cpp
 *
 *  Created on: Mar 6, 2011
 *      Author: bmonkey
 */

#include "System/GUI.h"

#include <sstream>
#include <string>
#include <QSize>
#include <QRectF>
#include <QDebug>
#include <QMap>

#include "Material/Materials.h"
#include "Material/Textures.h"
#include "Scene/SceneData.h"
#include "Mesh/Geometry.h"
#include "Window/MediaLayer.h"
#include "System/Logger.h"
#include "System/Timer.h"

GUI::GUI() {
  textLines = QMap<string,string>();
  QList<string> attributes;
  attributes.push_back("uv");
  material = new Simple("Texture/font",attributes);

  node = new Node("GUI", { 0, 0, 0 }, 1, Geometry::plane(attributes, QRectF(0.5,0,0.5,1)), material);
  screenSize = QSize(480,600);
  textBox = QRectF(0,0,screenSize.width(), screenSize.height());
  image = new QImage(screenSize, QImage::Format_ARGB32);

  //clean alpha channel
  black = QImage(screenSize, QImage::Format_Mono);
  QPainter blackPainter(&black);
  blackPainter.fillRect ( textBox, Qt::black );
  blackPainter.end();

  clear();
  drawBox = QRectF(50,200,screenSize.width()-100, screenSize.height()-100);
}

GUI::~GUI() {
  // TODO Auto-generated destructor stub
}
void GUI::init() {
  addText("ascene", SceneData::Instance().name);
  addText("fps", "FPS");
  addText("cam", "Cam");
  addText("time", "Time");
  render();
  texture = new TextureQImage(image,"myTexture");
  material->addTexture(texture);
}

void GUI::update() {
  std::stringstream fps, cam, zoom, time;
  fps << "FPS: " << Timer::Instance().getFPS();
  time << "ms: " << Timer::Instance().getSPF();
  updateText("fps",fps.str());
  cam << "\nCam:\n" << SceneData::Instance().getCurrentCamera()->position.x()
      <<"\n " << SceneData::Instance().getCurrentCamera()->position.y()
      <<"\n " << SceneData::Instance().getCurrentCamera()->position.z();
  updateText("fps",fps.str());
  updateText("cam",cam.str());
  updateText("time",time.str().substr (0,12));
  clear();
  render();
  texture->bind();
  texture->loadQImage(image);
}

void GUI::clear() {
  image->setAlphaChannel(black);
}

void GUI::render() {
  QPainter fontPainter(image);
  //draw font
  fontPainter.setRenderHint(QPainter::Antialiasing, true);
  fontPainter.setRenderHint(QPainter::TextAntialiasing, true);
  fontPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  fontPainter.setFont(QFont("Ubuntu", 24));
  fontPainter.setLayoutDirection(Qt::RightToLeft);
  fontPainter.setPen(Qt::white);

  string text;
  foreach(string line, textLines) {
    text += line + "\n";
  }

  fontPainter.drawText(drawBox, Qt::AlignLeft, QString::fromStdString(text));
  fontPainter.end();

}

void GUI::addText(string id, string value){
  textLines.insert(id, value);
}
void GUI::updateText(string id, string value) {
  textLines[id] = value;
}

void GUI::draw() {
  glError;
  glEnable(GL_BLEND);
  ShaderProgram * shaderProgram = material->getShaderProgram();
  shaderProgram->use();
  shaderProgram->setUniform("MVPMatrix",QMatrix4x4());
  node->draw();
  glDisable(GL_BLEND);
  glError;
}
