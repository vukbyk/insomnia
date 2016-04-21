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
private:
	float m[16];
public:
	btTransform t;
public:
	Object3D();
    Object3D(btTransform argT);
//    void init();
    void update();
    virtual void render();
	virtual ~Object3D();
	virtual void setM();
	const float* getM() const
	{
		return m;
	}
};

#endif /* OBJECT3D_H_ */
