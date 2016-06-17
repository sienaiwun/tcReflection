#include "keyFrame.h"
#include <assert.h>
KeyFrame::KeyFrame():m_isCameraChange(true)
{
	m_LastKeyFrameNumber = -1;
}
int KeyFrame::getBeforeFrame(int frameNumber)
{
	extern int BEGININDEX;
	extern int ENDINDEX;
	assert(frameNumber>=BEGININDEX&&frameNumber<=ENDINDEX);
	int OptixFrame = (frameNumber-BEGININDEX) /JianGe;
	if(OptixFrame == m_LastKeyFrameNumber)
	{
		m_isCameraChange = false;
	}
	else
	{
		m_isCameraChange = true;	
	}
	m_LastKeyFrameNumber = OptixFrame;
	return OptixFrame;
}
int KeyFrame::getOptixFrame(int frameNumber)
{
	return getBeforeFrame(frameNumber);
	extern int BEGININDEX;
	extern int ENDINDEX;
	assert(frameNumber>=BEGININDEX&&frameNumber<=ENDINDEX);
	int OptixFrame = (frameNumber-BEGININDEX) /JianGe;
	if(frameNumber %JianGe> JianGe/2)
		OptixFrame++;
	if(OptixFrame == m_LastKeyFrameNumber)
	{
		m_isCameraChange = false;
	}
	else
	{
		m_isCameraChange = true;	
	}
	m_LastKeyFrameNumber = OptixFrame;
	return OptixFrame;
}

