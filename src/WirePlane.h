/*
 * WirePlane.h
 *
 *  Created on: Apr 22, 2016
 *      Author: vuk
 */

#ifndef WIREPLANE_H_
#define WIREPLANE_H_

#include "Object3D.h"

class WirePlane: public Object3D
{
public:
	WirePlane();
	WirePlane(btTransform argT);
	virtual void render();
	virtual ~WirePlane();
};

#endif /* WIREPLANE_H_ */
