/*
 * Object3D.h
 *
 *  Created on: Apr 10, 2016
 *      Author: vuk
 */

#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include <GL/gl.h>
#include <bullet/LinearMath/btTransform.h>


class Object3D
{
public:
	btTransform t;
public:
	Object3D();
    Object3D(btTransform argT);
    virtual void render();
	virtual ~Object3D();
};

#endif /* OBJECT3D_H_ */
