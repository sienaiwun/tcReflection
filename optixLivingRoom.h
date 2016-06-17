#include "fileRecorder.h"
class optixLivingRoomRecorder:public FileRecorder
{
public:
	optixLivingRoomRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/livingOptixScene.txt";
		m_avgFileName  = "script/livingOptixAvgScene.txt";
		m_maxFileName  = "script/livingOptixMaxScene.txt";
	}
};