/*
 * AssimpModel.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: vuk
 */

#include "AssimpModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL2/SDL_image.h>

AssimpModel::AssimpModel()
{
	modelFile = "models/trup.obj";
	textureFile = "models/trup.jpg";
    vertexArray = NULL;
    normalArray = NULL;
    uvArray = NULL;
    image = NULL;
    numVerts = 0;
}

AssimpModel::AssimpModel(int idm)
{
	idModel = idm;
	idTexture = idm;
    vertexArray = NULL;
    normalArray = NULL;
    uvArray = NULL;
    image = NULL;
    numVerts = 0;
}


AssimpModel::AssimpModel(string model, string texture)
{
	modelFile = model;
	textureFile = texture;
    vertexArray = NULL;
    normalArray = NULL;
    uvArray = NULL;
    image = NULL;
    numVerts = 0;
}

void AssimpModel::render()
{
	glPushMatrix();
		glMultMatrixf(getM());
		glCallList(idModel);
	glPopMatrix();
	Object3D::render();
}

void AssimpModel::init()
{
	if(idModel==0)
	{
		idModel = ++idModel_counter;
		idTexture = idModel;
		loadModel(modelFile.c_str());
		loadTexture(textureFile.c_str());
		glNewList(idModel, GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, idTexture);
			glBegin(GL_TRIANGLES);
				for(uint i=0; i < numVerts; i++)
				{
					glTexCoord2d(uvArray[2*i], 1-uvArray[2*i+1]);
					glNormal3f(normalArray[3*i],normalArray[3*i+1],normalArray[3*i+2]);
					glVertex3f(vertexArray[3*i],vertexArray[3*i+1],vertexArray[3*i+2]);
				}
			glEnd();
		glEndList();
	}

    Object3D::init();
}

void AssimpModel::loadModel(const char* modelFile)
{
    Assimp::Importer importer;
    //aiProcessPreset_TargetRealtime_Fast has the configs you'll need
    const aiScene *scene = importer.ReadFile(modelFile, aiProcessPreset_TargetRealtime_Fast);
    //assuming you only want the first mesh
    aiMesh *mesh = scene->mMeshes[0];

    numVerts = mesh->mNumFaces*3;

    vertexArray = new float[mesh->mNumFaces*3*3];
    normalArray = new float[mesh->mNumFaces*3*3];
    uvArray = new float[mesh->mNumFaces*3*2];

    for(unsigned int i=0;i<mesh->mNumFaces;i++)
    {
    const aiFace& face = mesh->mFaces[i];

        for(int j=0;j<3;j++)
        {
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray,&uv,sizeof(float)*2);
            uvArray+=2;

            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray,&normal,sizeof(float)*3);
            normalArray+=3;

            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray,&pos,sizeof(float)*3);
            vertexArray+=3;
        }
    }

    uvArray-=mesh->mNumFaces*3*2;
    normalArray-=mesh->mNumFaces*3*3;
    vertexArray-=mesh->mNumFaces*3*3;
}

void AssimpModel::loadTexture(const char* textureFile)
{
	SDL_Surface* img = IMG_Load(textureFile);
	if (!img)
	{
		printf("Error: \"%s\"\n",SDL_GetError());
	}
	image=SDL_ConvertSurfaceFormat(img,SDL_PIXELFORMAT_RGBA8888,SDL_SWSURFACE);
	SDL_FreeSurface(img);
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);


//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->w,image->h,0,GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,image->pixels);
}
AssimpModel::~AssimpModel()
{
	// TODO Auto-generated destructor stub
}

