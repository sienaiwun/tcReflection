#include "Cg/Cg.h"
#include "Cg/cgGL.h"
//#include "nvModel.h"
#include "float.h"
#include "stdlib.h"
#ifndef TRANSFORM_H
#define  TRANSFORM_H
class transform1
{
	float* transformMatrix;
	int startTime;
	int inverse;
	int dispear;
public:
	float* getMatrix(){return transformMatrix;};
	void draw(float * data,float *sourceData,int size,float time,float* beforMatrix,int isNormal);
	int endTime;
	transform1(float t[16],int time1,int time2,int disappera=0,int inverse = 0) ;
};


class action
{
	transform1 *tArray;
	int arraySize;
	int * gapArray;
	int index;
	int is_loop;
	int loopTime;
public:
	void draw(float * data,float *sourceData,int size,int isNormal = 0);
	action(transform1*,int size,int is_loop = 1);

};
#endif
