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
	void add(float value);
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
};
ostream & operator<<(ostream &out, frameStruct &s);
class FileRecorder
{
public:
	FileRecorder(int frame)
	{
		m_frameNum = frame;
		m_fileName = "toiletScene.txt";
		m_avgFileName  = "toiletAvgScene.txt";
	}

	void saveAvgFile();
	void readFile();
	void saveFile();
	void addValue(float value, int frame);
	void save();
	void init();
	
private:
	std::string m_fileName;
	std::string m_avgFileName;
	int m_frameNum;
	std::vector<frameStruct> m_frameVec;	
};
#endif