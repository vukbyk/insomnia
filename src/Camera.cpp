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

void Camera::update()
{
	if(!updated)
	{
		*tm=*t;
		if(parent)
		{
			if(!parent->updated)
				parent->update();


			btVector3 forwardOffset=parent->tm->getBasis()* t->getOrigin();
			tm->setOrigin(parent->tm->getOrigin() + forwardOffset);
			tm->setRotation(parent->tm->getRotation() * t->getRotation());

//			btVector3 z=tm->getBasis()*btVector3(0,0,1);
//			btVector3 x=tm->getBasis()*btVector3(1,0,0);
//			btVector3 y=tm->getBasis()*btVector3(0,1,0);
//
//			btVector3 zc=z.angle(tm->);
//			btVector3 x=parent->tm->getBasis()*btVector3(1,0,0);
//			btVector3 y=parent->tm->getBasis()*btVector3(0,1,0);
//			btVector3 forward=parent->tm->getBasis()*btVector3(0,0,1);
//			btVector3 upLocal=parent->tm->getBasis() * btVector3(0,1,0);
//			forward.angle()
//			btVector3 planetUp=parent->tm->getOrigin().normalized();
//			tm->setRotation(btQuaternion(forwardOffset,0));
//			tm->setRotation(parent->tm->getRotation() * up * t->getRotation());
//			float pitch=45* SIMD_RADS_PER_DEG;
//			float roll=0;

		}
		setM();
		updated=true;
	}

//	setM();
//	if(objects)
//	    for(auto &obj: *objects)
//	    {
//	      obj->update();
//	    }
////	Object3D::update();
}

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

