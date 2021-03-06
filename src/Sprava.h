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
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <drawtext.h>
//#include <oglft/OGLFT.h>
//#include <FTGL/ftgl.h>
//#include <FTGL/FTGLBitmapFont.h>
//#include <gltt/FTFace.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>

#include <vector>
#include <memory>

#include "Object3D.h"
#include "Body.h"
#include "ModelCallList.h"
#include "DebugBtInterface.h"
#include "Player.h"

class Scene;

#include <iostream>
using namespace std;


typedef std::shared_ptr <Object3D> ObjectPtr;
//typedef std::make_shared() <Object3D> ObjectPtr;
typedef std::vector <ObjectPtr> Objects;
//struct dtx_font;

class Sprava
{
	float mouseX=0, mouseY=0;
	bool mouseButtonDown=false;
    char s[100];
    int raceTime;
	bool raceCompleted = false;
	unsigned int currentChk=0;
	vector <btQuaternion> chk;
	dtx_font *font;
	//Screen dimension constants
	int screenWidth  = 1024;
	int screenHeight = 760;

	//Event handler
	SDL_Event event;
	bool keysHeld[323] = {false};
	bool keysPressed[323] = {false};

	//The window we'll be rendering to
	SDL_Window* glWindow = NULL;
	//OpenGL context
	SDL_GLContext gContext;
	//Main loop flag
	bool quit = false;

	bool fps=false;
	unsigned int lastTime;
	unsigned int currentTime;
//	float dt;
	unsigned int begin=0;
	unsigned int countStep=0;
	unsigned int printStep=50;
//	unsigned int avg=0;

	Scene *scene;
	Player *player;
	Body *truck;
	Body *car;
	float maxTurn=45;
	bool rearView=false;
	Object3D *controls, *controls2;
	ModelCallList *marker;

	Objects models;
	Objects objects;
//	ObjectPtr controls, controls2;

//	shared_ptr<Body> player;
	shared_ptr<AssimpModel> truckModel;
	shared_ptr<AssimpModel> carModel;
	shared_ptr<AssimpModel> truckWheelModel;
	shared_ptr<AssimpModel> vultureModel;
	shared_ptr<AssimpModel> specialopsModel;
	shared_ptr<AssimpModel> bansheeModel;
	shared_ptr<AssimpModel> marsModel;
	shared_ptr<AssimpModel> satelitModel;

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
