/*
 * ModelCallList.h
 *
 *  Created on: May 27, 2016
 *      Author: vuk
 */

#ifndef MODELCALLLIST_H_
#define MODELCALLLIST_H_

#include "Object3D.h"
class AssimpModel;

class ModelCallList: public Object3D
{
	unsigned int idModel;
public:
	ModelCallList();
	ModelCallList(AssimpModel &argAssimp);
	ModelCallList(unsigned int argIdCallList);
	virtual void render();
	virtual ~ModelCallList();

	unsigned int getIdModel() const { return idModel; }
	void setIdModel(unsigned int idModel) { this->idModel = idModel; }
};

#endif /* MODELCALLLIST_H_ */
