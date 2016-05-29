/*
 * Camera.h
 *
 *  Created on: Apr 21, 2016
 *      Author: vuk
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Object3D.h"

class Camera: public Object3D
{
	double angle;
	double ratio;
	double near;
	double far;
public:
	Camera();
	Camera(btTransform argT);
	virtual void initGL();
//	virtual void update();
	virtual void render();
	void setM();
	virtual const float* getM() const { return m;}
	virtual const float* mt()
	{
		t->inverse().getOpenGLMatrix(m);
		return m;
	}
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
