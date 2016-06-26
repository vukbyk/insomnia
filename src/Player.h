/*
 * Player.h
 *
 *  Created on: Jun 25, 2016
 *      Author: vuk
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Object3D.h"

class Body;
class Light;
class Player: public Object3D
{
public:
	Body *current;
	Light *light;
	Player();
	virtual void addLight(Light *l);
	virtual ~Player();


};

#endif /* PLAYER_H_ */
