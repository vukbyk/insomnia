/*
 * Sprava.h
 *
 *  Created on: May 27, 2016
 *      Author: vuk
 */

#ifndef SPRAVA_H_
#define SPRAVA_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <vector>
#include <memory>

#include "Object3D.h"
#include "Body.h"
#include "ModelCallList.h"


#include <iostream>
using namespace std;


typedef std::shared_ptr <Object3D> ObjectPtr;
//typedef std::make_shared() <Object3D> ObjectPtr;
typedef std::vector <ObjectPtr> Objects;

class Sprava
{
	//Screen dimension constants
	int screenWidth  = 1024;
	int screenHeight = 760;

	//Event handler
	SDL_Event event;
	bool keysHeld[323] = {false};

	//The window we'll be rendering to
	SDL_Window* glWindow = NULL;
	//OpenGL context
	SDL_GLContext gContext;
	//Main loop flag
	bool quit = false;

	bool fps=false;
	unsigned int lastTime;
	unsigned int currentTime;
	float dt;
	unsigned int begin=0;
	unsigned int countStep=0;
	unsigned int printStep=50;
//	unsigned int avg=0;

	Objects models;
	Objects objects;
	ObjectPtr controls, controls2;

	shared_ptr<Body> player;
	shared_ptr<AssimpModel> vultureModel;
	shared_ptr<AssimpModel> specialopsModel;
	shared_ptr<AssimpModel> bansheeModel;

//Physics
	btDynamicsWorld* world;	                    //every physical object go to the world
	btDispatcher* dispatcher;	                //what collision algorithm to use?
	btCollisionConfiguration* collisionConfig;	//what collision algorithm to use?
	btBroadphaseInterface* broadphase;	        //should Bullet examine every object, or just what close to each other
	btConstraintSolver* solver;					//solve collisions, apply forces, impulses

public:
	Sprava();
	void mainLoop();
	//Starts up SDL, creates window, and initializes OpenGL
	int init();
	//Initializes matrices and clear color
	int initGL();
	//Per frame update
	void update();
	//Renders quad to the screen
	void render();
	void eventHandler();
	void controlKeySetup();
	void printFPS();
	//Frees media and shuts down SDL
	void close();
	virtual ~Sprava();
};

#endif /* SPRAVA_H_ */
