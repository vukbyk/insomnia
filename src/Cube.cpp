/*
 * Cube.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: vuk
 */

#include "Cube.h"

Cube::Cube()
{
//	t.setIdentity();
//	t.setOrigin(btVector3(0,0,-10));
}

Cube::Cube(btTransform argT) : Object3D(argT)
{
}

void Cube::render()
{
//	glPushMatrix();
////	glLoadIdentity();
////	glLoadMatrixf(mt());
//	glMultMatrixf(mt());
//	glColor3f(1.8,.1,.08);
//	float span=10;
//	glLineWidth(3);
//	glBegin(GL_LINES);
//		glNormal3f(0,1,0);
//		glVertex3f(-span, 0, 0);
//		glVertex3f(span, 0, 0);
//		glVertex3f(0, 0,-span);
//		glVertex3f(0, 0,span);
//
//		glVertex3f(-span, 0, span);
//		glVertex3f(span, 0, span);
//		glVertex3f(-span, 0, -span);
//		glVertex3f(span, 0, -span);
//		glVertex3f(-span, 0, -span);
//		glVertex3f(-span, 0, span);
//		glVertex3f(span, 0, -span);
//		glVertex3f(span, 0, span);
//	glEnd();
//	glLineWidth(1);
//
//	glBegin(GL_LINES);
//		for(int i=1; i<span; i++)
//		{
//			glVertex3f(-span, 0, i);
//			glVertex3f(span, 0, i);
//			glVertex3f(-span, 0, -i);
//			glVertex3f(span, 0, -i);
//			glVertex3f(i, 0, -span);
//			glVertex3f(i, 0, span);
//			glVertex3f(-i, 0, -span);
//			glVertex3f(-i, 0, span);
//		}
//	glEnd();
//	glPopMatrix();

//	setM();
//	glMultMatrixf(getM());
	glPushMatrix();
//	glLoadIdentity();
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
   glPopMatrix();
   Object3D::render();
}

Cube::~Cube()
{
	// TODO Auto-generated destructor stub
}

