//============================================================================
// Name        : insomnia.cpp
// Author      : Vuk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
//#include <GL/glut.h>

#include <vector>
#include <memory>

#include <btBulletDynamicsCommon.h>

#include "Object3D.h"
#include "Scene.h"
#include "Camera.h"
#include "AssimpModel.h"
#include "WirePlane.h"
#include "Cube.h"

#include <iostream>
using namespace std;

//Screen dimension constants
int screenWidth  = 1024;
int screenHeight = 760;

//Main loop flag
bool quit = false;
//Starts up SDL, creates window, and initializes OpenGL
int init();
//Initializes matrices and clear color
int initGL();
//Per frame update
void update();
//Renders quad to the screen
void render();
//Frees media and shuts down SDL
void close();
//The window we'll be rendering to
SDL_Window* glWindow = NULL;
//OpenGL context
SDL_GLContext gContext;

typedef std::shared_ptr <Object3D> ObjectPtr;
//typedef std::make_shared() <Object3D> ObjectPtr;
typedef std::vector <ObjectPtr> Objects;
Objects objects;
ObjectPtr controls, controls2;


int init()
{
	objects.push_back(make_shared<Camera>());
		objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(0, 1, 5));
		controls=objects.back();
//	objects.push_back(make_shared<WirePlane>());
	objects.push_back(make_shared<AssimpModel>());
	objects.push_back(ObjectPtr(make_shared<AssimpModel>("models/vulture.obj","models/vulture.png")));
		objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(2, 0, 0));
	objects.push_back(ObjectPtr(make_shared<AssimpModel>("models/viking/viking.obj","models/viking/Viking_Diffuse.jpg")));
		objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(0, 0, 0));
		objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(0, 3, 0));
	objects.rbegin()[1]->add(objects.begin()[0].get());
	controls2=objects.rbegin()[1];
	int x=0, y=0;
	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
		{
			objects.push_back(make_shared<AssimpModel>(1));
			objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(i*3-x, 0, j*3-y));
//			controls2->add(objects.rbegin()[0].get());
		}
//	objects.push_back(ObjectPtr(new Cube())); objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(0, 2,-4));
//	objects.push_back(ObjectPtr(new Cube())); objects.back()->t.setOrigin(objects.back()->t.getOrigin()+btVector3(4, 2,-4));

    //Initialize SDL
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
	//Initialize OpenGL
	if( !initGL() )
	{
		printf( "Unable to initialize OpenGL!" );
	       return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}

int initGL()
{
    GLenum error = GL_NO_ERROR;
	for(auto &obj: objects)
	{
		obj->init();
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
    gluPerspective(60.0, (float)screenWidth/screenHeight,.1,500.0);
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

void update()
{
    for(auto &obj: objects)
    {
      obj->update();
    }
}

void render()
{
	glDepthMask (true);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    float pos[]= {2, 2, 3, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

//    scene.render();

//    glTranslatef(0,-2,-14);
//    glRotatef(45,0,1,0);

//    gluLookAt (5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    for(auto &obj: objects)
    {
    	obj->render();
    }
//    glPopMatrix();


}

void close()
{
    //Destroy window
    SDL_DestroyWindow( glWindow );
    glWindow = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] )
{
//	glutInit(&argc, args);
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
        close();
        return EXIT_FAILURE;
    }
	//Event handler
	SDL_Event event;
	bool keysHeld[323] = {false};

	//While application is running
	while( !quit )
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
			{
				keysHeld[event.key.keysym.scancode] = true;
			}
			if (event.type == SDL_KEYUP)
			{
				keysHeld[event.key.keysym.scancode] = false;
			}
		}
		if ( keysHeld[SDL_SCANCODE_DOWN] )
		{
			controls->t.setRotation(controls->t.getRotation()*btQuaternion(0, -1*SIMD_RADS_PER_DEG, 0));
//			controls->t.setRotation(btQuaternion(0, -1*SIMD_RADS_PER_DEG, 0) * controls->t.getRotation());
//			controls->t.setOrigin(controls->t.getOrigin().rotate( btVector3(1, 0, 0), -SIMD_RADS_PER_DEG));
		}
		if ( keysHeld[SDL_SCANCODE_UP] )
		{
			controls->t.setRotation(controls->t.getRotation()*btQuaternion(0,  1*SIMD_RADS_PER_DEG, 0));
//			controls->t.setRotation(btQuaternion(0,  1*SIMD_RADS_PER_DEG, 0) * controls->t.getRotation());
//			controls->t.setOrigin(controls->t.getOrigin().rotate( btVector3(1, 0, 0), SIMD_RADS_PER_DEG));
		}
		if ( keysHeld[SDL_SCANCODE_LEFT] )
		{
			controls->t.setRotation(btQuaternion( 1*SIMD_RADS_PER_DEG, 0, 0) * controls->t.getRotation());
//			controls->t.setOrigin(controls->t.getOrigin().rotate( controls->t.getBasis()*btVector3(0, 1, 0), -SIMD_RADS_PER_DEG));
//			controls->t.setRotation(controls->t.getRotation()*btQuaternion( -1*SIMD_RADS_PER_DEG, 0, 0));
		}
		if ( keysHeld[SDL_SCANCODE_RIGHT] )
		{
			controls->t.setRotation(btQuaternion(-1*SIMD_RADS_PER_DEG, 0, 0) * controls->t.getRotation());
//			controls->t.setOrigin(controls->t.getOrigin().rotate( controls->t.getBasis()*btVector3(0, 1, 0), SIMD_RADS_PER_DEG));
//			controls->t.setRotation(controls->t.getRotation()*btQuaternion( 1*SIMD_RADS_PER_DEG, 0, 0));
		}
		if ( keysHeld[SDL_SCANCODE_W] )
			controls->t.setOrigin(controls->t.getOrigin()+quatRotate(controls->t.getRotation(), btVector3(0, 0,-.1)));
//			controls->t.setOrigin(controls->t.getOrigin()+btVector3(0, 0, 0.1));
		if ( keysHeld[SDL_SCANCODE_S] )
			controls->t.setOrigin(controls->t.getOrigin()+quatRotate(controls->t.getRotation(), btVector3(0, 0, 0.1)));
//			controls->t.setOrigin(controls->t.getOrigin()+btVector3(0, 0, -0.1));
		if ( keysHeld[SDL_SCANCODE_A] )
			controls->t.setOrigin(controls->t.getOrigin()+quatRotate(controls->t.getRotation(), btVector3(-0.1, 0, 0)));
//			controls->t.setOrigin(controls->t.getOrigin()+btVector3( 0.1, 0, 0));
		if ( keysHeld[SDL_SCANCODE_D] )
			controls->t.setOrigin(controls->t.getOrigin()+quatRotate(controls->t.getRotation(), btVector3( 0.1, 0, 0)));
//			controls->t.setOrigin(controls->t.getOrigin()+btVector3(-0.1, 0, 0));
		if ( keysHeld[SDL_SCANCODE_E] )
			controls->t.setOrigin(controls->t.getOrigin()+btVector3(0, 0.1, 0));
		if ( keysHeld[SDL_SCANCODE_Q] )
			controls->t.setOrigin(controls->t.getOrigin()+btVector3(0,-0.1, 0));


		if ( keysHeld[SDL_SCANCODE_T] )
			controls2->t.setOrigin(controls2->t.getOrigin()+controls2->t.getBasis()*btVector3(0, 0,-1.1));
		if ( keysHeld[SDL_SCANCODE_G] )
			controls2->t.setOrigin(controls2->t.getOrigin()+controls2->t.getBasis()*btVector3(0, 0, 0.1));
		if ( keysHeld[SDL_SCANCODE_F] )
			controls2->t.setOrigin(controls2->t.getOrigin()+controls2->t.getBasis()*btVector3(-0.1, 0, 0));
		if ( keysHeld[SDL_SCANCODE_H] )
			controls2->t.setOrigin(controls2->t.getOrigin()+controls2->t.getBasis()*btVector3( 0.1, 0, 0));
		if ( keysHeld[SDL_SCANCODE_Y] )
			controls2->t.setOrigin(controls2->t.getOrigin()+controls2->t.getBasis()*btVector3(0, 0.1, 0));
		if ( keysHeld[SDL_SCANCODE_R] )
			controls2->t.setOrigin(controls2->t.getOrigin()+btVector3(0,-0.1, 0));

		if ( keysHeld[SDL_SCANCODE_I] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion( 0, 1*SIMD_RADS_PER_DEG, 0));
		if ( keysHeld[SDL_SCANCODE_K] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion( 0,-1*SIMD_RADS_PER_DEG, 0));
		if ( keysHeld[SDL_SCANCODE_J] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion( 1*SIMD_RADS_PER_DEG, 0, 0));
		if ( keysHeld[SDL_SCANCODE_L] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion(-1*SIMD_RADS_PER_DEG, 0, 0));
		if ( keysHeld[SDL_SCANCODE_U] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion( 0, 0, 1*SIMD_RADS_PER_DEG));
		if ( keysHeld[SDL_SCANCODE_O] )
			controls2->t.setRotation(controls2->t.getRotation()*btQuaternion( 0, 0,-1*SIMD_RADS_PER_DEG));

		//Prepare
		update();
		//Render quad
		render();
		//Update screen
		SDL_GL_SwapWindow( glWindow );
	}
	//Disable text input
	SDL_StopTextInput();
    //Free resources and close SDL
    close();
    return EXIT_SUCCESS;
}
