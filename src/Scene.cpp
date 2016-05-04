/*
 * Scene.cpp
 *
 *  Created on: May 2, 2016
 *      Author: vuk
 */

#include "Scene.h"

Scene::Scene()
{
	// TODO Auto-generated constructor stub

}

//void Scene::update()
//{
////	if(parent)
////	{
//////		parent->setM();
////		setM();
////		if(objects)
////			for(auto &obj: *objects)
////			{
////				if(obj!=parent)
////				{
////					for(auto &obj: *objects)
////					{
////						obj->update();
////					}
////				}
////			}
////	}
////	else
//	{
//		updated=false;
//		setM();
//		updated=true;
//		if(objects)
//			for(auto &obj: *objects)
//			{
//				obj->update();
//			}
//	}
//}

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

