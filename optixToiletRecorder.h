#include "fileRecorder.h"
class optixToiletRecorder:public FileRecorder
{
public:
	optixToiletRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/toiletOptixScene.txt";
		m_avgFileName  = "script/toiletOptixAvgScene.txt";
		m_maxFileName  = "script/toiletOptixMaxScene.txt";
	}
};