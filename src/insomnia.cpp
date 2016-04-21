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

//#include <btBulletDynamicsCommon.h>

#include "Object3D.h"

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

//Input handler
void handleKeys( unsigned char key, int x, int y );

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

//Object3D cube;

typedef std::shared_ptr <Object3D> ObjectPtr;
typedef std::vector <ObjectPtr> Objects;
Objects objects;

int init()
{
//	Object o=(new Object3D());
//	shared_ptr <Object3D> o;

	objects.push_back(ObjectPtr(new Object3D()));
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

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    float dif[]= {1,1,1,1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

    glEnable(GL_DEPTH_TEST);
    //Initialize Projection Matrix
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
    gluLookAt (0.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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


void handleKeys( unsigned char key, int x, int y )
{
    if( key == 'q' )
    {
        quit = true;
    }
    if ( key == 'z')
    {
//    	cube.t.setRotation();
    }
}

void update()
{
    //No per frame update needed
}

float r=0;

void render()
{
	btTransform camera;
	camera.setIdentity();
	camera.setOrigin(btVector3(1.1,1.1,6));
	camera=camera.inverse();
	camera.setRotation(btQuaternion(0, 30*SIMD_PI/180, 0));
//	camera.setRotation(btQuaternion(30*SIMD_PI/180, 0, 0));
	float cmat[16];
	camera.getOpenGLMatrix(cmat);

//	    gluLookAt (0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    //Clear color buffer
	glDepthMask (true);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	gluLookAt (0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//    glPushMatrix();
    glPushMatrix();
//	    cube.render();
    	objects[0]->render();
	glPopMatrix();
//	camera.setOrigin(btVector3(15,10,0));
//	camera.getOpenGLMatrix(cmat);
//	glLoadMatrixf(cmat);

    glPushMatrix();
		glColor3f(.8,.8,.8);
		float span=10;
		glLineWidth(3);
		glBegin(GL_LINES);
			glNormal3f(0,1,0);
			glVertex3f(-span, 0, 0);
			glVertex3f(span, 0, 0);
			glVertex3f(0, 0,-span);
			glVertex3f(0, 0,span);

			glVertex3f(-span, 0, span);
			glVertex3f(span, 0, span);
			glVertex3f(-span, 0, -span);
			glVertex3f(span, 0, -span);
			glVertex3f(-span, 0, -span);
			glVertex3f(-span, 0, span);
			glVertex3f(span, 0, -span);
			glVertex3f(span, 0, span);
		glEnd();
		glLineWidth(1);

		glBegin(GL_LINES);
			for(int i=1; i<span; i++)
			{
				glVertex3f(-span, 0, i);
				glVertex3f(span, 0, i);
				glVertex3f(-span, 0, -i);
				glVertex3f(span, 0, -i);
				glVertex3f(i, 0, -span);
				glVertex3f(i, 0, span);
				glVertex3f(-i, 0, -span);
				glVertex3f(-i, 0, span);
			}
		glEnd();
	glPopMatrix();

    glPushMatrix();
    	glRotatef(r,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
		r+=1;
		glColor3f(0.0f,1.0f,0.0f);
		glBegin( GL_QUADS );
			glVertex3f( -0.5f, -0.5f, .5f );
			glVertex3f(  0.5f, -0.5f, .5f );
			glVertex3f(  0.5f,  0.5f, .5f );
			glVertex3f( -0.5f,  0.5f, .5f );
		glEnd();
	glPopMatrix();

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
			objects[0]->t.setRotation(objects[0]->t.getRotation()*btQuaternion(0,  1*SIMD_RADS_PER_DEG, 0));
		if ( keysHeld[SDL_SCANCODE_UP] )
			objects[0]->t.setRotation(objects[0]->t.getRotation()*btQuaternion(0, -1*SIMD_RADS_PER_DEG, 0));
		if ( keysHeld[SDL_SCANCODE_LEFT] )
			objects[0]->t.setRotation(objects[0]->t.getRotation()*btQuaternion(0, 0, -1*SIMD_RADS_PER_DEG));
		if ( keysHeld[SDL_SCANCODE_RIGHT] )
			objects[0]->t.setRotation(objects[0]->t.getRotation()*btQuaternion(0, 0,  1*SIMD_RADS_PER_DEG));
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
