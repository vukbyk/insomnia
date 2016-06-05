/*
 * DebugBtInterface.h
 *
 *  Created on: Jun 4, 2016
 *      Author: vuk
 */

#ifndef DEBUGBTINTERFACE_H_
#define DEBUGBTINTERFACE_H_

#include <LinearMath/btIDebugDraw.h>
#include <iostream>

class DebugBtInterface : public btIDebugDraw
{
	int debugMode;
public:
	DebugBtInterface(int argDebugMode=0);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color){};
    virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color){};
    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color){};
    virtual void reportErrorWarning(const char* warningString);
    virtual void draw3dText(const btVector3& location, const char* textString);
    virtual void setDebugMode(int argDebugMode) { debugMode=argDebugMode; }
	int getDebugMode() const { return debugMode; }
	virtual ~DebugBtInterface();
};

#endif /* DEBUGBTINTERFACE_H_ */
