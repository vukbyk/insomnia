/*
 * Object3D.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: vuk
 */

#include "Object3D.h"

#include "GL/glut.h"

Object3D::Object3D()
{
	id = ++id_counter;
	t.setIdentity();
	tm.setIdentity();
	parent=NULL;
	objects=NULL;
}

Object3D::Object3D(btTransform argT)
{
	id = ++id_counter;
	tm=t=argT;
	parent=NULL;
	objects=NULL;
}

void Object3D::init()
{
	if(objects)
		for(auto &obj: *objects)
		{
			obj->init();
		}
}

void Object3D::update()
{
	if(!updated)
	{
		tm=t;
		if(parent && !parent->updated)
		{
			parent->update();
			tm=parent->tm*tm;
		}
		setM();
		updated=true;
	}
//	if(objects)
//	    for(auto &obj: *objects)
//	    {
//	      	obj->update();
//	    }
}

void Object3D::render()
{
	updated=false;
//	if(objects)
//		for(auto &obj: *objects)
//		{
//		  obj->render();
//		}
}

void Object3D::setM()
{
//	cout<<"set: "<<id<<endl;
	tm.getOpenGLMatrix(m);
}

void Object3D::add(Object3D *o)
{
	if(objects)
	{
		objects->push_back(o);
		o->setParent(this);
	}
	else
	{
		objects = new std::vector <Object3D*>();
		objects->push_back(o);
		o->setParent(this);
	}
}

void Object3D::setParent(Object3D* parentArg)
{
	parent=parentArg;
}

void Object3D::unparent()
{
	parent=parent->parent;
}

Object3D::~Object3D()
{
	if(objects)
		for(auto &obj: *objects)
		{
			obj->parent=NULL;
		}
}

