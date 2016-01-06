#define BEGINFRAME 10
#ifndef FPSCOUNT_H
#define FPSCOUNT_H
class FPS
{
public:
	FPS( )
	{
	}
	FPS(int time, char * filename = "writeFile.txt"):nCount(time),minFps(0),maxFps(0),avgFps(0)
	{
		mfile = fopen(filename,"w");
		if(mfile == NULL)
			exit(-1);
		timeS = (float *)malloc(nCount*sizeof(float));
		beginWrite =true;
		isOnce = true;
	};
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
		for(int i =BEGINFRAME;i<nCount;i++)
		{
			if(beginWrite)
				fprintf(mfile,"%f\n",timeS[i]);

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
		avgFps = sumFps/(nCount-1);	
		beginWrite = false;
		isOnce = false;
	}
	void MinusCaculate()
	{
		FILE *loadFile = fopen("writeFile1.txt","r");
		for(int i =BEGINFRAME;i<nCount;i++)
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
};
#endif