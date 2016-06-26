/*
 * Light.h
 *
 *  Created on: Jun 23, 2016
 *      Author: vuk
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Object3D.h"

class Light: public Object3D
{
public:
	bool on=false;
	Light();
//    virtual void update();
    virtual void render();
	virtual ~Light();
};

#endif /* LIGHT_H_ */
