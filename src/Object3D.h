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
protected:
	float m[16];
public:
	btTransform t;
public:
	Object3D();
    Object3D(btTransform argT);
    virtual void init(){};
    virtual void update();
    virtual void render();
	virtual ~Object3D();
	virtual void setM();
	virtual const float* getM() const { return m;}
	virtual const float* mt() { t.getOpenGLMatrix(m); return m; }
};
#endif /* OBJECT3D_H_ */
