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

//			yaw locked camera

//			tm->setOrigin(btVector3(0,516,50));

//			btVector3 up = tm->getOrigin().normalized();
//			btVector3 forward = ( tm->getOrigin() - parent->tm->getOrigin() + parent->tm->getBasis()*btVector3(0,0,50)).normalized();
//			btVector3 right = up.cross(forward).normalized();
//			up=forward.cross(right).normalized();
//			btMatrix3x3 lookAt(
//								right.x()  , right.y()  , right.z()
//								,
//								up.x()     , up.y()     , up.z()
//								,
//							    forward.x(), forward.y(), forward.z()
//							  );
//			btQuaternion rot;
//			lookAt.getRotation(rot);
//			tm->setRotation(rot.inverse());

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

