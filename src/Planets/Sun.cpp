/*
 * Sun.cpp
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#include "Sun.h"

Sun::Sun() {
  // TODO Auto-generated constructor stub

}

Sun::~Sun() {
  // TODO Auto-generated destructor stub
}

void Sun::init(){
  QList<string> sunAttributes;
  sunAttributes.push_back("normal");
  sunAttributes.push_back("uv");
  Material * sunMaterial = new Template("perlin",sunAttributes);
  sunNode = new Node("sun", { 0,0,0 }, 1, Geometry::sphere(sunAttributes, innerRadius, 100, 50), sunMaterial);
  sunNode->setRotation(QVector3D(-90,0,180));
  perlinNoise = sunMaterial->getShaderProgram();
}
void Sun::draw(){
  float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
  perlinNoise->use();
  perlinNoise->setUniform("time", time);
  sunNode->setView(camera);
  sunNode->draw();
}
