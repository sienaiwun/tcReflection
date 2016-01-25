#include "livingRoom.h"
#define  geoNumber  49
#define  CHINAREFLECT 0.185f
static nv::vec3f livingLightPos =  nv::vec3f(0.0f, 200, 0);
static MyMeterial livingMat[] = 
{
	//"./model/cheat.jpg",

	"./aoModel/aofloor.jpg",  //0
	"./model/wood.jpg",    //1
	"./model/redstone.jpg",  //2
	"./model/book2.jpg",	//3
	"./model/book.jpg",     //4
	"./model/japen2.jpg",   //5
	"./wp/up.jpg",    //6
	"./model/photo.jpg",     //7
	"./model/picture.jpg",   //8
	"./model/sofa.jpg",      //9
	"./aoModel/aoWest.jpg",         //10 west
	"./aoModel/aoEast.jpg",      //11  esat
	"./aoModel/aonorth.jpg" ,  //12   north
	"./wp/south.jpg",       //13  south
	"./model/sphere.jpg",      //14
	make_float3(0.8,0.81,0.8),      //15
	make_float3(0.5,0.14,0.09),     //16
	make_float3(0,0,0),             //17 black
	make_float3(1,1,1),             //18 white
	make_float3(0.52,0.5,0.5),        //19 greay
	make_float3(1.1,1.0,1.0),          //20 pink
	make_float3(0.6,0.6,0.6),           //21 lightLine
	make_float3(1,1,1),             //22 white
	make_float3(0.67,1.0,0.51) ,              //23 bunny
	"./vase/test3.jpg",      //24
	make_float3(0.82,0.81,0.8),   //25
	make_float3(0.82,1.3,0.8), 

};

static GeoPara livingObj[geoNumber] = 
{
	{"./aoModel/aoEast.obj",11,0,0.0f},
	{"./aoModel/aoWest.obj",10,0,0.0f},
	{"./aoModel/aofloor.obj",0,0.1,0.15f},
	{"./aoModel/aoNorthShift.obj",12,0,0.0f},
	{"./t/south.obj",13,0,0.0f},
	{"./t/ceil.obj",6,0,0.0f},
	//{"./bunny/ballCenter.obj",18,0,1.0f},
	//{"./bunny/bunny2.obj",23,0,0.0f},
	/*{"./6teapot/1teapot.obj",19,0.5,0.5f},
	{"./6teapot/2teapot.obj",19,0.5,0.5f},
	{"./6teapot/3teapot.obj",19,0.5,0.5f},
	{"./6teapot/4teapot.obj",19,0.5,0.5f},
	{"./6teapot/5teapot.obj",19,0.5,0.5f},
	{"./6teapot/6teapot.obj",19,0.5,0.5f},*/
	{"./new/tableFrame2.obj",1,0,0.0f},
	{"./M/tableFrameShift.obj",1,0,0.1f},

	{"./new/closet.obj",1,0,0.1f},
	{"./new/bigSofa.obj",9,0,0.21f},

	//10
	{"./t/flatSofa.obj",9,0,0.2f},
	{"./t/littleSofa.obj",9,0,0.2f},

	{"./new/sofaFrame.obj",1,0,0.0f},
	{"./new/teapot.obj",19,0.5,0.51f},
	{"./new/mirror.obj",25,0,1.0f},
	{"./new/mirrorFrame.obj",16,0,0.0f},
	{"./new/cup2outshit.obj",5,0,0.0f},
	{"./new/cup2inShit.obj",18,1,0.0f},

	{"./new/cup2out3.obj",5,0,0.0f},
	{"./new/magazine1.obj",3,0,0.0f},
	{"./new/bottomBookOut.obj",2,0,0.0f},

	//{"./new/clock.obj",17,0,1.0f},
	{"./new/cup2in3.obj",18,1,0.0f},
	//20
	{"./new/clockPan.obj",22,0,0.2f},
	{"./new/blackObj.obj",17,0,0.0f},
	{"./new/clockBigPan.obj",21,0,0.3f},
	{"./new/floorLamp2.obj",19,0,0.0f},
	{"./new/closetHandle.obj",19,0,0.0f},
	{"./new/sofaBookOut.obj",4,0,0.0f},
	//{"./new/arcGolden.obj",1,0,0.1f},
	{"./new/tableMirrorFrame.obj",17,0,0.0f},
	{"./new/tableMirror.obj",8,0,0.0f},
	{"./new/picture.obj",7,0,0.0f},
	{"./new/vase.obj",15,0,0.2f},
	//30
	{"./new/vaseIn.obj",15,0,0.2f},
	{"./new/tree.obj",1,0,0.0f},
	{"./t/gong.obj",19,0,0.2f},
	{"./t/gongFrame.obj",1,0,0.0f},
	{"./t/gongString.obj",19,0,0.0f},
	{"./new/arcFrame.obj",19,0,0.0f},
	{"./new/arc3.obj",20,0,0.1f},
	{"./t/lightline.obj",19,0,0.0f},
	{"./new/gray.obj",19,0,0.0f},
	{"./new/black.obj",18,0,0.0f},

	//40
	{"./new/sphere.obj",14,0,0.0f},
	{"./t/bookInner.obj",26,0,0.0f},
	{"./t/ArcBack.obj",18,0,0.0f},
	{"./t/deskLamp.obj",1,0,0.0f},
	{"./vase/treeC2.obj",1,0,0.0f},
	{"./vase/vaseC2.obj",24,0,0.2f},
	{"./new/table_back.obj",17,0,0.0f},

};

static posPara livingPosArray[] =
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
	{make_float3(-57.028,64.293,-119.08  ),make_float3(-56.63,63.98,-119.94 )},
	//{make_float3(-44.861229,52.771347,-152.258347  ),make_float3(-40.733448,49.739819,-160.847336 )},
	//{make_float3(3.717343,127.523064,-188.046753  ),make_float3(4.662045,125.831253,-197.857208 )},
	//{make_float3(28.454458,72.989174,-62.790611  ),make_float3(34.133820,68.893318,-69.929871 )},  //sofa
	//{make_float3(-116.189865,75.214836,-10.983877  ),make_float3(-110.822113,74.555305,-19.395283 )},
	//{make_float3(74.428383,80.568787,-81.764351  ),make_float3(113.743385,25.439957,-177.745819 )},
	{make_float3(-200,100,100  ),make_float3(-0,100,-100)},            //100
	{make_float3(-200,100,100  ),make_float3(-22.8827,42.5,-180.212) }, //200
	{make_float3(-47.5,54,-150  ),make_float3(-22.8827,42.5,-180.212 )},//400
	{make_float3(-38,54,-150  ),make_float3(-22.8827,42.5,-180.212) },//600
	{make_float3(-38,54,-150  ),make_float3(65.128,15.7276,-63.6346 )},//800*/
	{make_float3(-38,70,-63.6346  ),make_float3(65.128,15.7276,-63.6346 )},//1000
	{make_float3(40,70,-63.6346  ),make_float3(65.128,15.7276,-63.6346 )},  //200
	{make_float3(40,70,-63.6346  ),make_float3(100,17.7966,-150.076 )},       //400
	{make_float3(85.779,100,-114.076  ),make_float3(100,17.7966,-150.076 )},   //600
	{make_float3(85.779,100,-114.076  ),make_float3(136.25,135.42,-160.3 )},   //700

	{make_float3(126.39,142.05,-160.3  ),make_float3(136.25,135.42,-160.3 )},   //800
	{make_float3(126.39,142.05,-65  ),make_float3(136.25,135.42,-65 )},  //1000

};
static float k = 1800/25.0;
static float livingSequence[14]={0,2*k,4*k,5*k,7*k,10*k,13*k,15*k,19*k,21*k,23*k,25*k,25*k,25*k};
livingRoom::livingRoom()
{
	m_objectNum = geoNumber;	
	setMainLightPos(livingLightPos);
	m_pObjectDis = livingObj;
	m_pMatDis = livingMat;
	m_posArray = livingPosArray;
	m_timeSequence = std::vector<int>(livingSequence,livingSequence+sizeof(livingSequence)/sizeof(livingSequence[0]));
}