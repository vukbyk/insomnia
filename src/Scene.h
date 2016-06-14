/*
 * Scene.h
 *
 *  Created on: May 2, 2016
 *      Author: vuk
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "Object3D.h"
#include "Body.h"
#include "ModelCallList.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>
#include "DebugBtInterface.h"

#include "Camera.h"

class Scene: public Object3D
{
public:
//Physics
	btDynamicsWorld* world;	                    //every physical object go to the world
	btDispatcher* dispatcher;	                //what collision algorithm to use?
	btCollisionConfiguration* collisionConfig;	//what collision algorithm to use?
	btBroadphaseInterface* broadphase;	        //should Bullet examine every object, or just what close to each other
	btConstraintSolver* solver;					//solve collisions, apply forces, impulses
	DebugBtInterface *debugBtInterface;

public:
	Object3D *camera;
	Scene();
	virtual void update();
	virtual void render();
	virtual void addPhysical(Body *argBody);
//	virtual void setM();
	virtual ~Scene();
};

#endif /* SCENE_H_ */
