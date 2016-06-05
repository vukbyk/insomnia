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
    glBegin(GL_LINES);
		glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		glVertex3f(from.getX(),from.getY(),from.getZ());
		glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		glVertex3f(to.getX(),to.getY(),to.getZ());
    glEnd();
    glPopMatrix();
}

void DebugBtInterface::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    drawLine(from,to,color,color);
}

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

