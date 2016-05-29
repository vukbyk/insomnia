/*
 * Body.cpp
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#include "Body.h"
#include <bullet/btBulletDynamicsCommon.h>

Body::Body()
{
	init();

}

Body::Body(AssimpModel& argAssimp) : ModelCallList(argAssimp)
{
	init();
}

Body::Body(unsigned int argIdCallList) : ModelCallList(argIdCallList)
{
	init();
}

void Body::init()
{
	body = new btRigidBody(10,new btDefaultMotionState(),new btSphereShape(1.f));
	body->setUserPointer(this);
	delete t;
	delete tm;
	t=tm=&body->getInterpolationWorldTransform();
}

Body::~Body()
{
	// TODO Auto-generated destructor stub
}

