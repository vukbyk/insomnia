/*
 * Body.cpp
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#include "Body.h"
#include "Player.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>

extern float dt;

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
	for(int i=0;i<4;i++)
		r[i]=0*SIMD_RADS_PER_DEG;
}


void Body::createVehicle(btDynamicsWorld* world, btVector3 wheelPosition)
{
	btVehicleRaycaster* vehicleRayCaster = new btDefaultVehicleRaycaster(world);
	btRaycastVehicle::btVehicleTuning tuning;
	vehicle = new btRaycastVehicle(tuning, body, vehicleRayCaster);
	vehicle->setCoordinateSystem(0,1,2);

	world->addVehicle(vehicle);
//	btVector3 v( 1.3, 1.2, 2.0);
	addWheels(tuning,  wheelPosition);
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
		if(player)
			player->update();
	//
		if(vehicle!=NULL)
			for(int i=0; i<4; i++)
			{

//				btWheelInfo& wheel = vehicle->getWheelInfo(i);
//				btVector3 relpos = wheel.m_raycastInfo.m_hardPointWS - body->getCenterOfMassPosition();
//				btVector3 vel = body->getVelocityInLocalPoint( relpos );
//				if (wheel.m_raycastInfo.m_isInContact)
//				{
//					const btTransform&   chassisWorldTransform = vehicle->getChassisWorldTransform();
//					 btVector3 fwd ( chassisWorldTransform.getBasis()*btVector3(0,0,-1));
//									 //chassisWorldTransform.getBasis()//[0],//[m_indexForwardAxis],
//									 //chassisWorldTransform.getBasis()[1],//[m_indexForwardAxis],
//									 //chassisWorldTransform.getBasis()[2]);//[m_indexForwardAxis]);
//
//					 btScalar proj = fwd.dot(wheel.m_raycastInfo.m_contactNormalWS);
//					 fwd += wheel.m_raycastInfo.m_contactNormalWS * proj;
//					 btScalar proj2 = fwd.dot(vel);
//					 wheel.m_deltaRotation = (proj2 * dt) / (wheel.m_wheelsRadius);
//					 wheel.m_rotation += wheel.m_deltaRotation;
//
//				}
//				else
//				{
//					wheel.m_rotation += wheel.m_deltaRotation;
//				}
//			    wheel.m_deltaRotation *= btScalar(.99);//damping of rotation when not in contact
//				std::cout<<i<<": "<<wheel.m_deltaRotation<<endl;

//				vehicle->updateVehicle(1/60);
//				vehicle->applyEngineForce(1.,1);
				vehicle->updateWheelTransform(i);
				vehicle->updateWheelTransformsWS(vehicle->getWheelInfo(i));
				*Body::wheel[i].t=vehicle->getWheelTransformWS(i);

//				btScalar rot=vehicle->getWheelInfo(i).m_deltaRotation*vehicle->getWheelInfo(i).m_wheelsRadius;
//				r[i]=vehicle->getWheelInfo(i).m_rotation;//vehicle->getWheelInfo(i).m_rotation;// *wheel[i].t;;
////				r[i]=fmod(r[i],SIMD_PI);
//				std::cout<<i<<": "<<r[i]<<endl;
////				rot<<endl;wheel[i].t->setOrigin(btVector3(2,2,2));
////				std::cout<<get
//				*Body::wheel[i].t=vehicle->getWheelTransformWS(i);
//				wheel[i].t->setRotation(wheel[i].t->getRotation()*btQuaternion(0, r[i], 0));
//				*wheel[i].tm=*wheel[i].t;

//				wheel[i].tm->setRotation(wheel[i].tm->getRotation()*btQuaternion(0,vehicle->getWheelInfo(i).m_deltaRotation, 0));
//				*wheel[i].t=*wheel[i].tm;
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
	btVector3 wheelAxleCS(-1, 0, 0);


//	btScalar wheelWidth(1);
	btScalar wheelRadius(.75);
	//The height where the wheels are connected to the chassis
	btScalar connectionHeight(1.4);
	btScalar suspensionRestLength(1.);
	argTuning.m_maxSuspensionForce=150000;
	argTuning.m_maxSuspensionTravelCm=90;
//	argTuning.m_suspensionCompression=40000;
//All the wheel configuration assumes the vehicle is centered at the origin and a right handed coordinate system is used
	btVector3 wheelConnectionPoint(-helpBox.x() /*- wheelRadius*/, connectionHeight, helpBox.z() /*- wheelWidth*/);
	//Adds the front wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3( 1, 1, 1),  wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, true);
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1),  wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, true);

	//Adds the rear wheels
	vehicle->addWheel(wheelConnectionPoint * btVector3( 1, 1, -1),  wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, false);
	vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, -1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, argTuning, false);

	//Configures each wheel of our vehicle, setting its friction, damping compression, etc.
	//For more details on what each parameter does, refer to the docs
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = 35;//​stiffness suspension. 10.0 ­ Offroad buggy, 	50.0 ­ Sports car, 200.0 ­ F1 Car

		wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
		wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
		//Larger friction slips will result in better handling
		wheel.m_frictionSlip = btScalar(.9);//1.2);
		wheel.m_rollInfluence = .9;//1;
	}

	for(int i=0; i<4; i++)
	{
		wheel[i]=ModelCallList(2);
		parent->addChild(&wheel[i]);
//		wheel[i].t->setOrigin(btVector3(2,2,2));
//		wheel[i].tm=const_cast<btTransform*>(&vehicle->getWheelTransformWS(i));
	}

}

Body::~Body()
{
	// TODO Auto-generated destructor stub
}

