#ifndef OBJECTDIS_H
#define OBJECTDIS_H
#include <string>
// scene discription file
class objDis
{
public:
	string fileName;
	int hasTx;
	string texName;
	nv::vec3f kd;
	float reflectFactor;
};

#endif