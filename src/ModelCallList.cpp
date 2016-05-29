/*
 * ModelCallList.cpp
 *
 *  Created on: May 27, 2016
 *      Author: vuk
 */

#include "ModelCallList.h"
#include "AssimpModel.h"

ModelCallList::ModelCallList()
{
	idModel=0;
}

ModelCallList::ModelCallList(AssimpModel& argAssimp)
{
	idModel=argAssimp.getIdModel();
}

ModelCallList::ModelCallList(unsigned int argIdCallList)
{
	idModel=argIdCallList;
}

void ModelCallList::render()
{
	glPushMatrix();
		glMultMatrixf(getM());
		glCallList(idModel);
	glPopMatrix();
	Object3D::render();
}

ModelCallList::~ModelCallList()
{
	// TODO Auto-generated destructor stub
}

