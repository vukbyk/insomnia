/*
 * Cube.h
 *
 *  Created on: Apr 21, 2016
 *      Author: vuk
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "Object3D.h"

class Cube: public Object3D
{
public:
	Cube();
    Cube(btTransform argT);
	virtual void render();
	virtual ~Cube();
};

#endif /* CUBE_H_ */
