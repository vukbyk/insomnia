/*
 * AssimpModel.h
 *
 *  Created on: Apr 28, 2016
 *      Author: vuk
 */

#ifndef ASSIMPMODEL_H_
#define ASSIMPMODEL_H_

#include "Object3D.h"
#include <iostream>
//#include <SDL2/SDL_image.h>

struct SDL_Surface;
using namespace std;

class AssimpModel: public Object3D
{

protected:
	unsigned int idModel=0;
    unsigned int idTexture=0;

	string textureFile;
	string modelFile;
	SDL_Surface *image;
    float *vertexArray;
    float *normalArray;
    float *uvArray;
    unsigned int numVerts;

public:
	AssimpModel();
	AssimpModel(int idm);
	AssimpModel(string, string);
	virtual void render();
	virtual void initGL();
	virtual void loadModel(const char* modelFile);
	virtual void loadTexture(const char* textureFile);
	virtual ~AssimpModel();

	unsigned int getIdModel() const
	{
		return idModel;
	}
};

#endif /* ASSIMPMODEL_H_ */
