/*
 * Body.cpp
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#include "Body.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>


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
	vehicle=NULL;
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();
	fileLoader->loadFile("models/hull.bullet");
	btCollisionObject* obj = fileLoader->getRigidBodyByIndex(0);
	body = btRigidBody::upcast(obj);
	body->setActivationState(DISABLE_DEACTIVATION);
//	for(int i=0; i< fileLoader->getNumRigidBodies(); i++)
//	{
//		cout<<fileLoader->getNumRigidBodies()<<": " <<fileLoader->getNameForPointer(fileLoader->getRigidBodyByIndex(i))<<endl;
//		cout<<fileLoader->getRigidBodyByIndex(i)->getCollisionShape()->getName()<<endl;
//	}
//	cout<<fileLoader->getNameForPointer(fileLoader->getRigidBodyByIndex(2))<<endl;
	//	body->setUserPointer(this);
	delete t;
	delete tm;
	t=tm=&body->getInterpolationWorldTransform();
}


void Body::createVehicle(btDynamicsWorld* world)
{
	btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(world);
	btRaycastVehicle::btVehicleTuning tuning;
	vehicle = new btRaycastVehicle(tuning, body, vehicleRayCaster);
	world->addVehicle(vehicle);
	btVector3 v( 1.0, 0.5, 2.0 );
	addWheels(tuning,  v);
}

void Body::addWheels( btRaycastVehicle::btVehicleTuning &argTuning, btVector3 &helpBox)
{
	//The direction of the raycast, the btRaycastVehicle uses raycasts instead of simiulating the wheels with rigid bodies
	btVector3 wheelDirectionCS0(0, -1, 0);

	//The axis which the wheel rotates arround
	btVector3 wheelAxleCS(1, 0, 0);

	btScalar suspensionRestLength(.7);

	btScalar wheelWidth(0.2);

	btScalar wheelRadius(.35);

	//The height where the wheels are connected to the chassis
	btScalar connectionHeight(.2);

	//All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(helpBox.x() - wheelRadius, connectionHeight, helpBox.z() - wheelWidth);

//	argTuning.m_maxSuspensionForce=10000;
	argTuning.m_maxSuspensionTravelCm=35;
	argTuning.m_suspensionCompression=10000;
	//Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint,                        wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, true);
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1),  wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, true);

	//Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint* btVector3(1, 1, -1),   wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, false);
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, false);

	//Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	//For more details on what each parameter does, refer to the docs
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = 50;
		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
		//Larger friction slips will result in better handling
		wheel.m_frictionSlip = btScalar(1.2);
		wheel.m_rollInfluence = 1;
	}
}

Body::~Body()
{
	// TODO Auto-generated destructor stub
}

