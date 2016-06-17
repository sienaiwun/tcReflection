#include "fileRecorder.h"
class tcLivingRecorder:public FileRecorder
{
public:
	tcLivingRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/tcLivingScene.txt";
		m_avgFileName  = "script/tcLivingAvgScene.txt";
		m_maxFileName  = "script/tcLivingMaxScene.txt";
	}
};