/*
 * Body.h
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#ifndef BODY_H_
#define BODY_H_

#include "ModelCallList.h"
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>

#include <iostream>

class btRigidBody;
class btRaycastVehicle;
class btDynamicsWorld;
class btVector3;
class btRaycastVehicle;
//class btVehicleTuning;

class Body: public ModelCallList
{
public:
	btRigidBody *body;

	btRaycastVehicle* vehicle;
	ModelCallList wheel[4];
	Body(string btModelFile);
	Body(string btModelFile, AssimpModel &argAssimp);
	Body(string btModelFile, unsigned int argIdCallList);
	Body(btRigidBody *btModel, AssimpModel &argAssimp);
	Body(btRigidBody *btModel, unsigned int argIdCallList);

	virtual void init(string btModelFile);
	virtual void update();
	virtual void updateNoParent();
	void createVehicle(btDynamicsWorld* world);
	void addWheels( btRaycastVehicle::btVehicleTuning &argTuning, btVector3 &helpBox);

	virtual ~Body();
};

#endif /* BODY_H_ */
