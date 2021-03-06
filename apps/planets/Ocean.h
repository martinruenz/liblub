/*
 * Ocean.h
 *
 *  Created on: May 5, 2011
 *      Author: bmonkey
 */

#ifndef OCEAN_H_
#define OCEAN_H_

#include "Scene/Node.h"
#include "Material/Material.h"
#include "PlanetElement.h"

class Ocean : public PlanetElement{
public:
  Material * ocean;
  Ocean(Planet * planet);
  virtual ~Ocean();
  void setOceanUniforms(ShaderProgram * program);
  void init();
  void draw();
};

#endif /* OCEAN_H_ */
