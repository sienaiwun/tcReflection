#include "macro.h"
#ifndef KEY_FRAME_H
#define KEY_FRAME_H
class KeyFrame
{
public:
	KeyFrame();
	int getOptixFrame(int);
	int getBeforeFrame(int);
	inline bool isKeyFrameChage()
	{
		return m_isCameraChange;
	}
private:
	bool m_isCameraChange;
	int m_LastKeyFrameNumber;
};
#endif