/*
 * Camera.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: vuk
 */

#include "Camera.h"
#include <GL/glu.h>


Camera::Camera()
{
	angle=60.;
	ratio=4.0/3.;
	near=0.1;
	far=500.0;
}

Camera::Camera(btTransform argT) : Object3D(argT)
{
	angle=60.;
	ratio=4.0/3;
	near=0.1;
	far=500.;
}

void Camera::initGL()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(angle, ratio, near, far);
}

//void Camera::update()
//{
//	setM();
//	if(objects)
//	    for(auto &obj: *objects)
//	    {
//	      obj->update();
//	    }
////	Object3D::update();
//}

void Camera::render()
{
//	glLoadIdentity();
//	glMultMatrixf(getM());
	glLoadMatrixf(getM());
	if(objects)
	    for(auto &obj: *objects)
	    {
	      obj->render();
	    }
	Object3D::render();
}

void Camera::setM()
{
//	cout<<"cet: "<<id<<endl;
	tm->inverse().getOpenGLMatrix(m);
//	tm.getOpenGLMatrix(m);
}

Camera::~Camera()
{
	// TODO Auto-generated destructor stub
}

