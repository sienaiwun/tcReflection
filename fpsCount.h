#define BEGINFRAMECOUNT 10
#ifndef FPSCOUNT_H
#define FPSCOUNT_H
class FPS
{
public:

	/*
	计算平均反应时间，从BEGINFRAMECOUNT 开始计数 知道某一个时间点
	*/
	FPS( )
	{
	}
	/*FPS(int time):nCount(time):nCount(time),minFps(0),maxFps(0),avgFps(0),m_isReady(false)
	{
		timeS = (float *)malloc(nCount*sizeof(float));
		beginWrite =false;
		isOnce = true;
	}*/
	FPS(int time, char * filename = "writeFile.txt"):nCount(time),minFps(0),maxFps(0),avgFps(0),m_isReady(false)
	{
		timeS = (float *)malloc(nCount*sizeof(float));
		beginWrite = false;
		isOnce = true;
	};
	void insertTime(float value)
	{
		static int id = 0;
		if(!m_isReady)
		{
			timeS[id] = value;
			id++;
			if(id>nCount)
			{
				m_isReady = true;
			}
		}
	}
	inline bool isReady()
	{
		return m_isReady;
	}
	void insertTime(int index,float value)
	{
		if(index>nCount-1)
			return;
		timeS[index] = value;
		//printf("%f %d\n",timeS[index],index);
	}
	void countResult()
	{
		float sumFps = 0.0f;
		minFps  = 100.0f;
	/*	if(!isOnce)
			return;
		if(!beginWrite)
			MinusCaculate();*/
		for(int i =BEGINFRAMECOUNT;i<nCount;i++)
		{
			
			if(minFps>timeS[i])
			{
				minFps = timeS[i];
			}
			if(maxFps<timeS[i])
			{
				maxFps = timeS[i];
			}
			sumFps+=timeS[i];
		}
		avgFps = sumFps/(nCount-BEGINFRAMECOUNT);	
		beginWrite = false;
		isOnce = false;
	}
	void MinusCaculate()
	{
		FILE *loadFile = fopen("writeFile1.txt","r");
		for(int i =BEGINFRAMECOUNT;i<nCount;i++)
		{
			float readNum ;
			fscanf(loadFile,"%f",&readNum);
			timeS[i]-= readNum;
		}
	}
	float getMinimum() const 	
	{
		return minFps;
	}
	float getMaximum() const 
	{
		return maxFps;
	}
	float getAverge() const 
	{
		return avgFps;
	}
private:
	float *timeS;
	int nCount;
	float minFps,maxFps,avgFps;
	FILE *mfile;
	bool beginWrite;
	bool isOnce ;
	bool m_isReady;
};
#endif