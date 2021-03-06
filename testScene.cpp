#include "testScene.h"
#define  geoNumber  (34+1)
#define  CHINAREFLECT 0.185f
//#define ALLSPECULAR
static nv::vec3f toiletLightPos =  nv::vec3f(0.0f, 65.0f, 30.0f);
static MyMeterial toiletMat[] = 
{
	//"./model/cheat.jpg",

	"/bathroom/masac.jpg",  //0
	"./bathroom/sceneSight.jpg",    //1
	"./model/redstone2.jpg",  //2
	"./model/book2.jpg",	//3
	"./model/book.jpg",     //4
	"./model/japen2.jpg",   //5
	make_float3(0.9,0.9,0.9),     //6
	make_float3(247.0/255,242.0/255,216.0/255),     //7dish
	make_float3(237.0/255,136.0/255,54.0/255),    //8soap
	make_float3(169.0/255,170.0/255,156.0/255),     //9littleBarrier
	"./aoBathroom/masic.jpg",     //10
	"./aoBathroom/walls2.jpg",   //11
	"./aoBathroom/floor.jpg",      //12
	"./aoBathroom/masic.jpg",         //13 west
	"./aoBathroom/littleWall.jpg",      //14  esat
	make_float3(0.9,0.9,0.9),      //15
	//"./model/cheat.jpg",               //15
	make_float3(108.0/255,65.0/255,42.0/255),     //16 tooth1
	make_float3(88.0/255,163.0/255,22.0/255),             //17 tooth2
	make_float3(1,1,1),             //18 white
	make_float3(192.0/255,129.0/255,74.0/255),        //19 toothPaste
	make_float3(198.0/255,219.0/255,207.0/255),          //20 sorport
	make_float3(1,219.0/255,207.0/255),           //21 mirrorFrame
	make_float3(1,225.0/255,250.0/255),             //22 paperRoll
	make_float3(0.7,0.9,0.6) ,              //23 paperSurport
	make_float3(0.74901,0.678,0.435) ,            //24 metal
	"./vase/test3.jpg",      //25
	make_float3(0.91,0.9,0.9),     //26
	make_float3(0.91,219.0/255,207.0/255)  //27

};
#ifdef ALLSPECULAR


static GeoPara toiletObj[geoNumber] = 
{
	{"model/bathroom/toilet_body_all.3ds",0,0,1,3},
	{"model/bathroom/toilet_up_all.3ds",15,0,1,3},

	//{"model/bathroom/testSphere.obj",15,0,CHINAREFLECT,0},
	//{"model/bathroom/testBox.obj",15,0,CHINAREFLECT,0},
	{"model/aoBathroom/littleBarrier.obj",9,0,1,0},
	{"model/bathroom/ceil.obj",15,0,1,0},
	{"model/bathroom/cockoff2.3ds",24,0,1,5},
	{"model/bathroom/cupOrigin.3ds",21,0,1,8},
	{"model/bathroom/floor.obj",15,0,1,0},
	{"model/aoBathroom/littleWall.obj",14,0,1,0},
	{"model/aoBathroom/walls.obj",11,0,1,0},
	{"model/aoBathroom/masic.obj",13,0,1,0},//10+1
	{"model/bathroom/metal12.obj",24,0,1,0},//11

	{"model/bathroom/mirror.obj",15,0,1.0f,6},//12
	{"model/bathroom/mirrorB.3ds",27,0,1,6},//13
	{"model/bathroom/mirrorFrame.obj",21,0,1,6},//14
	{"model/bathroom/paperRoll.obj",22,0,1,0},//15
	{"model/bathroom/paperSurpot.obj",23,0,1,0},//16
	{"model/bathroom/newwash.obj",15,0,1,4},//16
	{"model/bathroom/support.3ds",20,0,1,11},//18
	{"model/bathroom/brushOnBoard.obj",16,0,1,9},//19
	{"model/bathroom/brush2OnBoard.obj",17,0,1,10},//20
	{"model/bathroom/white1.3ds",18,0,1,9},
	{"model/bathroom/white22.3ds",18,0,1,10},

	{"model/bathroom/wash_basin0n.3ds",15,0,1,5},//20
	{"model/bathroom/washbasin1.3ds",15,0,1,5},//20
	{"model/bathroom/washbasin2_n.3ds",15,0,1,5},//20
	{"model/bathroom/washbasin3_n.3ds",26,0,1,5},//20
	{"model/bathroom/washbasin41.3ds",15,0,1,5},//20
	{"model/bathroom/washbasin42n.3ds",15,0,1,5},//20

	{"model/bathroom/scien.obj",1,0,1,0},
	{"model/bathroom/soapSurport2.obj",24,0,1,0},
	{"model/bathroom/dish.obj",7,0,1,0},
	{"model/bathroom/ceilMasac.3DS",6,0,1,0},
	{"model/bathroom/washbasinCircle.3DS",15,0,1,0},


};
#else

static GeoPara toiletObj[geoNumber] = 
{
	{"model/aoBathroom/sphereTest.obj",15,0,CHINAREFLECT,3},
	{"model/bathroom/toiletSmooth.obj",0,0,CHINAREFLECT,3},
	{"model/bathroom/toilet_up_all.obj",15,0,CHINAREFLECT,3},
	
	{"model/bathroom/testSphere.obj",15,0,CHINAREFLECT,0},
	//{"model/bathroom/testBox.obj",15,0,CHINAREFLECT,0},
	{"model/aoBathroom/littleBarrier.obj",9,0,0.0f,0},
	{"model/bathroom/ceil.obj",15,0,0.0f,0},
	{"model/bathroom/cockoff2.3ds",24,0,0.2,5},
	{"model/bathroom/cupOrigin.3ds",21,0,0.3f,8},
	{"model/bathroom/floor.obj",15,0,0.0f,0},
	{"model/aoBathroom/littleWall.obj",14,0,0.0f,0},
	{"model/aoBathroom/walls.obj",11,0,0.0f,0},
	{"model/aoBathroom/masic.obj",13,0,0.1,0},//10+1
	{"model/bathroom/metal12.obj",24,0,0.0f,0},//11

	{"model/bathroom/mirror.obj",15,0,1.0f,6},//12
	{"model/bathroom/mirrorB.3ds",27,0,0.0f,6},//13
	{"model/bathroom/mirrorFrame.obj",21,0,0.0f,6},//14
	{"model/bathroom/paperRoll.obj",22,0,0.0f,0},//15
	{"model/bathroom/paperSurpot.obj",23,0,0.0f,0},//16
	{"model/bathroom/newwash.obj",15,0,0.0f,4},//16
	{"model/bathroom/support.3ds",20,0,0.2f,11},//18
	{"model/bathroom/brushOnBoard.obj",16,0,0.0f,9},//19
	{"model/bathroom/brush2OnBoard.obj",17,0,0.0f,10},//20
	{"model/bathroom/white1.3ds",18,0,0.0f,9},
	{"model/bathroom/white22.3ds",18,0,0.0f,10},

	
	{"model/bathroom/wash_basin0n.3ds",15,0,CHINAREFLECT,5},//20
	{"model/bathroom/washbasin1.3ds",15,0,CHINAREFLECT,5},//20
	{"model/bathroom/washbasin2_n.3ds",15,0,CHINAREFLECT,5},//20
	{"model/bathroom/washbasin3_n.3ds",26,0,CHINAREFLECT,5},//20
	{"model/bathroom/washbasin41.3ds",15,0,CHINAREFLECT,5},//20
	{"model/bathroom/washbasin42n.3ds",15,0,CHINAREFLECT,5},//20
	
	//{"model/bathroom/washroom.3ds",15,0,CHINAREFLECT,5},//20

	{"model/bathroom/scien.obj",1,0,0.0f,0},
	{"model/bathroom/soapSurport2.obj",24,0,0.0f,0},
	{"model/bathroom/dish.obj",7,0,0.2f,0},
	{"model/bathroom/ceilMasac.3DS",6,0,0.2f,0},
	{"model/bathroom/washbasinCircle.3DS",15,0,0.0f,0},





};
#endif
static 

posPara toiletScenePosArray[] = 
{

	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(3.482440,31.107332,29.666050 )},//2
	{make_float3(18.045897,55.233578,45.419922  ),make_float3(19.309855,50.282154,36.824249 )},//4

	{make_float3(24.302208,48.071339,26.106796  ),make_float3(24.212349,47.543545,25.262188 )},//7//7
	//10
	{make_float3(13.948970,43.934372,17.002832  ),make_float3(18.528547,37.866623,10.505962 )},//14
	{make_float3(6.768429,44.201805,11.434514  ),make_float3(7.580947,43.711884,11.118615 )},
	{make_float3(-21.551481,41.017429,22.418484  ),make_float3(-15.171497,37.550900,15.542520 )}, //16
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},//22
	{make_float3(-31.926453,38.532524,-19.002571  ),make_float3(-23.099068,34.075119,-20.488741 )},//25
	


};
/*
posPara teasView[] = 
{

	// basin bottom view
	{make_float3(-10.034333,27.992634,-9.426936 ),make_float3(-8.987239,27.796152,-9.087838)},
	{make_float3(-13.686275,27.992634,4.087978 ),make_float3(-12.579448,27.834780,4.091589)},
	{make_float3(-13.686275,27.992634,4.087978 ),make_float3(-12.579448,27.834780,4.091589)},

};
*/
static posPara teasView[] =
{
	// basin side view need to be toper
	
	{make_float3(-2.403542,34.498703,37.004547  ),make_float3(3.482440,31.107332,29.666050 )},//2
	{make_float3(14.760003,34.922985,39.577850 ),make_float3(15.154776,34.594097,38.719994)},
	{make_float3(10.055952,32.957062,45.132004 ),make_float3(10.516335,32.651867,44.298397)},
	{make_float3(10.055952,32.957062,45.132004 ),make_float3(10.516335,32.651867,44.298397)},

};
static posPara toiletView[]
=
{
	{make_float3(-10.085075,17.626787,-0.612146 ),make_float3(-9.797616,17.471970,-1.557343)},
	{make_float3(-1.976262,17.626787,-0.796497 ),make_float3(-1.926718,17.467030,-1.782408)},
};
static float k = 75;
static float stay = 0.25*k;
static float toiletSequence[14]={0,2*k,4*k,6*k,8*k,10*k,12*k,15*k,9*k+2*stay,10*k+2*stay};

static float teaseSequence[3]={0,9,21};


// video path
/*
toiletScene::toiletScene()
{
	m_objectNum = geoNumber;	
	setMainLightPos(toiletLightPos);
	m_pObjectDis = toiletObj;
	m_pMatDis = toiletMat;
    m_posArray = toiletScenePosArray;
	m_timeSequence = std::vector<int>(toiletSequence,toiletSequence+sizeof(toiletSequence)/sizeof(toiletSequence[0]));
}*/



//basin view

testScene::testScene()
{
	m_objectNum = geoNumber;	
	setMainLightPos(toiletLightPos);
	m_pObjectDis = toiletObj;
	m_pMatDis = toiletMat;
    m_posArray = teasView;
	m_timeSequence = std::vector<int>(teaseSequence,teaseSequence+sizeof(teaseSequence)/sizeof(teaseSequence[0]));
}

//toilet view
/*
toiletScene::toiletScene()
{
	m_objectNum = geoNumber;	
	setMainLightPos(toiletLightPos);
	m_pObjectDis = toiletObj;
	m_pMatDis = toiletMat;
    m_posArray = toiletView;
	m_timeSequence = std::vector<int>(teaseSequence,teaseSequence+sizeof(teaseSequence)/sizeof(teaseSequence[0]));
}*/
