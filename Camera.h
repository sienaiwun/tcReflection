#ifndef _CAMERA_H
#define _CAMERA_H
#define SCREEN_WIDTH 1024								// We want our screen width 800 pixels
#define SCREEN_HEIGHT 1024	
#include "main.h"
#include<stdio.h>
#include <nvMath.h>
// This is our basic 3D point/vector class
class CVector3
{
public:

	// A default constructor
	CVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;						
};

typedef struct posPara 
{
	float3 pos;
	float3 view;
} posPara;
// This is our camera class
class CCamera {

public:

	// Our camera constructor
	CCamera();	

	CCamera(CVector3,CVector3,CVector3);

	// These are are data access functions for our camera's private data
	CVector3 Position() {	return m_vPosition;		}
	CVector3 View()		{	return m_vView;			}
	CVector3 UpVector() {	return m_vUpVector;		}
	CVector3 Strafe()	{	return m_vStrafe;		}
	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(CVector3 vCenter, float X, float Y, float Z);

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

	// This uses gluLookAt() to tell OpenGL where to look
	void Look();

	// This returns the inverse to the current modelview matrix in OpenGL
	void GetInverseMatrix(float mCameraInverse[16]);
	void navigate(posPara pos1,posPara pos2,int currentTime,int startTime,int endTime);
	

	void CoutCamera();

	inline  float * getModelViewMat()
	{
		return (float*)m_modelView.get_value();
	}
	inline  float * getMvpMat()
	{
		return (float*)m_mvpMat.get_value();
	}
	inline  float * getModelViewInvMat()
	{
		return (float*)inverse(m_modelView).get_value();
	}
	inline nv::vec3f getCameraPos()
	{
		return nv::vec3f(m_vPosition.x,m_vPosition.y,m_vPosition.z);
	}
private:
	// The camera's position
	CVector3 m_vPosition;					

	// The camera's view
	CVector3 m_vView;						

	// The camera's up vector
	CVector3 m_vUpVector;		
	
	// The camera's strafe vector
	CVector3 m_vStrafe;

	nv::matrix4f m_modelView;
	nv::matrix4f m_mvpMat;
	nv::matrix4f m_projMat;
	float old_x ,old_y;
};


// This makes sure we only draw at a specified frame rate
bool AnimateNextFrame(int desiredFrameRate);


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for our current tutorial.
// 
// 
// ?000-2005 GameTutorials