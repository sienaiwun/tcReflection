#include "glossyScene.h"
#define  geoNumber  10
#define  CHINAREFLECT 0.185f
//#define ALLSPECULAR
static nv::vec3f toiletLightPos =  nv::vec3f(0.0f, 215, 30.0f);
static MyMeterial glossyMat[] = 
{
	//"./model/cheat.jpg",

	
	"./glossyModel/woodFloor.jpg",  //0
	"./glossyModel/wood.jpg",    //1
	"./glossyModel/redstone.jpg",  //2
	"./glossy/bw.jpg",	//3
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
	"./glossy/IEEE.BMP",     //17

	make_float3(0.74901,0.678,0.435) , 

};
#ifdef ALLSPECULAR


static GeoPara glossyObj[geoNumber] = 
{
	{"model/bathroom/toilet_body_all.3ds",16,0,0,3},
	{"model/bathroom/toilet_up_all.3ds",17,0,0,3},

	{"model/bathroom/toothBrush3Body.3ds",15,0,CHINAREFLECT,0},
	{"model/bathroom/toothBrush3White.3ds",15,0,CHINAREFLECT,0},
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

static GeoPara glossyObj[geoNumber] = 
{
	{"./glossy/wallsouth2.obj",      13,0,0,0},
    {"./glossy/lw.obj",      16,0,0,0},
	{"./glossy/lw4.obj",      16,0,0,0},
	{"./glossy/glossyFloorBW.obj",      3,0,1,0},
	{"./aomodel/aoWest.obj",      10,0,0,0},
	{"./glossyModel/ceil4225.3ds",      6,0,0,0},
	{"./aomodel/aoEast.obj",       11,0,0,0},
	{"./aomodel/aoNorthShift1089.3DS",      12,0,0,0},
	{"./glossyModel/floor2.obj",           0,0,0,0},
	{"./glossy/glossyTeapot.obj",           18,0,1,0},


};
#endif
static float k = 1800/25.0;

posPara toiletScenePosArray[] = 
{

	{make_float3(14.002789,39.998417,8.168809 ),make_float3(14.715953,39.353256,7.894608)},
	{make_float3(16.870752,38.273430,13.183920 ),make_float3(17.477489,37.687389,12.646851)},

};


static posPara golssyArray[] =
{
	//{make_float3(-201.957062,89.004913,-54.296104  ),make_float3(-193.146011,88.565048,-59.004818 )},
	//{make_float3(57.599560,73.710175,-69.969009  ),make_float3(63.420052,70.224892,-77.315712 )},  //sofa view
	/*{make_float3(62.123985,48.740673,-154.885864  ),make_float3(68.884621,47.642910,-162.172012 )},
	{make_float3(-24.060835,64.171219,-60.498837  ),make_float3(-24.277496,62.439995,-70.345428 )},
	{make_float3(-23.116392,45.627377,-64.996521  ),make_float3(-23.495859,46.107189,-74.977776 )},
	{make_float3(20.749678,66.900589,-63.084290  ),make_float3(26.805634,59.155106,-64.909927 )},
	{make_float3(40,70,-63.6346  ),make_float3(100,17.7966,-150.076 )},
	//{make_float3(43.398304,83.185516,-73.213638  ),make_float3(48.700409,79.272903,-80.735497 )},
	*/
	{make_float3(-82.339928,84.632927,136.407333 ),make_float3(-83.153648,84.143738,136.997650)},
	{make_float3(-146.490814,70.849846,123.232201 ),make_float3(-146.924545,70.421814,124.169495)},

	//{make_float3(-57.028,64.293,-119.08  ),make_float3(-56.63,63.98,-119.94 )},
	//{make_float3(-44.861229,52.771347,-152.258347  ),make_float3(-40.733448,49.739819,-160.847336 )},
	//{make_float3(3.717343,127.523064,-188.046753  ),make_float3(4.662045,125.831253,-197.857208 )},
	//{make_float3(28.454458,72.989174,-62.790611  ),make_float3(34.133820,68.893318,-69.929871 )},  //sofa
	//{make_float3(-116.189865,75.214836,-10.983877  ),make_float3(-110.822113,74.555305,-19.395283 )},
	//{make_float3(74.428383,80.568787,-81.764351  ),make_float3(113.743385,25.439957,-177.745819 )},



};
static float glossySequence[14]={0,2*k,4*k,5*k,7*k,10*k,13*k,15*k,19*k,21*k,23*k,25*k,25*k,25*k};
static float targetSequence [3] = {0,9,21};
glossyScene::glossyScene()
{
	 m_objectNum = geoNumber;	
	 setMainLightPos(toiletLightPos);
	 m_pObjectDis = glossyObj;
	 m_pMatDis = glossyMat;
	 m_posArray = golssyArray;
	 m_timeSequence = std::vector<int>(targetSequence,targetSequence+sizeof(targetSequence)/sizeof(targetSequence[0]));
	 m_hasGlossy = 0;
}