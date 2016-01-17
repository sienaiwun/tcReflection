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

static posPara glossyArray[] =
{
	//{make_float3(16.033649,38.942272,15.414365  ),make_float3(22.289124,34.077927,9.314583 )},
	//{make_float3(20.541248,38.556011,16.854353  ),make_float3(24.467501,33.042343,9.493363 )},
	//{make_float3(18.591887,38.901070,18.539888  ),make_float3(22.336548,34.001892,10.667604 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},

	//{make_float3(24.898901,45.217281,-10.226630  ),make_float3(25.289194,38.099701,-3.213274 )},
	//	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(19.777601,40.256077,18.724140  ),make_float3(21.469124,33.352386,11.690137 )},
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},
	//{make_float3(18.743200,38.181194,-3.370136  ),make_float3(23.238724,31.557341,2.622727 )},
	//{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(23.576305,42.725662,17.686817 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.228128,47.533394,25.267078 )},
	//{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.231396,47.535080,25.265717 )},
	//{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},
	//{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},

	//1st
	//	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(0.809388,32.024666,27.863651 )},//
	//Burning add
	//{make_float3(12.813933,30.045429,23.866928),make_float3(16.026863,27.571392,14.726033)},
	{make_float3(-23.214741,132.878403,104.965492  ),make_float3(-23.922035,132.399780,105.471191 )},
	{make_float3(-64.204552,128.025604,54.879829  ),make_float3(-64.709442,127.616676,55.533115 )},
	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},//5

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7
	//10
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7

	{make_float3(13.948970,43.934372,17.002832  ),make_float3(18.528547,37.866623,10.505962 )},//14
	//{make_float3(4.111155,44.597073,8.203527  ),make_float3(12.948935,40.556053,5.844954 )},
	//{make_float3(3.722486,49.674381,28.524050  ),make_float3(10.202328,45.074345,22.453533 )},//7
	{make_float3(6.768429,44.201805,11.434514  ),make_float3(7.580947,43.711884,11.118615 )},
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )}, //16
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )},  //18
	//{make_float3(21.208389,43.804527,-13.503925  ),make_float3(24.115210,37.705105,-6.132097 )},  //14

	//{make_float3(8.996864,46.504173,-18.891628  ),make_float3(14.546986,39.261303,-14.800542 )},//16
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(8.603185,30.805325,-0.314001 )},  //18
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000
	//{make_float3(-0.140227,33.588104,3.662050  ),make_float3(1.301753,30.177917,-5.627264 )},    //1000

	//{make_float3(-19.653139,33.588104,-3.880467  ),make_float3(-14.104563,29.767727,-11.270871 )},   //800
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},//22
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )}//25

};
static float k = 1800/25.0;
static float glossySequence[14]={0,2*k,4*k,5*k,7*k,10*k,13*k,15*k,19*k,21*k,23*k,25*k,25*k,25*k};
glossyScene::glossyScene()
{
	m_objectNum = geoNumber;	
	setMainLightPos(toiletLightPos);
	m_pObjectDis = glossyObj;
	 m_pMatDis = glossyMat;
	 m_posArray = glossyArray;
	 m_timeSequence = std::vector<int>(glossySequence,glossySequence+sizeof(glossySequence)/sizeof(glossySequence[0]));
}