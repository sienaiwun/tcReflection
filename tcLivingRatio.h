#include "fileRecorder.h"
class tcLivingRatioRecorder:public FileRecorder
{
public:
	tcLivingRatioRecorder(int num)
	{
		m_frameNum = num;
		m_fileName = "script/tcLivingRatio.txt";
		m_avgFileName  = "script/tclivingtRatioAvg.txt";
		m_maxFileName  = "script/tclivingRatioMax.txt";
	}
};