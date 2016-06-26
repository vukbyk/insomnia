/*
 * Sun.h
 *
 *  Created on: Jun 25, 2016
 *      Author: vuk
 */

#ifndef SUN_H_
#define SUN_H_

#include "Object3D.h"

class Sun: public Object3D
{
public:
	Sun();
    virtual void render();
	virtual ~Sun();
};

#endif /* SUN_H_ */
