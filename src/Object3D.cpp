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
	setM();
	glMultMatrixf(getM());
		glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
		// Top face (y = 1.0f)
		// Define vertices in counter-clockwise (CCW) order with normal pointing out
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f( 1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f,  1.0f);
		glVertex3f( 1.0f, 1.0f,  1.0f);

		// Bottom face (y = -1.0f)
		glColor3f(1.0f, 0.5f, 0.0f);     // Orange
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);

		// Front face  (z = 1.0f)
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);

		// Back face (z = -1.0f)
		glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);

		// Left face (x = -1.0f)
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);

		// Right face (x = 1.0f)
		glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		glVertex3f(1.0f,  1.0f, -1.0f);
		glVertex3f(1.0f,  1.0f,  1.0f);
		glVertex3f(1.0f, -1.0f,  1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

//			glColor3f(0.5, 0.0 ,0);
//			glutSolidCube(.5);
//			glTranslated(1, 0, 0);
//			glutSolidCube(.5);
//			glTranslated(0, 0, 1);
//			glutSolidCube(.5);
//			glTranslated(-1, 0, 0);
//			glutSolidCube(.5);
//			glTranslated(-1, 0, 0);
//			glutSolidCube(.5);
//			glTranslated(0, 0, -1);
//			glutSolidCube(.5);
//			glTranslated(0, 0, -1);
//			glutSolidCube(.5);
//			glTranslated(1, 0, 0);
//			glutSolidCube(.5);
//			glTranslated(1, 0, 0);
//			glutSolidCube(.5);
//			glTranslated(-1, 0, -1);
//			glutSolidCube(.5);
   glPopMatrix();
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

