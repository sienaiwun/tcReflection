#include <string>
#include <vector>
 #include<iostream>
#include <fstream>
#ifndef FILERECORDER_H
#define FILERECORDER_H
using namespace std;
 class frameStruct
{
public :
	float maxValue;
	float totalValue;
	float minValue;
	int validNumber;
	int m_index;
	frameStruct();
	void add(float value,int index);
	inline bool isValid()
	{
		return (m_index != -1);
	}
	inline int getIndex()
	{
		return m_index;
	}
	inline float getAvgNumber()
	{
		if(validNumber<=0)
		{
			return 0;
		}
		return totalValue/validNumber;
	}
	inline float getMaxNumber()
	{
		return maxValue;
	}
};
ostream & operator<<(ostream &out, frameStruct &s);
 class FileRecorder
{
public:
	FileRecorder(){};
	 FileRecorder(int frame)
	{
	
	}
	void debug(int begin,int end);
	void saveAvgFile();
	void saveMinFile();
	void readFile();
	void saveFile();
	void addValue(float value, int frame);
	void init();
	void save();
	inline std::vector<frameStruct> & getVec()
	{
		return m_frameVec;
	}
protected:
	std::string m_fileName;
	std::string m_avgFileName;
	std::string m_maxFileName;
	int m_frameNum;
	std::vector<frameStruct> m_frameVec;	
};
#endif