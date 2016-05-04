/*
 * AssetManager.h
 *
 *  Created on: Apr 30, 2016
 *      Author: vuk
 */

#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

class AssetManager
{
protected:
    float *vertexArray;
    float *normalArray;
    float *uvArray;
    unsigned int numVerts;
    unsigned int idTexture;
public:
	AssetManager();
	virtual ~AssetManager();
};

#endif /* ASSETMANAGER_H_ */
