/*
 * Copyright © 2010 Lubosz Sarnecki
 * Geometry.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "Mesh.h"

class Geometry {
 public:
	static Mesh * makeTetrahedron();
	static Mesh * makeCube();
	static Mesh * makePlane();
	static Mesh * makePlaneTess();
	static Mesh * makeStars(
	        vector<float> & resolution,
	        float density,
	        float randomness,
	        float colorIntensity
	);
	static Mesh * makeSpiral(int resolution);
	static Mesh * gluSphere(GLdouble radius, GLint slices, GLint stacks);
 private:
	float density, randomness;
	int variation;
	static float randomize(float density, float randomness);
};
