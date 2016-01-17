//*********************************************************************//
//																		 //
//		- "Talk to me like I'm a 3 year old!" Programming Lessons -		 //
//                                                                       //
//		$Author:		Ben Humphrey	digiben@gametutorilas.com		 //
//																		 //
//		$Program:		ShadersShadowMappingPCF							 //
//																		 //
//		$Description:	Creating higher quality shadowmaps with GLSL	 //
//																		 //
//***********************************************************************//

#pragma comment(lib, "winmm.lib")

#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <gl/glext.h>
#include <GL/glew.h>
#include <string>		// Used for our STL string objects
#include<iostream>

#include <fstream>	
#include "Camera.h"

// We increased the speed a bit from the Camera Strafing Tutorial
// This is how fast our camera moves
#define kSpeed	10.0f									

// Our global float that stores the elapsed time between the current and last frame
float g_FrameInterval = 0.2f;


/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function clamps a specified frame rate for consistency
/////
/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*


/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
												
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	// Calculate the cross product with the non cumulative equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Normalize(CVector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}


///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is the class constructor
/////
///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		// Init a vector to 0 0 0 for our position
	CVector3 vView = CVector3(0.0, 1.0, 0.5);		// Init a starting view vector (looking up and out the screen) 
	CVector3 vUp   = CVector3(0.0, 0.0, 1.0);		// Init a standard up vector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector

	// Set the mouse position to the middle of our window
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);							
}

CCamera::CCamera(CVector3 vZero,CVector3 vView,CVector3 vUp){
	
	m_vPosition = vZero;
	m_vView		= vView	;				// Init the view to a std starting view
	m_vUpVector	= vUp;	


}


///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the camera's position and view and up vector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vPosition	= CVector3(positionX, positionY, positionZ);
	CVector3 vView		= CVector3(viewX, viewY, viewZ);
	CVector3 vUpVector	= CVector3(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
	//std::cout<<m_vPosition.x<<std::endl;
	//printf("asd\n");
}


///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
inline void normalizeR(float *v)
	{
		float magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
		v[0] /= magnitude;
		v[1] /= magnitude;
		v[2] /= magnitude;
	}
void CCamera::SetViewByMouse()
{
#define sensitivity  0.005f
	 
	float rot_x, rot_y;
	float rot_axis[3];

	POINT mousePos;	

	GetCursorPos(&mousePos);	// 获得鼠标的当前位置

	rot_x = -(float)(mousePos.x - old_x) *sensitivity;
	rot_y = -(float)(mousePos.y - old_y) *sensitivity;

	old_x = mousePos.x;
	old_y = mousePos.y;

	CVector3 direction = m_vView-m_vPosition;
	float * vView = (float*)&direction;
	if(GetKeyState(VK_LBUTTON) & 0x80)
	{
		RotateView(rot_x, 0.0f, 1.0f, 0.0f);

		rot_axis[0] = -vView[2];
		rot_axis[1] = 0.0f;
		rot_axis[2] = vView[0];

		normalizeR(rot_axis);

		RotateView(rot_y, rot_axis[0], rot_axis[1], rot_axis[2]);
	}
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(float angle, float x, float y, float z)
{
	CVector3 vNewView;

	// Get the view vector (The direction we are facing)
	CVector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;


}


///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::StrafeCamera(float speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	CVector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.y += vVector.y * speed;		// Add our acceleration to our position's Y
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.y += vVector.y * speed;			// Add our acceleration to our view's Y
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}


//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::CheckForMovement()
{	
	// Once we have the frame interval, we find the current speed
	float speed = kSpeed * g_FrameInterval;

	// Check if we hit the Up arrow or the 'w' key
	if(GetKeyState('W') & 0x80) {				

		// Move our camera forward by a positive SPEED
		MoveCamera(speed);				
	}

	// Check if we hit the Down arrow or the 's' key
	if(GetKeyState('S') & 0x80) {			

		// Move our camera backward by a negative SPEED
		MoveCamera(-speed);				
	}

	// Check if we hit the Left arrow or the 'a' key
	if(GetKeyState('A') & 0x80) {			

		// Strafe the camera left
		StrafeCamera(-speed);
	}

	// Check if we hit the Right arrow or the 'd' key
	if(GetKeyState('D') & 0x80) {			

		// Strafe the camera right
		StrafeCamera(speed);
		//CoutCamera();
	}	
}
void CCamera::CoutCamera(){
	printf("%f %f %f\n%f %f %f\n%f %f %f\n",m_vPosition.x,m_vPosition.y,m_vPosition.z,m_vView.x,m_vView.y,m_vView.z,m_vUpVector.x,m_vUpVector.y,m_vUpVector.z);
}

///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera's view and strafe vector
/////
///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Update() 
{
	// Initialize a variable for the cross product result
	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	// Normalize the strafe vector
	m_vStrafe = Normalize(vCross);

	// Move the camera's view by the mouse
	SetViewByMouse();

	// This checks to see if the keyboard was pressed
	CheckForMovement();
	
	// Calculate our frame rate and set our frame interval for time-based movement
	//CalculateFrameRate();
}


///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera according to the 
/////
///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Look()
{
	// Give OpenGL our camera position, then camera view, then camera up vector
	#define squire3 (1.732050807/3)
	m_modelView = nv::matrix4f::lookAt(nv::vec3f(m_vPosition.x, m_vPosition.y, m_vPosition.z),	
			 nv::vec3f( m_vView.x,	 m_vView.y,     m_vView.z),	
			  nv::vec3f(m_vUpVector.x, m_vUpVector.y, m_vUpVector.z));
	m_projMat = nv::matrix4f::frustum(-squire3,squire3,-squire3,squire3,1,1000);
	m_mvpMat =  m_projMat * m_modelView;
	
}


/////////////////////////////// GET INVERSE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the inverse to the current modelview matrix in OpenGL
/////
/////////////////////////////// GET INVERSE MATRIX \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::GetInverseMatrix(float mCameraInverse[16])
{
	float m[16] = {0};

	// Grab the current modelview matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	// Below we do a little algebra to invert our current modelview matrix.
	// Instead of doing a more complicated method of determining the inverse
	// of a matrix, we can use this simplified version because we assume our
	// matrix is an orthogonal matrix, meaning that each column is perpendicular
	// to the other column.  What we do is remove the translation of the matrix
	// and then multiply it later by the negative translation, which reverses the
	// calculations of the original matrix.
	mCameraInverse[0]  = m[0]; mCameraInverse[1] = m[4]; mCameraInverse[2]  = m[8];
	mCameraInverse[4]  = m[1]; mCameraInverse[5] = m[5]; mCameraInverse[6]  = m[9];
	mCameraInverse[8]  = m[2]; mCameraInverse[9] = m[6]; mCameraInverse[10] = m[10];
	mCameraInverse[3]  = 0.0f; mCameraInverse[7] = 0.0f; mCameraInverse[11] = 0.0f;
	mCameraInverse[15] = 1.0f;

	mCameraInverse[12] = -(m[12] * m[0]) - (m[13] * m[1]) - (m[14] * m[2]);
	mCameraInverse[13] = -(m[12] * m[4]) - (m[13] * m[5]) - (m[14] * m[6]);
	mCameraInverse[14] = -(m[12] * m[8]) - (m[13] * m[9]) - (m[14] * m[10]);
}

void CCamera::navigate(posPara pos1,posPara pos2,int currentTime,int startTime,int endTime)
{
	float3 cameraPosition,cameraView;
	float radio = ((float)(currentTime-startTime))/((float)(endTime-startTime));
	float3 view1 = pos1.pos+normalize(pos1.view-pos1.pos);
	float3 view2 = pos2.pos+normalize(pos2.view-pos2.pos);

	cameraPosition= pos1.pos*(1-radio)+pos2.pos*radio;
	cameraView = view1*(1-radio)+view2*radio;
	PositionCamera(cameraPosition.x,cameraPosition.y,cameraPosition.z,cameraView.x,cameraView.y,cameraView.z,0,1,0);
}
/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for our current tutorial.
//
//
// ?000-2005 GameTutorials