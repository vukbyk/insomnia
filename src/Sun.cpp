/*
 * Sun.cpp
 *
 *  Created on: Jun 25, 2016
 *      Author: vuk
 */

#include "Sun.h"

Sun::Sun()
{
	// TODO Auto-generated constructor stub

}

void Sun::render()
{
	updated=false;
	glPushMatrix();
    float pos[]= {tm->getOrigin().x(), tm->getOrigin().y(), tm->getOrigin().z(), 0};
//    std::cout<<t->getOrigin().x()<< " " << t->getOrigin().y()<< " " << t->getOrigin().z()<< std::endl;
    glColor4f(1,1,1,1);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    glEnable(GL_LIGHT0);
    btQuaternion dq(90*SIMD_DEGS_PER_RAD, 0*SIMD_DEGS_PER_RAD, 0*SIMD_DEGS_PER_RAD);

    btVector3 v=tm->getBasis()*btVector3(0,0,-1);
    v.normalize();
//	float dif[]= {v.x(),v.y(),v.z(),0};
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dif);
//	glDisable(GL_LIGHT0);

	// Light default parameters
	const GLfloat light_ambient[4]  = {.0, .0, .1, 1.0};
	const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
	const GLfloat light_diffuse[4]  = {1.0, 1.0, .9, 1.0};
//
//	glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 5.0);
//	glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
//	glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  0.05f);
//	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.04f/*0.3f*/);
//	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0001f);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glPopMatrix();
	Object3D::render();
}

Sun::~Sun()
{
	// TODO Auto-generated destructor stub
}

