/*
 * WirePlane.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: vuk
 */

#include "WirePlane.h"

WirePlane::WirePlane()
{
	// TODO Auto-generated constructor stub
}

WirePlane::WirePlane(btTransform argT) : Object3D(argT)
{
}

void WirePlane::render()
{
	glPushMatrix();
		glMultMatrixf(getM());
		glColor3f(.8,.8,.8);
		float span=10;
		glLineWidth(3);
		glBegin(GL_LINES);
			glNormal3f(0,1,0);
			glVertex3f(-span, 0, 0);
			glVertex3f(span, 0, 0);
			glVertex3f(0, 0,-span);
			glVertex3f(0, 0,span);

			glVertex3f(-span, 0, span);
			glVertex3f(span, 0, span);
			glVertex3f(-span, 0, -span);
			glVertex3f(span, 0, -span);
			glVertex3f(-span, 0, -span);
			glVertex3f(-span, 0, span);
			glVertex3f(span, 0, -span);
			glVertex3f(span, 0, span);
		glEnd();
		glLineWidth(1);

		glBegin(GL_LINES);
			for(int i=1; i<span; i++)
			{
				glVertex3f(-span, 0, i);
				glVertex3f(span, 0, i);
				glVertex3f(-span, 0, -i);
				glVertex3f(span, 0, -i);
				glVertex3f(i, 0, -span);
				glVertex3f(i, 0, span);
				glVertex3f(-i, 0, -span);
				glVertex3f(-i, 0, span);
			}
		glEnd();
	glPopMatrix();
}

WirePlane::~WirePlane()
{
	// TODO Auto-generated destructor stub
}

