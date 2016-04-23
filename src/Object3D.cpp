/*
 * Object3D.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: vuk
 */

#include "Object3D.h"

#include "GL/glut.h"

Object3D::Object3D()
{
	t.setIdentity();
}

Object3D::Object3D(btTransform argT)
{
	t=argT;
}

void Object3D::render()
{
}

void Object3D::update()
{
	setM();
}

void Object3D::setM()
{
	t.getOpenGLMatrix(m);
}

Object3D::~Object3D()
{
	// TODO Auto-generated destructor stub
}

