/*
 * Sprava.cpp
 *
 *  Created on: May 27, 2016
 *      Author: vuk
 */

#include "Sprava.h"

#include <math.h>
#include <GL/glu.h>
//#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Scene.h"
#include "Camera.h"
#include "AssimpModel.h"
#include "ModelCallList.h"


#include "WirePlane.h"
#include "Cube.h"

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
		dt = ((double)(currentTime - lastTime)) / 1000;
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
	SDL_GL_SetSwapInterval(0);


	//Physiscs

    collisionConfig=new btDefaultCollisionConfiguration();
    dispatcher=new btCollisionDispatcher(collisionConfig);
    broadphase=new btDbvtBroadphase();
    solver=new btSequentialImpulseConstraintSolver();
    world=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
    world->setGravity(btVector3(0,-9.81,0));	//gravity on Earth
    debugBtInterface=new DebugBtInterface(btIDebugDraw::DBG_DrawWireframe
    									| btIDebugDraw::DBG_DrawAabb
										| btIDebugDraw::DBG_EnableCCD
										| btIDebugDraw::DBG_DrawConstraints
										| btIDebugDraw::DBG_DrawFeaturesText
										| btIDebugDraw::DBG_DrawText
										| btIDebugDraw::DBG_DrawFrames
										);
//    world->setDebugDrawer(debugBtInterface);


    btTransform tr;
    tr.setIdentity();
    tr.setOrigin(btVector3(0, -15,0));
//    btStaticPlaneShape* plane=new btStaticPlaneShape(btVector3(0,1,0),0);
//    btMotionState* motion=new btDefaultMotionState(tr);
//    btRigidBody::btRigidBodyConstructionInfo info(0.0,motion,plane);
//    btRigidBody* planeBody=new btRigidBody(info);
//    planeBody->setRestitution(.33);
//    world->addRigidBody(planeBody);

	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();
	fileLoader->loadFile("models/mars/mars.bullet");
	btCollisionObject* obj = fileLoader->getRigidBodyByIndex(0);
	btRigidBody *mars;
	mars = btRigidBody::upcast(obj);
	mars->translate(btVector3(0, 0,0));
	world->addRigidBody(mars);


	//Initialize OpenGL

	vultureModel=make_shared<AssimpModel>("models/vulture.obj","models/vulture.png");
	models.push_back(vultureModel);
	specialopsModel=make_shared<AssimpModel>("models/specialops.obj","models/specialops.png");
	models.push_back(specialopsModel);
	bansheeModel=make_shared<AssimpModel>("models/banshee.obj","models/banshee.png");
	models.push_back(bansheeModel);
	marsModel=make_shared<AssimpModel>("models/mars/mars.obj","models/mars/mars8kcompress.png");
	models.push_back(marsModel);
	for(auto &mdl: models)
		mdl->initGL();

	objects.push_back(make_shared<Camera>());
		objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 1, 5));
		controls=objects.back();

	player=make_shared<Body>(*vultureModel);
	objects.push_back(player);
	world->addRigidBody(player->body);
	player->createVehicle(world);
//	objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 2, 0));

    btTransform initialTransform;
    initialTransform.setIdentity();
    initialTransform.setOrigin(btVector3(2, 2, 0));
//	player->body->setCenterOfMassTransform( initialTransform);
	controls2=objects.back();
	objects.push_back(make_shared<ModelCallList>(*bansheeModel));
	objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(2, 0, 0));
	objects.push_back(make_shared<ModelCallList>(*specialopsModel));
	objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(-2, 0, 0));
	objects.push_back(make_shared<ModelCallList>(*marsModel));
	objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, -1535, 0));
//	objects.push_back(make_shared<AssimpModel>());
//	objects.push_back(ObjectPtr(make_shared<AssimpModel>("models/vultureModel.obj","models/vultureModel.png")));
//			objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 2, 0));
//	objects.push_back(ObjectPtr(make_shared<AssimpModel>("models/viking/viking.obj","models/viking/Viking_Diffuse.jpg")));
//		objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 0, 0));
//		objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 3, 0));
////	objects.rbegin()[1]->add(objects.begin()[0].get());
//	controls2=objects.rbegin()[1];
	int x=0*10, y=0*500;
	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
		{
			objects.push_back(make_shared</*AssimpModel*/ModelCallList>(rand()%3+1));
			objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(i*3-x, 0, j*3-y));
//			controls->add(objects.rbegin()[0].get());
		}
//	objects.push_back(ObjectPtr(new Cube())); objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(0, 2,-4));
//	objects.push_back(ObjectPtr(new Cube())); objects.back()->t->setOrigin(objects.back()->t->getOrigin()+btVector3(4, 2,-4));

    //Initialize SDL !?!ZASTO?!?
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

	for(auto &obj: objects)
	{
		obj->initGL();
	}



    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    float dif[]= {1,1,1,1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

    glEnable(GL_DEPTH_TEST);
    //Initialize Projection Matrix
//    camera->init();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60.0, (float)screenWidth/screenHeight,.1,1500.0);
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
    	printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    	return EXIT_FAILURE;
    }
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
//    gluLookAt (0.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
    	printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    	return EXIT_FAILURE;
    }
    //Initialize clear color
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );
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
    world->stepSimulation(dt, 12);

    for(auto &obj: objects)
    {
      obj->update();
    }
}

void Sprava::render()
{
	glDepthMask (true);
//	glMatrixMode( GL_PROJECTION );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    float pos[]= {2, 2, 3, 1};
    glColor3f(1,1,1);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

//    scene.render();

//    glTranslatef(0,-2,-14);
//    glRotatef(45,0,1,0);

//    gluLookAt (5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    for(auto &obj: objects)
    {
    	obj->render();
    }
    world-> debugDrawWorld();
//    player.render();
//    glPopMatrix();


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

                gluPerspective(60.0, (float)screenWidth/screenHeight, 1.0, 500.0);
//                    glOrtho( 0, screenWidth, screenHeight, 0, -1, 1 );
				glMatrixMode( GL_MODELVIEW );
			}
		}
		if (event.type == SDL_KEYDOWN)
			keysHeld[event.key.keysym.scancode] = true;
		if (event.type == SDL_KEYUP)
			keysHeld[event.key.keysym.scancode] = false;
	}
	controlKeySetup();
}

void Sprava::controlKeySetup()
{
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
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(0, 0,-100.f)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 0, 0.1));
	if ( keysHeld[SDL_SCANCODE_S] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(0, 0, 100.f)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 0, -0.1));
	if ( keysHeld[SDL_SCANCODE_A] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3(-100.f, 0, 0)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3( 0.1, 0, 0));
	if ( keysHeld[SDL_SCANCODE_D] )
		controls->t->setOrigin(controls->t->getOrigin()+quatRotate(controls->t->getRotation(), btVector3( 100.f, 0, 0)*dt));
//			controls->t->setOrigin(controls->t->getOrigin()+btVector3(-0.1, 0, 0));
	if ( keysHeld[SDL_SCANCODE_E] )
		controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 1.f, 0)*dt);
	if ( keysHeld[SDL_SCANCODE_Q] )
		controls->t->setOrigin(controls->t->getOrigin()+btVector3(0, 1.f, 0)*dt);

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


	if ( keysHeld[SDL_SCANCODE_KP_5] )
			player->vehicle->applyEngineForce(1500,0), player->vehicle->applyEngineForce(1500,1)
			,player->vehicle->applyEngineForce(1500,2), player->vehicle->applyEngineForce(1500,3);
	if ( keysHeld[SDL_SCANCODE_KP_2] )
			player->vehicle->applyEngineForce(0,0), player->vehicle->applyEngineForce(0,1)
			,player->vehicle->applyEngineForce(0,2), player->vehicle->applyEngineForce(0,3);
	if ( keysHeld[SDL_SCANCODE_KP_1] && player->vehicle->getSteeringValue(3)<SIMD_RADS_PER_DEG*45)
		player->vehicle->setSteeringValue(player->vehicle->getSteeringValue(2)+SIMD_RADS_PER_DEG*45*dt,2),
		player->vehicle->setSteeringValue(player->vehicle->getSteeringValue(3)+SIMD_RADS_PER_DEG*45*dt,3);
	if ( keysHeld[SDL_SCANCODE_KP_3] && player->vehicle->getSteeringValue(3)>-SIMD_RADS_PER_DEG*45)
		player->vehicle->setSteeringValue(player->vehicle->getSteeringValue(2)-SIMD_RADS_PER_DEG*45*dt,2),
		player->vehicle->setSteeringValue(player->vehicle->getSteeringValue(3)-SIMD_RADS_PER_DEG*45*dt,3);
	if ( keysHeld[SDL_SCANCODE_KP_4] )
		player->vehicle->setBrake(130,0),player->vehicle->setBrake(130,1),player->vehicle->setBrake(130,2),player->vehicle->setBrake(130,3);
	if ( keysHeld[SDL_SCANCODE_KP_0]||keysHeld[SDL_SCANCODE_KP_5] )
			player->vehicle->setBrake(0,0),player->vehicle->setBrake(0,1),player->vehicle->setBrake(0,2),player->vehicle->setBrake(0,3);

	if (keysHeld[SDL_SCANCODE_SPACE])
		player->body->applyCentralForce(btVector3(0,10000,0));

	if ( keysHeld[SDL_SCANCODE_P]/* && (keysHeld[SDL_SCANCODE_LCTRL] || keysHeld[SDL_SCANCODE_RCTRL])*/)
		(fps==false)?fps=true:fps=false;

}

void Sprava::printFPS()
{
	if(printStep++==0)
		begin=SDL_GetTicks();
	else if(printStep==printStep)
	{
		cout<<"Ticks: "<< 1000.*printStep/(SDL_GetTicks()-begin)<<endl;
		printStep=0;
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

