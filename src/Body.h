/*
 * Body.h
 *
 *  Created on: May 28, 2016
 *      Author: vuk
 */

#ifndef BODY_H_
#define BODY_H_

#include "ModelCallList.h"

class btRigidBody;

class Body: public ModelCallList
{
public:
	btRigidBody *body;
	Body();
	Body(AssimpModel &argAssimp);
	Body(unsigned int argIdCallList);
	virtual void init();
	virtual ~Body();
};

#endif /* BODY_H_ */
