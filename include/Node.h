/*
 * Node.h
 *
 *  Created on: Jun 28, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"
#include "Material.h"

class Node {
private:
	string name;
	vector<float> position;
	Node * parent;
	Mesh * mesh;
	Material * material;


public:
	Node(string name, vector<float> position, Mesh * mesh);
	Node(string name, vector<float> position, Mesh * mesh, Material * material);
	virtual ~Node();

    void setMesh(Mesh *mesh);
    void setPosition(vector<float> position);
	void draw();
    string getName() const;
    vector<float> getPosition() const;
    void setName(string name);
    Material *getMaterial() const;
    void setMaterial(Material *material);
};
