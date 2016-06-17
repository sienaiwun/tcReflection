
#include "fileRecorder.h"
ostream & operator<<(ostream &out, frameStruct &s)
{
	std::string gap = std::string(" ");
	out << s.maxValue << gap << s.totalValue << gap  << s.minValue <<gap<<s.m_index <<endl;
    return out;

 }
istream & operator>>(istream &in, frameStruct &s)
{
	std::string gap = std::string(" ");
    in >> s.maxValue >> gap >> s.totalValue >> gap  >> s.minValue >>gap >> s.m_index; ;

    return in;
 }
frameStruct::frameStruct()
{
	m_index = -1;
	validNumber = 1;
	minValue = 999.0;
	maxValue = -1;
	totalValue = 0;
	validNumber = 0;
}
void frameStruct::add(float value)
{
	maxValue = std::max<float>(value,maxValue);
	minValue = std::min<float>(value,minValue);
	totalValue += value;
	validNumber +=1;
}


void FileRecorder::readFile()
{
	ifstream ifile;
	ifile.open(m_fileName);
	if (ifile.fail()) 
	{
		ifile.close();
		return;
	}
	frameStruct currentOne;
	for(int i =0;i<m_frameNum;i++)
	{
		ifile >> currentOne;
		if(currentOne.isValid())
		{
			if(currentOne.isValid())
			{
				int index = currentOne.getIndex();
				m_frameVec[index] = currentOne;
			}
		}
	}
	ifile.close();
}
void FileRecorder::init()
{
	m_frameVec.resize(m_frameNum);
	readFile();
}
void FileRecorder::saveAvgFile()
{
	
	ofstream ofile;
	ofile.open(m_avgFileName);
	if (ofile.fail()) 
	{
		ofile.close();
		return;
	}
	for(int i =0;i<m_frameNum;i++)
	{
		ofile<<m_frameVec[i].getAvgNumber()<<",";
	}
	ofile.close();
}
void FileRecorder::addValue(float value, int index)
{
	m_frameVec[index].add(value);
}
void FileRecorder::saveFile()
{
	ofstream ofile;
	ofile.open(m_fileName);
	if (ofile.fail()) 
	{
		ofile.close();
		return;
	}
	for(int i =0;i<m_frameNum;i++)
	{
		ofile<<m_frameVec[i];
	}
	ofile.close();
}