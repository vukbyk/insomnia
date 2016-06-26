/*
 * Light.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: vuk
 */

#include "Light.h"

Light::Light()
{
	// TODO Auto-generated constructor stub

}

//void Light::update()
//{
//}

void Light::render()
{
	updated=false;
	if(on)
	{
		glPushMatrix();
		float pos[]= {tm->getOrigin().x(), tm->getOrigin().y(), tm->getOrigin().z(), 1};
	//    std::cout<<t->getOrigin().x()<< " " << t->getOrigin().y()<< " " << t->getOrigin().z()<< std::endl;
		glColor4f(1,1,1,1);
		glLightfv(GL_LIGHT1, GL_POSITION, pos);

		glEnable(GL_LIGHT1);
		btQuaternion dq(90*SIMD_DEGS_PER_RAD, 0*SIMD_DEGS_PER_RAD, 0*SIMD_DEGS_PER_RAD);

		btVector3 v=tm->getBasis()*btVector3(0,0,-1);
		v.normalize();
		float dif[]= {v.x(),v.y(),v.z(),0};
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dif);
//		if(on)
//			glDisable(GL_LIGHT1);
	//	glEnable(GL_LIGHT1);


		const GLfloat l[4]  = {1.0, 1.0, 1.0, 1.0};
		const GLfloat d[4]  = { .0,  .0,  .0, 1.0};
		glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 5.0);
		glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	//	glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.05f);
	//	glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.04f/*0.3f*/);
		glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001f);
		glLightfv(GL_LIGHT1, GL_AMBIENT,  on?l:d);
		glPopMatrix();
	}
	else
		glDisable(GL_LIGHT1);
	Object3D::render();
}

Light::~Light()
{
	// TODO Auto-generated destructor stub
}

