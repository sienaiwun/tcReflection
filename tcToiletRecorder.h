#include "fileRecorder.h"
class tcToiletRecorder:public FileRecorder
{
public:
	tcToiletRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/tcOptixScene.txt";
		m_avgFileName  = "script/tcOptixAvgScene.txt";
		m_maxFileName  = "script/tcOptixMaxScene.txt";
	}
};