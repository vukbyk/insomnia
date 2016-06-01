/*
 * Body.cpp
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#include "Body.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>


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
//	body = new btRigidBody(10,new btDefaultMotionState(),new btSphereShape(1.f));

	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();
	fileLoader->loadFile("models/all.bullet");
	btCollisionObject* obj = fileLoader->getRigidBodyByIndex(2);
	body = btRigidBody::upcast(obj);
	body->setActivationState(DISABLE_DEACTIVATION);
	for(int i=0; i< fileLoader->getNumRigidBodies(); i++)
	{
		cout<<fileLoader->getNumRigidBodies()<<": " <<fileLoader->getNameForPointer(fileLoader->getRigidBodyByIndex(i))<<endl;
		cout<<fileLoader->getRigidBodyByIndex(i)->getCollisionShape()->getName()<<endl;
	}
//	cout<<fileLoader->getNameForPointer(fileLoader->getRigidBodyByIndex(2))<<endl;
	//	body->setUserPointer(this);
	delete t;
	delete tm;
	t=tm=&body->getInterpolationWorldTransform();
}

Body::~Body()
{
	// TODO Auto-generated destructor stub
}

