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
#include "Camera.h"

class Scene: public Object3D
{
	std::shared_ptr <Object3D> camera;
public:

	Scene();
//	virtual void update();
//	virtual void setM();
	virtual ~Scene();
};

#endif /* SCENE_H_ */
