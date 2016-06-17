#include "main.h"
#include "fpsCount.h"
#include "time.h"
#ifndef TIMEMESURE_H
#define TIMEMESURE_H

enum TimeType
{
	optixRenderingType = 0,
	tcRenderingType  = 1,
	hybridRenderingType = 2,
	noGeometryRenderingType = 3,
	noGeometryTcType = 4,
	compareType = 5,
	superImageType = 6,

};
class TimeMesure
{
private:
	TimeType m_type;
	float m_lastFrameEndTime;
	float m_frameBeginTime,m_traceBeginTime,m_finalRenderingBeginTime,m_frameEndTime;
	float m_cudaBeginTime,m_forwardTime,m_secondTraceTime,m_imageProjStartTime;
	float m_radio;
	int m_frame;
	int m_totalTime;
	static float last_frame_time ;
	static float last_frame_time2 ;
	static float currentFps;
	static int frame_count ;
	FPS m_fcount;
	float m_ratio;
public:
	static inline double getCurrentTime()
	{
		return clock();
	}
	bool needPreocompute()
	{
		return (m_type!=optixRenderingType);
		return (m_type==noGeometryTcType)|| (m_type==tcRenderingType);
	}
	inline TimeType getType()
	{
		return m_type;
	}
	int getFrame()
	{
		return m_frame;
	}
	float getFps()
	{
		return  1000/(m_frameEndTime - m_lastFrameEndTime);
	}
	inline void setOptixBeginTime(float m)
	{
		m_traceBeginTime = m;
	}
	inline void setFinalRenderingTime(float m)
	{
		m_finalRenderingBeginTime= m;
	}
	inline void setRatio(float v)
	{
		m_radio = v;
	}
	inline void setCudaBeginTime(float m)
	{
		m_cudaBeginTime = m;
	}
	inline void setImageProjectStartTime(float m)
	{
		m_imageProjStartTime = m;
	}
	inline void setForwardingTime(float m)
	{
		m_forwardTime = m;
	}
	inline void setSecondTraceTime(float m)
	{
		m_secondTraceTime = m;
	}
	inline void setCudaTime(float m)
	{
		m_cudaBeginTime = m;
	}
	inline void setBeginTime(float m)
	{
		m_frameBeginTime = m;
	}
	inline void setEndTime(float m)
	{
		m_frameEndTime = m;
	}
	inline void setRadio(float m)
	{
		m_radio = m;
	}
	TimeMesure(TimeType t,int totalTime):m_frame(0),m_totalTime(totalTime)
	{

		m_frame = 4;//debug 
		m_type = t;
		m_fcount = FPS(110);
	};
	inline void setTotalTime(int m)
	{
		m_totalTime = m_totalTime;
	}
	void print();
	void printTC();
	void printOptix();
	void printHybrid();
	void printNoGeometry();
	void nextFrame(int& time );
	void previousFrame();
	void finishFuc();
	void updateLastTime();
private:
	
};
 
#endif