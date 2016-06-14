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


Body::Body(string btModelFile)
{
	init(btModelFile);
}

Body::Body(string btModelFile, AssimpModel& argAssimp) : ModelCallList(argAssimp)
{
	init(btModelFile);
}

Body::Body(string btModelFile, unsigned int argIdCallList) : ModelCallList(argIdCallList)
{
	init(btModelFile);
}


Body::Body(btRigidBody* btModel, AssimpModel& argAssimp) : ModelCallList(argAssimp)
{
	vehicle=NULL;
	body = btModel;
	body->setActivationState(DISABLE_DEACTIVATION);
//	delete t;
	delete tm;
//	t=tm=&body->getInterpolationWorldTransform();
	tm=&body->getInterpolationWorldTransform();
}

Body::Body(btRigidBody* btModel, unsigned int argIdCallList) : ModelCallList(argIdCallList)
{
	vehicle=NULL;
	body = btModel;
	body->setActivationState(DISABLE_DEACTIVATION);
//	delete t;
	delete tm;
//	t=tm=&body->getInterpolationWorldTransform();
	tm=&body->getInterpolationWorldTransform();
}

void Body::init(string btModelFile)
{
//	body = new btRigidBody(10,new btDefaultMotionState(),new btSphereShape(1.f));
	vehicle=NULL;
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();
	fileLoader->loadFile(btModelFile.c_str());
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
//	delete t;
	delete tm;
//	t=tm=&body->getInterpolationWorldTransform();
	tm=&body->getInterpolationWorldTransform();
}


void Body::createVehicle(btDynamicsWorld* world)
{
	btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(world);
	btRaycastVehicle::btVehicleTuning tuning;
	vehicle = new btRaycastVehicle(tuning, body, vehicleRayCaster);
//	vehicle
	world->addVehicle(vehicle);
	btVector3 v( 1.3, 1.2, 2.0);
	addWheels(tuning,  v);
}

void Body::update()
{
	if(!updated)
	{
	//	setM();
		btVector3 g=body->getCenterOfMassPosition();
		g.normalize();
		body->setGravity(g*-10);
		(*tm * *t).getOpenGLMatrix(m);
		updated=true;
		if(objects)
			for(auto &obj: *objects)
				obj->update();
	//
		if(vehicle!=NULL)
			for(int i=0; i<4; i++)
			{

//				wheel[i].updated=true;
				btScalar rot=vehicle->getWheelInfo(i).m_deltaRotation*vehicle->getWheelInfo(i).m_wheelsRadius;
//				std::cout<<rot<<endl;
//				rot<<endl;wheel[i].t->setOrigin(btVector3(2,2,2));
//				std::cout<<get
				*wheel[i].tm=*wheel[i].t;
				*wheel[i].tm=vehicle->getWheelTransformWS(i);// *wheel[i].t;
				*wheel[i].t=*wheel[i].tm;
//				wheel[i].tm->setRotation(wheel[i].tm->getRotation()*btQuaternion(0,0,2));
//				wheel[i].t->setRotation(wheel[i].tm->getRotation()*btQuaternion(0,0,2));
//				*wheel[i].tm=*wheel[i].t;
			}

	}
}

void Body::updateNoParent()
{
	update();
}


void Body::addWheels( btRaycastVehicle::btVehicleTuning &argTuning, btVector3 &helpBox)
{
	//The direction of the raycast, the btRaycastVehicle uses raycasts instead of simiulating the wheels with rigid bodies
	btVector3 wheelDirectionCS0(0, -1, 0);

	//The axis which the wheel rotates arround
	btVector3 wheelAxleCS(1, 0, 0);

	btScalar suspensionRestLength(.5);

//	btScalar wheelWidth(1);

	btScalar wheelRadius(.75);

	//The height where the wheels are connected to the chassis
	btScalar connectionHeight(1.2);

	//All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(helpBox.x() /*- wheelRadius*/, connectionHeight, helpBox.z() /*- wheelWidth*/);

	argTuning.m_maxSuspensionForce=150000;
	argTuning.m_maxSuspensionTravelCm=50;
//	argTuning.m_suspensionCompression=40000;
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
		wheel.m_suspensionStiffness = 30;//​stiffness suspension. 10.0 ­ Offroad buggy, 	50.0 ­ Sports car, 200.0 ­ F1 Car

		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
		//Larger friction slips will result in better handling
		wheel.m_frictionSlip = btScalar(.9);//1.2);
		wheel.m_rollInfluence = .9;//1;
	}

	for(int i=0; i<4; i++)
	{
		wheel[i]=ModelCallList(2);
		parent->add(&wheel[i]);
//		wheel[i].t->setOrigin(btVector3(2,2,2));
//		wheel[i].tm=const_cast<btTransform*>(&vehicle->getWheelTransformWS(i));
	}

}

Body::~Body()
{
	// TODO Auto-generated destructor stub
}

