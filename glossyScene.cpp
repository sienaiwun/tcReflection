#include "glossyScene.h"
#define  geoNumber  9
#define  CHINAREFLECT 0.185f
static nv::vec3f toiletLightPos =  nv::vec3f(0.0f, 65.0f, 30.0f);
static MyMeterial glossyMat[] = 
{
	//"./model/cheat.jpg",

	"./glossyModel/woodFloor.jpg",  //0
	"./glossyModel/wood.jpg",    //1
	"./glossyModel/redstone.jpg",  //2
	"./glossyModel/book2.jpg",	//3
	"./glossyModel/book.jpg",     //4
	"./glossyModel/japen2.jpg",   //5
	"./wp/up.jpg",    //6
	"./glossyModel/photo.jpg",     //7
	"./glossyModel/picture.jpg",   //8
	"./glossyModel/sofa.jpg",      //9
	"./aomodel/aoWest.jpg",         //10 west
	"./aomodel/aoEast.jpg",      //11  esat
	"./aomodel/aoNorth.jpg" ,  //12   north
	"./wp/south.jpg",       //13  south
	"./glossyModel/sphere.jpg",      //9
	"./vase/test3-5.jpg",     //15
	"./glossy/tv.jpg",     //16
	make_float3(0.9,0.9,0.9),     //17

};

static GeoPara glossyObj[geoNumber] = 
{
	{"./glossy/wallsouth2.obj",      13,0,0,0},
    {"./glossy/lw.obj",      16,0,0,0},
	{"./glossy/lw4.obj",      16,0,0,0},
	{"./glossyModel/floor4.obj",      17,0,CHINAREFLECT,0},
	{"./aomodel/aoWest.obj",      10,0,0,0},
	{"./glossyModel/ceil4225.3ds",      6,0,0,0},
	{"./aomodel/aoEast.obj",       11,0,0,0},
	{"./aomodel/aoNorthShift1089.3DS",      12,0,0,0},
	{"./glossyModel/floor2.obj",           0,0,0,0},


};
glossyScene::glossyScene()
{
	m_objectNum = geoNumber;	
	setMainLightPos(toiletLightPos);
	m_pObjectDis = glossyObj;
	 m_pMatDis = glossyMat;
}