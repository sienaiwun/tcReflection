#include "fileRecorder.h"
class tcToiletRatioRecorder:public FileRecorder
{
public:
	tcToiletRatioRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/tcToiletRatio.txt";
		m_avgFileName  = "script/tcToiletRatioAvg.txt";
		m_maxFileName  = "script/tcToiletRatioMax.txt";
	}
};