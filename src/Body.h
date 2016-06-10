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

class btRigidBody;
class btRaycastVehicle;
class btDynamicsWorld;
class btVector3;
class btRaycastVehicle;
//class btVehicleTuning;

class Body: public ModelCallList
{
public:
	btRaycastVehicle* vehicle;
	btRigidBody *body;
	Body();
	Body(AssimpModel &argAssimp);
	Body(unsigned int argIdCallList);
	virtual void init();

	void createVehicle(btDynamicsWorld* world);
	void addWheels( btRaycastVehicle::btVehicleTuning &argTuning, btVector3 &helpBox);

	virtual ~Body();
};

#endif /* BODY_H_ */
