/*
 * Sprava.cpp
 *
 *  Created on: May 27, 2016
 *      Author: vuk
 */

#include "Sprava.h"

#include <math.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <freetype2/freetype.h>
//#include <freetype2/ft2build.h>
//#include <FTGL/ftgl.h>

#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Sun.h"
#include "AssimpModel.h"
#include "ModelCallList.h"


#include "WirePlane.h"
#include "Cube.h"

#include <type_traits>//izbrisati ako ne treba unistanje konsta
#include <time.h>
#include <drawtext.h>

float dt=0;

Sprava::Sprava()
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
        close();
//        return EXIT_FAILURE;
    }
}

void Sprava::mainLoop()
{
	//While application is running
	while( !quit )
	{
		lastTime = currentTime;
		currentTime = SDL_GetTicks();

		dt = ((double)(currentTime - lastTime)) / 1000.0;
		eventHandler();
		update();
		render();
		//Update screen
		SDL_GL_SwapWindow( glWindow );

		if(fps)
			printFPS();
	}
}

int Sprava::init()
{
	/* XXX dtx_open_font opens a font file and returns a pointer to dtx_font */
    if(!(font = dtx_open_font("models/font.ttf", 16)))
    {
    	fprintf(stderr, "failed to open font\n");
    	return 1;
    }
    /* XXX select the font and size to render with by calling dtx_use_font
    * if you want to use a different font size, you must first call:
    * dtx_prepare(font, size) once.
    */
    dtx_use_font(font, 16);

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
	//Use OpenGL 2.1
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	//Create window
	glWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_SHOWN*/ );
	if( glWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	       return EXIT_FAILURE;
	}
	//Create context
	gContext = SDL_GL_CreateContext( glWindow );
	if( gContext == NULL )
	{
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
	       return EXIT_FAILURE;
	}
	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );

	}
	SDL_GL_SetSwapInterval(1);

	//Initialize OpenGL models
	truckModel=make_shared<AssimpModel>("models/vehicles/truck.obj","models/vehicles/truck.png");
	models.push_back(truckModel);
	truckWheelModel=make_shared<AssimpModel>("models/vehicles/truckwheel.obj","models/vehicles/truck.png");
	models.push_back(truckWheelModel);
	carModel=make_shared<AssimpModel>("models/vehicles/car.obj","models/vehicles/car.png");
	models.push_back(carModel);
	vultureModel=make_shared<AssimpModel>("models/vulture.obj","models/vulture.png");
	models.push_back(vultureModel);
	specialopsModel=make_shared<AssimpModel>("models/specialops.obj","models/specialops.png");
	models.push_back(specialopsModel);
	bansheeModel=make_shared<AssimpModel>("models/banshee.obj","models/banshee.png");
	models.push_back(bansheeModel);
	marsModel=make_shared<AssimpModel>("models/mars/megamars1od4.obj","models/mars/mars16k.png");
	models.push_back(marsModel);
	satelitModel=make_shared<AssimpModel>("models/satelit.obj","models/satelit.png");
	models.push_back(satelitModel);
	for(auto &mdl: models)
		mdl->initGL();

	scene=new Scene();

	Camera *camera=new Camera();
	camera->t->setOrigin(btVector3(0, 3.2, -.8));
//	camera->t->setOrigin(btVector3(0, 525, 75));
	controls=camera;
	scene->camera=camera;

	Sun *sun=new Sun();
	scene->sun=sun;
//	scene->add(sun);
	sun->t->setOrigin(btVector3(1, 1, 1));
//	controls2=scene->objects->back();

	Light *light=new Light();
//	scene->light=light;
	scene->addChild(light);
	light->t->setOrigin(btVector3(0, 4, -2));
	light->t->setRotation(btQuaternion(0,-15*SIMD_RADS_PER_DEG,0));
//	light->addChild(new ModelCallList(*vultureModel));
	controls2=light;

	truck = new Body("models/vehicles/truck.bullet", *truckModel);
//	player->t->setOrigin(btVector3(0, 526, 0));
	truck->body->getWorldTransform().setOrigin(btVector3(0, 516, 0));
//	btTransform trans;
//	trans.setIdentity();
//	trans.setOrigin(btVector3(0,5,0));
	scene->addPhysical(truck);
	truck->createVehicle(scene->world, btVector3( 1.3, 1.2, 2.0));
//	truck->addChild(camera);

	car = new Body("models/vehicles/car.bullet", *carModel);
	car->body->getWorldTransform().setOrigin(btVector3(5, 516, 0));
	scene->addPhysical(car);
	car->createVehicle(scene->world, btVector3( 1.1, 1.2, 1.6));

	player=new Player();
//	player->t->setOrigin(btVector3(0, 3.1, -0.95));
	player->addChild(camera);
	player->addLight(light);
	truck->addChild(player);
	player->current=truck;
//	truck->orphan(player);
//	car->addChild(player);
//	player->current=car;s


	Body *mars;
	mars = new Body("models/mars/megamars1od4.bullet", *marsModel);
	mars->body->setCollisionFlags(mars->body->getCollisionFlags()| btRigidBody::CF_DISABLE_VISUALIZE_OBJECT  );
	scene->addPhysical(mars);

	btQuaternion q;

	q = btQuaternion(240*SIMD_RADS_PER_DEG, 30*SIMD_RADS_PER_DEG,0);
	marker = new ModelCallList(*satelitModel);
	scene->addChild(marker);
	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
//	marker->t->setRotation(q);
	chk.push_back(q);

	q = btQuaternion(180*SIMD_RADS_PER_DEG, 60*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion(150*SIMD_RADS_PER_DEG, 90*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion(110*SIMD_RADS_PER_DEG, 90*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion( 60*SIMD_RADS_PER_DEG, 95*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion( 30*SIMD_RADS_PER_DEG, 85*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion(30*SIMD_RADS_PER_DEG, 170*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	q = btQuaternion(30*SIMD_RADS_PER_DEG, 230*SIMD_RADS_PER_DEG,0); chk.push_back(q);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(150*SIMD_RADS_PER_DEG, 90*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(110*SIMD_RADS_PER_DEG, 90*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(60*SIMD_RADS_PER_DEG, 95*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(30*SIMD_RADS_PER_DEG, 85*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*550);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(30*SIMD_RADS_PER_DEG, 170*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*540);
////	marker->t->setRotation(q);
//
//	q = btQuaternion(30*SIMD_RADS_PER_DEG, 230*SIMD_RADS_PER_DEG,0);
//	marker = new ModelCallList(*satelitModel);
//	scene->addChild(marker);
//	marker->t->setOrigin(q.getAxis().safeNormalize()*540);
////	marker->t->setRotation(q);
//
//	scene->addChild(new ModelCallList(*bansheeModel));
//	scene->objects->back()->t->setOrigin(btVector3(5, 500, 0));
//	scene->addChild(new ModelCallList(*specialopsModel));
//	scene->objects->back()->t->setOrigin(btVector3(2, 500, 0));
////	controls2=scene->objects->back();
////	controls2->add(light);
////	controls2->add(new ModelCallList(*vultureModel));
////	mars->t->setOrigin(btVector3(0, -526, 0));
////	mars->body->getWorldTransform().setOrigin(btVector3(0, -526, 0));
////	scene->objects->back()->t->setRotation(btQuaternion(0,-90,0));
//	int x=25, y=25;
//	for(int i = 0; i < x; i++)
//		for(int j = 0; j < y; j++)
//		{
//			scene->addChild(new ModelCallList(*specialopsModel));
//			scene->objects->back()->t->setOrigin(btVector3(i*2-x/2*2, 0, j*2-y/2*2));
////			controls->add(objects.rbegin()[0].get());
//		}

    //Initialize SDL !?!ZASTO IZBACUJE GRESKU?!?
	if( !initGL() )
	{
		printf( "Unable to initialize OpenGL!" );
	       return EXIT_FAILURE;
	}
	currentTime = SDL_GetTicks();
    return EXIT_SUCCESS;
}

int Sprava::initGL()
{

    GLenum error = GL_NO_ERROR;

    scene->camera->initGL();

//	for(auto &obj: objects)
//		obj->initGL();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    //Initialize Projection Matrix
//  scene->camera->init();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60.0, (float)screenWidth/screenHeight, 1,3000.0);

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
    	printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    	return EXIT_FAILURE;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
    	printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    	return EXIT_FAILURE;
    }
    //Initialize clear color
    glClearColor( 0.01f, 0.03f, 0.1f, 1.f );
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
    	printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void Sprava::update()
{
	if(dt>0)
	{
		scene->world->stepSimulation(dt, 12);
		scene->update();
	}
//    for(auto &obj: objects)
//      obj->update();
}

void Sprava::render()
{
	glDepthMask (true);
//	glMatrixMode( GL_PROJECTION );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glViewport(0, 0, screenWidth, screenHeight);
    scene->camera->render();

    scene->render();
//    std::cout<<scene->camera->t->getOrigin().x()<< " " << scene->camera->t->getOrigin().y()<< " " << scene->camera->t->getOrigin().z()<< std::endl;
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    if(!raceCompleted)
    {
        glTranslatef(-screenWidth/5.5-35, screenHeight/5.5+10, -400);
    	glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glColor3f(1, 1, 1);
        //    glOrtho(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -1, 1);
    	sprintf(s, " Speed(Km/h): %4.1f \n Time: %d : %d",
    			((Body *)player->parent)->body->getLinearVelocity().length()*2, currentTime/60000, currentTime/1000%60);
    	dtx_string(s);
    	glRasterPos3f(10, 20, 10);
    }
    else
    {
        glTranslatef(-150, 100, -300);
    	glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glColor3f(1, 1, 1);
        //    glOrtho(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -1, 1);
    	sprintf(s, "                FINNISHED         \n"
    			   "                Time %d %d", raceTime/60000, raceTime/1000%60);
    	dtx_string(s);
    	glRasterPos3f(10, 10, 10);
    }


    glEnable(GL_LIGHTING);
    glPopMatrix();
    if(rearView && scene->camera->parent)
    {
		glViewport(screenWidth/3, screenHeight*8/10, screenWidth/3, screenHeight*2/10);

		glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor( 0.01f, 0.03f, 0.1f, 1.f );
		Object3D *parent=scene->camera->parent;
		btTransform cam=*scene->camera->t;
		btTransform retrovisor=*parent->parent->tm;
		btVector3 forward=retrovisor.getBasis()*btVector3(0,0,1);
		btVector3 top=retrovisor.getBasis()*btVector3(0,1,0);
		btVector3 position=retrovisor.getOrigin();
		btQuaternion rot;
		rot.setRotation(top,SIMD_RADS_PER_DEG*180);
		retrovisor.setRotation(rot.normalize()*retrovisor.getRotation().normalize());
		position+=forward*3.8+top*2.3;
		retrovisor.setOrigin(position);
		scene->camera->parent=NULL;
		*scene->camera->t=retrovisor;
		glMatrixMode(GL_PROJECTION);
		glScalef (-1.0, .5, 1.0);
		glMatrixMode(GL_MODELVIEW);
		scene->camera->update();
		scene->camera->render();
		scene->render();
		scene->camera->parent=parent;
		*scene->camera->t=cam;
		glMatrixMode(GL_PROJECTION);
		glScalef (-1.0, 2.0, 1.0);
	}



//    for(auto &obj: objects)
//    	obj->render();
//    scene->render();

//	if(player->tm->getOrigin().angle(marker->t->getOrigin())<5*SIMD_RADS_PER_DEG)
//		marker->t->setOrigin(btVector3(0,0,0));

	if(currentChk>=chk.size())
	{
		raceCompleted=true;
	}
	if(!raceCompleted && player->tm->getOrigin().angle(marker->t->getOrigin())<5*SIMD_RADS_PER_DEG)
	{
		currentChk++;
		marker->t->setOrigin(chk.begin()[currentChk].getAxis()*550);
		if(currentChk>=chk.size())
			raceTime=currentTime;
	}
}

void Sprava::eventHandler()
{
	//Handle events on queue
	while( SDL_PollEvent( &event ))
	{
		//User requests quit
		if( event.type == SDL_QUIT )
		{
			quit = true;
			break;
		}
		if( event.type == SDL_WINDOWEVENT) // The window is resized
		{
			if(event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				screenWidth  = event.window.data1;
				screenHeight = event.window.data2;

				glMatrixMode( GL_PROJECTION );
				glViewport(0, 0, screenWidth, screenHeight);
				glLoadIdentity();

                gluPerspective(60.0, (float)screenWidth/screenHeight, 1, 3000.0);
//                    glOrtho( 0, screenWidth, screenHeight, 0, -1, 1 );
				glMatrixMode( GL_MODELVIEW );
			}
		}
		if (event.type == SDL_KEYDOWN)
		{
			keysHeld[event.key.keysym.scancode] = true;
			keysPressed[event.key.keysym.scancode] = true;
		}
		if (event.type == SDL_KEYUP)
			keysHeld[event.key.keysym.scancode] = false;
		if (event.type == SDL_MOUSEMOTION && mouseButtonDown)

		{
			controls->t->setRotation(btQuaternion( (  mouseX-event.motion.x )*screenWidth/100*SIMD_RADS_PER_DEG*dt, 0, 0) * controls->t->getRotation());
//			controls->t->setRotation(btQuaternion( 0,(  mouseY-event.motion.y )*screenHeight/100*SIMD_RADS_PER_DEG*dt, 0) * controls->t->getRotation());
			controls->t->setRotation(controls->t->getRotation()*btQuaternion(0, (  mouseY-event.motion.y )*screenHeight/100*SIMD_RADS_PER_DEG*dt, 0));

			mouseX = event.motion.x;
			mouseY = event.motion.y;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			mouseButtonDown = true;

		}
		if (event.type == SDL_MOUSEBUTTONUP)
//		else
			mouseButtonDown=false;

	}
	controlKeySetup();
}

void Sprava::controlKeySetup()
{
	float camspeed = 1;
	if ( keysHeld[SDL_SCANCODE_ESCAPE] )
		quit=true;
	if ( keysHeld[SDL_SCANCODE_DOWN] )
	{
		controls->t->setRotation(controls->t->getRotation()*btQuaternion(0, -30*SIMD_RADS_PER_DEG*dt, 0));
//			controls->t->setRotation(btQuaternion(0, -1*SIMD_RADS_PER_DEG, 0) * controls->t->getRotation());
//			controls->t->setOrigin(controls->t->getOrigin().rotate( btVector3(1, 0, 0), -SIMD_RADS_PER_DEG));
	}
	if ( keysHeld[SDL_SCANCODE_UP] )
	{
		controls->t->setRotation(controls->t->getRotation()*btQuaternion(0,  30*SIMD_RADS_PER_DEG*dt, 0));
//			controls->t->setRotation(btQuaternion(0,  1*SIMD_RADS_PER_DEG, 0) * controls->t->getRotation());
//			controls->t->setOrigin(controls->t->getOrigin().rotate( btVector3(1, 0, 0), SIMD_RADS_PER_DEG));
	}
	if ( keysHeld[SDL_SCANCODE_LEFT] )
	{
		controls->t->setRotation(btQuaternion( 30*SIMD_RADS_PER_DEG*dt, 0, 0) * controls->t->getRotation());
//			controls->t->setOrigin(controls->t->getOrigin().rotate( controls->t->getBasis()*btVector3(0, 1, 0), -SIMD_RADS_PER_DEG));
//			controls->t->setRotation(controls->t->getRotation()*btQuaternion( -1*SIMD_RADS_PER_DEG, 0, 0));
	}
	if ( keysHeld[SDL_SCANCODE_RIGHT] )
	{
		controls->t->setRotation(btQuaternion(-30*SIMD_RADS_PER_DEG*dt, 0, 0) * controls->t->getRotation());
//			controls->t->setOrigin(controls->t->getOrigin().rotate( controls->t->getBasis()*btVector3(0, 1, 0), SIMD_RADS_PER_DEG));
//			controls->t->setRotation(controls->t->getRotation()*btQuaternion( 1*SIMD_RADS_PER_DEG, 0, 0));
	}
	if ( keysHeld[SDL_SCANCODE_W] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(0, 0, -camspeed)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 0, 0.1));
	if ( keysHeld[SDL_SCANCODE_S] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(0, 0, camspeed)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 0, -0.1));
	if ( keysHeld[SDL_SCANCODE_A] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(-camspeed, 0, 0)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3( 0.1, 0, 0));
	if ( keysHeld[SDL_SCANCODE_D] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3( camspeed, 0, 0)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(-0.1, 0, 0));
	if ( keysHeld[SDL_SCANCODE_E] )
		controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, camspeed, 0)*dt);
	if ( keysHeld[SDL_SCANCODE_Q] )
		controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, -camspeed, 0)*dt);

	if ( keysHeld[SDL_SCANCODE_T] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3( 0, 0,-1.f)*dt);
	if ( keysHeld[SDL_SCANCODE_G] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3( 0, 0, 1.f)*dt);
	if ( keysHeld[SDL_SCANCODE_F] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3(-1.f, 0, 0)*dt);
	if ( keysHeld[SDL_SCANCODE_H] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3( 1.f, 0, 0)*dt);
	if ( keysHeld[SDL_SCANCODE_Y] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3( 0, 1.f, 0)*dt);
	if ( keysHeld[SDL_SCANCODE_R] )
		controls2->t->setOrigin(controls2->t->getOrigin()+controls2->t->getBasis()*btVector3( 0,-1.f, 0)*dt);

	if ( keysHeld[SDL_SCANCODE_I] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion( 0, 30*SIMD_RADS_PER_DEG*dt, 0));
	if ( keysHeld[SDL_SCANCODE_K] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion( 0,-30*SIMD_RADS_PER_DEG*dt, 0));
	if ( keysHeld[SDL_SCANCODE_J] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion( 30*SIMD_RADS_PER_DEG*dt, 0, 0));
	if ( keysHeld[SDL_SCANCODE_L] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion(-30*SIMD_RADS_PER_DEG*dt, 0, 0));
	if ( keysHeld[SDL_SCANCODE_U] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion( 0, 0, 30*SIMD_RADS_PER_DEG*dt));
	if ( keysHeld[SDL_SCANCODE_O] )
		controls2->t->setRotation(controls2->t->getRotation()*btQuaternion( 0, 0,-30*SIMD_RADS_PER_DEG*dt));

	if ( keysHeld[SDL_SCANCODE_KP_2] )
	{
		player->current->vehicle->applyEngineForce(2500,0);
		player->current->vehicle->applyEngineForce(2500,1);
		player->current->vehicle->applyEngineForce(2500,2);
		player->current->vehicle->applyEngineForce(2500,3);
		player->current->vehicle->setBrake(0,0);
		player->current->vehicle->setBrake(0,1);
		player->current->vehicle->setBrake(0,2);
		player->current->vehicle->setBrake(0,3);
	}
	else
	{
		player->current->vehicle->applyEngineForce(0,0);
		player->current->vehicle->applyEngineForce(0,1);
		player->current->vehicle->applyEngineForce(0,2);
		player->current->vehicle->applyEngineForce(0,3);
	}
	if ( keysHeld[SDL_SCANCODE_KP_5] )
	{
		player->current->vehicle->applyEngineForce(-2500,0);
		player->current->vehicle->applyEngineForce(-2500,1);
		player->current->vehicle->applyEngineForce(-2500,2);
		player->current->vehicle->applyEngineForce(-2500,3);
		player->current->vehicle->setBrake(0,0);
		player->current->vehicle->setBrake(0,1);
		player->current->vehicle->setBrake(0,2);
		player->current->vehicle->setBrake(0,3);
	}
	else
	{
		player->current->vehicle->applyEngineForce(0,0);
		player->current->vehicle->applyEngineForce(0,1);
		player->current->vehicle->applyEngineForce(0,2);
		player->current->vehicle->applyEngineForce(0,3);
	}
	if ( keysHeld[SDL_SCANCODE_KP_1] && player->current->vehicle->getSteeringValue(3)<SIMD_RADS_PER_DEG*maxTurn)
		player->current->vehicle->setSteeringValue(player->current->vehicle->getSteeringValue(2)+SIMD_RADS_PER_DEG*60*dt,2),
		player->current->vehicle->setSteeringValue(player->current->vehicle->getSteeringValue(3)+SIMD_RADS_PER_DEG*60*dt,3);
	if ( keysHeld[SDL_SCANCODE_KP_3] && player->current->vehicle->getSteeringValue(3)>-SIMD_RADS_PER_DEG*maxTurn)
		player->current->vehicle->setSteeringValue(player->current->vehicle->getSteeringValue(2)-SIMD_RADS_PER_DEG*60*dt,2),
		player->current->vehicle->setSteeringValue(player->current->vehicle->getSteeringValue(3)-SIMD_RADS_PER_DEG*60*dt,3);
	if ( keysHeld[SDL_SCANCODE_KP_4] )
		player->current->vehicle->setBrake(130,0),player->current->vehicle->setBrake(130,1),
		player->current->vehicle->setBrake(130,2),player->current->vehicle->setBrake(130,3);
	if ( keysHeld[SDL_SCANCODE_KP_0] )
	{
		player->current->vehicle->setBrake(1300,0);
		player->current->vehicle->setBrake(1300,1);
		player->current->vehicle->setBrake(1300,2);
		player->current->vehicle->setBrake(1300,3);
		player->current->vehicle->applyEngineForce(0,0);
		player->current->vehicle->applyEngineForce(0,1);
		player->current->vehicle->applyEngineForce(0,2);
		player->current->vehicle->applyEngineForce(0,3);
	}
	if ( keysHeld[SDL_SCANCODE_KP_PERIOD] )
	{
		player->current->vehicle->setBrake(0,0);
		player->current->vehicle->setBrake(0,1);
		player->current->vehicle->setBrake(0,2);
		player->current->vehicle->setBrake(0,3);
		player->current->vehicle->applyEngineForce(0,0);
		player->current->vehicle->applyEngineForce(0,1);
		player->current->vehicle->applyEngineForce(0,2);
		player->current->vehicle->applyEngineForce(0,3);
	}

	if (keysHeld[SDL_SCANCODE_SPACE])
		player->current->body->applyCentralForce(player->current->body->getCenterOfMassPosition().normalized()*100000);

	if ( keysPressed[SDL_SCANCODE_P])
	{
		(fps==false)?fps=true:fps=false;
		keysPressed[SDL_SCANCODE_P]=false;
	}
	if ( keysPressed[SDL_SCANCODE_Z] || keysPressed[SDL_SCANCODE_KP_7])
	{
		keysPressed[SDL_SCANCODE_Z] = keysPressed[SDL_SCANCODE_KP_7]=false;

		if(player->light->on)
			player->light->on=false;
		else
			player->light->on=true;
	}

	if ( keysPressed[SDL_SCANCODE_KP_8])
	{
		keysPressed[SDL_SCANCODE_KP_8]=false;
		(rearView==false)? rearView = true: rearView = false;
	}
	if ( keysPressed[SDL_SCANCODE_KP_9])
	{
		keysPressed[SDL_SCANCODE_KP_9]=false;
		if(player->current==truck)
		{
			truck->orphan(player);
			car->addChild(player);
			player->current=car;
			maxTurn = 25;
			scene->camera->t->setOrigin(btVector3(0, 2.7, .0));
		}
		else
		{
			car->orphan(player);
			truck->addChild(player);
			player->current=truck;
			maxTurn = 45;
			scene->camera->t->setOrigin(btVector3(0, 3.2, -.4));
		}
	}
	if ( keysPressed[SDL_SCANCODE_KP_6])
	{
		keysPressed[SDL_SCANCODE_KP_6]=false;
		if(!scene->camera->cabinView)
		{
			if(player->current==truck)
			{
				scene->camera->t->setOrigin(btVector3(0, 3.2, -.4));
			}
			else
			{
				scene->camera->t->setOrigin(btVector3(0, 2.7, .0));
			}
			scene->camera->cabinView=true;
		}
		else
		{
			scene->camera->cabinView=false;
			scene->camera->t->setOrigin(btVector3(0,6,10));
			btVector3 forwardOffset=player->tm->getBasis()* scene->camera->t->getOrigin();
			scene->camera->tm->setOrigin(player->tm->getOrigin() + forwardOffset);
			scene->camera->tm->setRotation(player->tm->getRotation() * scene->camera->t->getRotation());
		}
	}
}

void Sprava::printFPS()
{
	if(countStep++==0)
		begin=SDL_GetTicks();
	else if(countStep==printStep)
	{
		cout<<"FPS: "<< 1000.*printStep/(SDL_GetTicks()-begin)<<endl;
		countStep=0;
	}
}

void Sprava::close()
{
	//Disable text input
	SDL_StopTextInput();
    //Destroy window
    SDL_DestroyWindow( glWindow );
    glWindow = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

Sprava::~Sprava()
{
	close();
}

