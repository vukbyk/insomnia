/*
 * Player.cpp
 *
 *  Created on: Jun 25, 2016
 *      Author: vuk
 */

#include "Player.h"

Player::Player()
{
	// TODO Auto-generated constructor stub
}

void Player::addLight(Light *l)
{
	Object3D::addChild((Object3D*) l);
	light=l;
}

Player::~Player()
{
	// TODO Auto-generated destructor stub
}
