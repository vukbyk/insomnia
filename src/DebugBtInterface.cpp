/*
 * DebugBtInterface.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: vuk
 */

#include "DebugBtInterface.h"
#include "GL/gl.h"

DebugBtInterface::DebugBtInterface(int argDebugMode)
{
	debugMode = argDebugMode;
}

void DebugBtInterface::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
		glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		glVertex3f(from.getX(),from.getY(),from.getZ());
		glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		glVertex3f(to.getX(),to.getY(),to.getZ());
    glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
    glPopMatrix();
}

void DebugBtInterface::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    drawLine(from,to,color,color);
}

//void DebugBtInterface::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
//{
//    gl::color(Color(color.getX(), color.getY(), color.getZ()));
//    gl::drawSphere(Vec3f(p.getX(),p.getY(),p.getZ()), radius);
//}
//
//void DebugBtInterface::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color)
//{
//    gl::color(Color(color.getX(), color.getY(), color.getZ()));
//    gl::drawStrokedCube(AxisAlignedBox3f(
//        Vec3f(bbMin.getX(),bbMin.getY(),bbMin.getZ()),
//        Vec3f(bbMax.getX(),bbMax.getY(),bbMax.getZ())));
//}

//void DebugBtInterface::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
//{
//    Vec3f from(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
//    Vec3f to(normalOnB.getX(), normalOnB.getY(), normalOnB.getZ());
//    to = from+to*1;
//
//    gl::color(Color(color.getX(),color.getY(),color.getZ()));
//    gl::begin(GL_LINES);
//    gl::vertex(from);
//    gl::vertex(to);
//    gl::end();
//}

void DebugBtInterface::reportErrorWarning(const char* warningString)
{
    std::cout << warningString << std::endl;
}

void DebugBtInterface::draw3dText(const btVector3& location, const char* textString)
{
	std::cout << textString << std::endl;
//    TextLayout textDraw;
//    textDraw.clear(ColorA(0,0,0,0));
//    textDraw.setColor(Color(1,1,1));
//    textDraw.setFont(Font("Arial", 16));
//    textDraw.addCenteredLine(textString);
//    gl::draw(gl::Texture(textDraw.render()),
//        Vec2f(location.getX(),location.getY()));
}

DebugBtInterface::~DebugBtInterface()
{
	// TODO Auto-generated destructor stub
}

