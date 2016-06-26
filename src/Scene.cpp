/*
 * Scene.cpp
 *
 *  Created on: May 2, 2016
 *      Author: vuk
 */

#include "Scene.h"

Scene::Scene()
{
	camera=NULL;
	sun=NULL;
	player=NULL;
	//Physiscs

    collisionConfig=new btDefaultCollisionConfiguration();
    dispatcher=new btCollisionDispatcher(collisionConfig);
    broadphase=new btDbvtBroadphase();
    solver=new btSequentialImpulseConstraintSolver();
    world=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);

    debugBtInterface=new DebugBtInterface(
    									  btIDebugDraw::DBG_DrawWireframe |
    									  btIDebugDraw::DBG_DrawAabb |
										  btIDebugDraw::DBG_EnableCCD |
										  btIDebugDraw::DBG_DrawConstraints |
										  btIDebugDraw::DBG_DrawFeaturesText |
										  btIDebugDraw::DBG_DrawText |
										  btIDebugDraw::DBG_DrawFrames
										);
    world->setDebugDrawer(debugBtInterface);

//	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();
//	fileLoader->loadFile("models/mars/megamars1od4.bullet");
//	btCollisionObject* obj = fileLoader->getRigidBodyByIndex(0);
//	btRigidBody *mars;
//	mars = btRigidBody::upcast(obj);
//    btTransform t;
//    t.setIdentity();
////    t.setRotation(btQuaternion(0,-90,0));
////    t.setOrigin(btVector3(0, -526, 0));
//	mars->setWorldTransform(t);
////	mars->translate(btVector3(0, -506, 0));
//	world->addRigidBody(mars);

//    world->setGravity(btVector3(0, -10, 0));	//gravity on Earth
//	btTransform tr;
//    tr.setIdentity();
//    tr.setOrigin(btVector3(0, 515 ,0));
//    btStaticPlaneShape* plane=new btStaticPlaneShape(btVector3(0,1,0),0);
//    btMotionState* motion=new btDefaultMotionState(tr);
//    btRigidBody::btRigidBodyConstructionInfo info(0.0,motion,plane);
//    btRigidBody* planeBody=new btRigidBody(info);
//    planeBody->setRestitution(.33);
//    world->addRigidBody(planeBody);
}

void Scene::update()
{
	camera->update();
	camera->tm->setOrigin(objects->front()->tm->getOrigin());
	if(sun)
		sun->update();
	if(player)
		player->update();
	Object3D::update();
	updated=true;
//	if(objects)
//		for(auto &obj: *objects)
//			obj->updateNoParent();
}

void Scene::render()
{
	updated=false;
	if(sun)
		sun->render();
	if(player)
		player->render();
//    camera->render(); // ne treba render jer samo treba update pozicije
    Object3D::render();
    world->debugDrawWorld();
}

void Scene::addPhysical(Body *argBody)
{
	Object3D::addChild(argBody);
	world->addRigidBody(argBody->body);
}

//void Scene::setM()
//{
//	tm=t;
//	if(parent && !updated)
//	{
//		tm=parent->tm*tm;
////		tm.setRotation(parent->tm.getRotation().inverse());
////		tm.setOrigin(2*tm.getOrigin()-parent->tm.getOrigin());
////		tm.setOrigin(tm.getRotation()*tm.getOrigin());
//		tm.getOpenGLMatrix(m);
//
//	}
//	tm.getOpenGLMatrix(m);
//}


Scene::~Scene()
{
	// TODO Auto-generated destructor stub
}

