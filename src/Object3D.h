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
#include <vector>

class Scene;

#include <iostream>
using namespace std;

class Object3D
{
protected:
	unsigned long int id;
	float m[16];
	Object3D *parent;
//	int index;//index niza

	virtual void setParent(Object3D* parent);
public:
	btTransform *t;
	btTransform *tm;
	std::vector <Object3D*> *objects;
	bool updated;

	Object3D();
    Object3D(btTransform argT);
    virtual void initGL();
    virtual void update();
    virtual void updateNoParent();
    virtual void render();
	virtual void setM();
	virtual const float* getM() const { return m;}
	virtual void add(Object3D *o);
	virtual void unparent();
//	virtual const float* mt() { t.getOpenGLMatrix(m); return m; }
	virtual ~Object3D();
};
#endif /* OBJECT3D_H_ */
