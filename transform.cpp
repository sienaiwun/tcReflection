#include "transform.h"

float BrushLeaptransform[16] = 
{
	0.850377,-0.3685,0.375588,0,
	0.392689,0.919578,0.0131285,0,
	-0.350221,0.136325,0.926694,0,
	-5.34601,10.0876,-8.75351,1,
};
float Brushfallform[16] = 
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,12,0,1,
};
float indetity[16] = 
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
};
float upAndDown[16] = 
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,1,0,1,
};
float gapLean[16]=
{
	1,1.17008e-008,-2.2412e-007,0,
	-2.73061e-008,0.997564,-0.0697563,0,
	2.22758e-007,0.0697563,0.997564,0,
	7.36271e-006,3.49904,1.15892,1,

};
float toiletLeft[16] = 
{
	0.999048,0.0436193,-1.02717e-008,0,
	-0.0436193,0.999048,-1.29155e-006,0,
	-4.60746e-008,1.29077e-006,1,0,
	0.000104616,-0.0053903,2.03277e-005,1,
};

float toiletRight[16] = 
{
	0.999048,-0.0436194,1.35787e-007,0,
	0.0436194,0.999048,-4.92304e-007,0,
	-1.14184e-007,4.97758e-007,1,0,
	0.000127014,0.00605096,7.88904e-006,1,

};
float washLeft[16]  = 
{
	0.998135,0.0610484,-6.02915e-007,0,
	-0.0610484,0.998135,1.07418e-008,0,
	6.02446e-007,2.60852e-008,1,0,
	0.0373635,-1.22263,1.19716e-005,1,
};
float washRight[16] = 
{0.998135,0.0610484,-6.02915e-007,0,
-0.0610484,0.998135,1.07418e-008,0,
6.02446e-007,2.60852e-008,1,0,
0.0373635,-1.22263,1.19716e-005,1,

};
float basinLeft[16] = {
	1,-6.11745e-008,7.21629e-009,0,
	6.1431e-008,0.999048,-0.0436195,0,
	-4.54102e-009,0.0436195,0.999048,0,
	-1.13489e-006,-0.256111,0.00807272,1,


};
float basinRight[16] = {
	1,-4.86251e-008,1.18725e-007,0,
	4.34001e-008,0.999048,0.0436194,0,
	-1.20733e-007,-0.0436194,0.999048,0,
	-2.00711e-007,0.256035,0.00309203,1,


};
float basinFront[16] = 
{
	0.999657,0.026177,-2.56812e-007,0,
	-0.026152,0.998706,0.0436196,0,
	0.00114208,-0.0436046,0.999048,0,
	0.00221105,-0.42281,0.00311095,1,


};
float cup1[16] = 
{
	0.539113,0.677759,-0.5,0,
	-0.782608,0.622515,-1.19209e-007,0,
	0.311257,0.391304,0.866025,0,
	39.5448,-12.073,32.7582,1,

};
float cup2[16] = 
{
	-0.377298,0.75375,-0.538058,0,
	-0.21792,0.492424,0.842633,0,
	0.900087,0.435177,-0.0215334,0,
	13.5415,-48.7542,-6.66207,1,

};
float cup3[16] = 
{
	-0.377298,0.75375,-0.538058,0,
	-0.21792,0.492424,0.842632,0,
	0.900087,0.435177,-0.0215336,0,
	-26.5364,-48.7542,3.00136,1,

};
float a = 0.6;
float b = 0.01;
float fallTime = 500;


float mirrorMove[16] = 
{
	0.745141,0.199368,-0.63641,0,
	-0.151601,0.979925,0.129479,0,
	0.649448,2.23517e-008,0.760406,0,
	15.5087,-5.07372,12.8675,1,



};
float mirror1[16] = {
	0.853735,0.187933,0.48561,0,
	-0.129479,0.979925,-0.1516,0,
	-0.504352,0.0665502,0.86093,0,
	-5.47182,-13.6897,-16.7691,1,


};
float mirror2[16] = 
{
	-0.0357311,-0.999048,0.0250187,0,
	0.818372,-0.0436193,-0.57303,0,
	0.573576,-4.17233e-007,0.819152,0,
	-70.5006,36.1519,31.2198,1,


};
float mirror3[16] = 
{
	-0.0357302,-0.999047,0.0250189,0,
	0.818372,-0.0436186,-0.57303,0,
	0.573576,-1.78814e-007,0.819152,0,
	-92.5871,36.1518,13.637,1,

};
float m1n[16] = 
{
	0.853736,0.187933,0.48561,0,
	-0.129479,0.979925,-0.1516,0,
	-0.504352,0.0665502,0.86093,0,
	4.35936,-13.6897,-12.1998,1,

};
float m2n[16] = 
{
	-0.0357313,-0.999049,0.0250185,0,
	0.818373,-0.0436195,-0.573031,0,
	0.573577,-4.47035e-007,0.819152,0,
	-57.3187,36.1519,29.9924,1,

};
float m3n[16] = 
{
	/*-0.0357309,-0.999048,0.0250189,0,
	0.818372,-0.0436193,-0.57303,0,
	0.573576,-2.68221e-007,0.819152,0,
	-73.8284,36.1519,19.9409,1,*/
	-0.0357311,-0.999048,0.0250188,0,
	0.818372,-0.0436194,-0.573031,0,
	0.573576,-2.68221e-007,0.819152,0,
	-63.511,36.1519,23.8079,1,


};
float air1[16] = 
{
	0.614421,0.438262,-0.656059,0,
	-0.580702,0.814116,-1.04308e-007,0,
	0.534108,0.380975,0.75471,0,
	30.1877,-24.1662,22.7164,1,

};
float air2[16] = 
{
	0.727881,0.0723352,-0.681878,0,
	-0.0957867,0.995396,0.00334504,0,
	0.678981,0.0628801,0.731458,0,
	0.827108,-16.5695,24.169,1,

};
float air3[16] = 
{
	0.515996,-0.528043,-0.674477,0,
	0.699237,0.714473,-0.0244178,0,
	0.494789,-0.45902,0.737892,0,
	-32.5835,22.7261,25.9462,1,

};
float air4[16] = 
{
	-0.0820741,-0.752384,-0.653593,0,
	0.99631,-0.0784593,-0.034792,0,
	-0.0251033,-0.654036,0.756046,0,
	-26.6391,75.4876,23.8138,1,
};

float air5[16] = 
{
	-0.380667,-0.621976,-0.68428,0,
	0.822119,-0.566406,0.0574885,0,
	-0.423337,-0.540675,0.72695,0,
	-6.16627,93.3926,22.0282,1,

};
float air6[16] = 
{
	-0.293476,-0.538297,-0.790005,0,
	0.676393,-0.700909,0.226318,0,
	-0.675548,-0.467935,0.5698,0,
	0.107398,94.0918,12.8949,1,

};
float b1[16] = 
{
	0.528983,0.538297,-0.656059,0,
	-0.71325,0.70091,1.3411e-007,0,
	0.459838,0.467934,0.75471,0,
	39.8585,-15.7941,22.4799,1,
};
float b2[16] = 
{
	0.573886,0.490144,-0.656059,0,
	-0.649447,0.760406,0,0,
	0.498871,0.426076,0.75471,0,
	35.0029,-11.2067,21.5277,1,

};
float b3[16] = 
{
	0.614422,0.438262,-0.656058,0,
	-0.580702,0.814116,-2.98023e-008,0,
	0.534108,0.380975,0.75471,0,
	30.1445,-17.4657,20.1772,1,
};
float b4[16] = 
{
	0.636516,0.405505,-0.656059,0,
	-0.537299,0.843392,-7.45058e-008,0,
	0.553315,0.3525,0.75471,0,
	28.1854,-27.802,18.2946,1,
};
float b5[16] = 
{
	0.786142,0.538297,-0.303672,0,
	-0.617693,0.700909,-0.356625,0,
	0.0208767,0.467934,0.883517,0,
	27.7784,-25.5332,26.9926,1,

};
float b6[16] = 
{
	0.644446,0.538297,-0.54307,0,
	-0.700146,0.700909,-0.136095,0,
	0.307384,0.467934,0.828585,0,
	34.1408,-25.5332,27.5679,1,

};
float b7[16] = 
{
	0.0399757,0.606679,-0.793941,0,
	-0.685401,0.594822,0.420015,0,
	0.727068,0.527377,0.439597,0,
	55.6949,-22.8104,15.1447,1,

};
float b8[16] = 
{
	-0.269813,0.573885,-0.773213,0,
	-0.425213,0.649449,0.630405,0,
	0.863942,0.498871,0.0687938,0,
	55.5763,-24.4747,5.51261,1,

};

float path1[16] = 
{
	0.754711,-3.13992e-009,-0.656057,0,
	7.80929e-008,1,8.50499e-008,0,
	0.656057,-1.15422e-007,0.754711,0,
	1.06177,0.15741,22.4259,1,
};
float path2[16] = 
{
	0.427472,0.621976,-0.656059,0,
	-0.824126,0.566406,2.98023e-008,0,
	0.371596,0.540675,0.75471,0,
	48.9912,-21.9752,22.4259,1,

};

float path3[16] = 
{0.536117,0.252386,-0.805531,0,
-0.215367,0.963573,0.158567,0,
0.816209,0.088474,0.570943,0,
12.7327,-20.3555,19.3348,1,



};
float path4[16] = 
{
	0.684606,-0.499015,-0.531318,0,
	-0.212753,0.560379,-0.800444,0,
	0.697173,0.661029,0.277472,0,
	11.662,15.4448,56.8803,1,


};
float path5[16] = 
{
	0.291016,-0.492561,-0.820179,0,
	-0.60849,0.56625,-0.555968,0,
	0.738275,0.660867,-0.134931,0,
	42.0032,14.9069,57.8979,1,
};
float path6[16] = 
{
	-0.232049,-0.492561,-0.838771,0,
	-0.817336,0.56625,-0.106406,0,
	0.527366,0.660867,-0.533986,0,
	69.2658,14.9069,40.3627,1,

};
float path7[16] = 
{
	-0.604156,-0.492561,-0.626402,0,
	-0.769061,0.56625,0.296488,0,
	0.208662,0.660866,-0.720913,0,
	81.076,14.9069,16.6342,1,

};
float path8[16] = 
{
	-0.913999,-0.386818,-0.122385,0,
	-0.307601,0.463989,0.83072,0,
	-0.264553,0.796923,-0.543071,0,
	74.3507,16.3236,-24.9927,1,


};
float path9[16] = 
{
	-0.853565,-0.111648,-0.508881,0,
	-0.520837,0.206128,0.828396,0,
	0.0124054,0.972134,-0.234094,0,
	82.0629,19.388,-14.3797,1,
};
float brush1[16] = 
{
	0.838663,6.51362e-006,0.544651,0,
	-8.97457e-006,1,1.85994e-006,0,
	-0.544651,-6.44787e-006,0.838663,0,
	-10.6245,-0.000191222,-15.9042,1,
};
float brush2[16] = 
{
	-0.309026,8.1386e-006,0.951053,0,
	2.13981e-006,1,-7.86217e-006,0,
	-0.951053,-3.94545e-007,-0.309026,0,
	19.7212,-11.9012,-23.593,1,

};
float brush3[16] = 
{
	0.838812,0.465873,0.281704,0,
	-0.366904,0.866021,-0.339688,0,
	-0.402213,0.181576,0.89736,0,
	-7.69277,-46.9779,15.8075,1,

};
float brush4[16] = 
{
	0.582449,0.811654,0.0443899,0,
	-0.639227,0.49108,-0.591801,0,
	-0.502137,0.316319,0.804861,0,
	1.94766,-52.5803,39.3871,1,


};
float brush21[16] = 
{
	1,2.38581e-006,3.65354e-005,0,
	-2.38543e-006,1,-1.05166e-005,0,
	-3.65354e-005,1.05165e-005,1,0,
	-14.2498,-0.000107878,-8.75495,1,

};
float brush22[16] = 
{
	0.945181,-0.225482,0.236201,0,
	0.29708,0.894034,-0.335331,0,
	-0.135561,0.387119,0.91201,0,
	-21.0552,-0.697151,-7.12949,1,


};
float brush23[16] = 
{
	

	0.843754,-0.536489,0.016063,0,
	0.47981,0.740525,-0.470536,0,
	0.240543,0.404724,0.882234,0,
	-27.4933,-5.94677,4.37942,1,

};
float brush24[16] = 
{
	0.437387,-0.872025,0.219695,0,
	0.872036,0.351615,-0.340472,0,
	0.219652,0.3405,0.914228,0,
	-34.6402,11.3924,-14.3389,1,

};
float t1l[16] = 
{
	0.999048,0.0436191,2.76745e-008,0,
	-0.0436191,0.999048,-6.94631e-007,0,
	-5.79473e-008,6.92763e-007,1,0,
	-0.0301605,0.0631549,1.07303e-005,1,
};
float t2l[16] = 
{
	0.999048,0.0436189,-2.8708e-007,0,
	-0.0435705,0.997939,-0.0471064,0,
	-0.00205444,0.0470616,0.99889,0,
	-0.109038,1.87006,0.758215,1,

};
float t3l[16] = 
{
	0.999048,0.043619,1.24215e-007,0,
	-0.043453,0.995247,-0.0871555,0,
	-0.00380176,0.0870725,0.996195,0,
	-0.177519,3.43826,1.3733,1,

};
float t1r[16] = 
{
	0.999848,-0.0174504,7.0563e-006,0,
	0.0174504,0.999848,0.00053246,0,
	-1.63469e-005,-0.000532256,1,0,
	-0.0673591,-0.00797557,-0.000111881,1,


};
float t2r[16] = 
{
	0.999848,-0.0174499,7.3984e-006,0,
	0.0174313,0.998763,-0.0465743,0,
	0.000805327,0.0465673,0.998915,0,
	-0.0358107,1.80005,0.758888,1,


};
float t3r[16] = 
{
	0.999848,-0.0174502,7.4298e-006,0,
	0.0173853,0.996089,-0.0866248,0,
	0.00150422,0.0866118,0.996241,0,
	-0.00843644,3.36917,1.37476,1,


};
float supportDown[16] = 
{
	0.999048,0.0436219,1.12013e-007,0,
	-0.0436219,0.999048,-2.51721e-009,0,
	-1.12017e-007,-2.37142e-009,1,0,
	2.19935,-1.56619,-3.55435e-006,1,

};
float w1[16] = 
{
	0.528983,0.538297,-0.656059,0,
	-0.71325,0.70091,1.49012e-007,0,
	0.459839,0.467934,0.754709,0,
	39.8585,-20.7941,22.4799,1,

};
float w2[16] = 
{
	0.573886,0.490145,-0.656059,0,
	-0.649448,0.760406,-1.19209e-007,0,
	0.498871,0.426076,0.75471,0,
	35.0029,-18.7067,21.5277,1,

};
float w3[16] = 
{
	0.614421,0.438262,-0.656059,0,
	-0.580703,0.814116,2.98023e-008,0,
	0.534108,0.380975,0.75471,0,
	30.2568,-22.4657,19.7691,1,

};

extern int currentTime;
extern float k;
float step = currentTime/fallTime;
void MutiMatrix9(float * Matrix,float x,float y,float z,float &outx,float &outy,float &outz)

{

	outx = x*Matrix[0]+y*Matrix[4]+z*Matrix[8];
	outy= x*Matrix[1]+y*Matrix[5]+z*Matrix[9];
     outz= x*Matrix[2]+y*Matrix[6]+z*Matrix[10];	
}
void MutiMatrix(float * Matrix,float x,float y,float z,float &outx,float &outy,float &outz)
{
	float tempx = x*Matrix[0]+y*Matrix[4]+z*Matrix[8]+Matrix[12];
	float tempy = x*Matrix[1]+y*Matrix[5]+z*Matrix[9]+Matrix[13];
	float tempz = x*Matrix[2]+y*Matrix[6]+z*Matrix[10]+Matrix[14];
	float tempt = x*Matrix[3]+y*Matrix[7]+z*Matrix[11]+Matrix[15];
	if(tempt<0.0001)
	{
		outx = FLT_MAX;
		outy = FLT_MAX;
		outz = FLT_MAX;

	}

	outx = tempx/tempt;
	outy = tempy/tempt;
	outz = tempz/tempt;

}
void transform1::draw(float * data,float *sourceData,int size,float time,float* beforMatrix,int isNormal)
{
	float g_lerp;
	if(time<startTime)
	{
		g_lerp = 0;
		if(dispear)
			g_lerp = -1000;
	}
	else if(time<endTime)
	{
		g_lerp = (time-startTime)*1.0f/(endTime-startTime);
	
	}
	else
	{
		g_lerp = 1;
	}
	if(inverse)
		g_lerp = 1.0-g_lerp;
	
	for(int i = 0 ;i<size;i++)
	{
		{
			float tx,ty,tz,bx,by,bz;
		float x  = sourceData[9*i];
		float y = sourceData[9*i+1];
		float z = sourceData[9*i+2];
		MutiMatrix(beforMatrix,x,y,z,bx,by,bz);
		MutiMatrix(transformMatrix,x,y,z,tx,ty,tz);
		data[9*i] = bx*(1-g_lerp)+tx*g_lerp;
		data[9*i+1] = by*(1-g_lerp)+ty*g_lerp;
		data[9*i+2] = bz*(1-g_lerp)+tz*g_lerp;
		}
		{
			float ntx,nty,ntz,nbx,nby,nbz;
		float nx  = sourceData[9*i+3];
		float ny = sourceData[9*i+4];
		float nz = sourceData[9*i+5];
		
		MutiMatrix9(beforMatrix,nx,ny,nz,nbx,nby,nbz);
		MutiMatrix9(transformMatrix,nx,ny,nz,ntx,nty,ntz);

		
		data[9*i+3] = nbx*(1-g_lerp)+ntx*g_lerp;
		data[9*i+4] = nby*(1-g_lerp)+nty*g_lerp;
		data[9*i+5] = nbz*(1-g_lerp)+ntz*g_lerp;
		}
	}
}
#define  TIME 4.5
transform1::transform1(float t[16],int time1,int time2,int _disappera,int _inverse):
startTime(time1*TIME),endTime(time2*TIME),transformMatrix(t),dispear(_disappera),inverse(_inverse)
{
		
}

//transform1 transformElement1=transform1(BrushLeaptransform,0,300);
//transform1 transformElement2=transform1(Brushfallform,300,400);
//std::vector<transform1> tArray(2);
transform1 transformArray[]=
{
	transform1(Brushfallform,8*k,10*k,1,1),transform1(BrushLeaptransform,10*k,12*k)
};
transform1 upDown[]=
{
	transform1(t2l,0,05),transform1(indetity,05,10),transform1(t2r,10,15),transform1(indetity,15,20),transform1(t2l,20,30),transform1(indetity,30,40),transform1(t2r,40,50),transform1(indetity,80,100),

	//transform1(upAndDown,0,14),transform1(indetity,15,28)
};
transform1 Lean[]=
{
	transform1(t3l,0,05),transform1(indetity,05,10),transform1(t3r,10,15),transform1(indetity,15,20),transform1(t3l,20,30),transform1(indetity,30,40),transform1(t3r,40,50),transform1(indetity,80,100),

	//transform1(gapLean,0,14),transform1(indetity,15,28)
};
transform1 cup[]=
{
	transform1(cup1,0,14),transform1(cup2,14,30),transform1(cup3,30,45),
};
transform1 wobble[] =
{
	transform1(t1l,0,05),transform1(indetity,05,10),transform1(t1r,10,15),transform1(indetity,15,20),transform1(t1l,20,30),transform1(indetity,30,40),transform1(t1r,40,50),transform1(indetity,50,100),
	//transform1(toiletLeft,0,14),transform1(indetity,14,28),transform1(toiletRight,28,43),transform1(indetity,43,58)
};
transform1 brashWobble[] =
{
	transform1(washRight,0,14),transform1(indetity,14,28),transform1(washLeft,28,43),transform1(indetity,43,58)
};
transform1 basinWobble[] = 
{
	transform1(basinLeft,0,03),transform1(indetity,03,6),transform1(basinRight,6,9),transform1(indetity,9,12),
	transform1(basinLeft,12,15),transform1(indetity,15,18),transform1(basinRight,18,22),transform1(indetity,22,28),
	transform1(basinLeft,28,35),transform1(indetity,35,46),transform1(indetity,46,110)
};
transform1 mirrorMoveArray[] = 
{
	//transform1(mirrorMove,0,580,0,ACTRUAL),
	//transform1(mirror1,0,14),transform1(mirror2,14,28),transform1(mirror3,28,43),transform1(mirror3,43,100)
	transform1(m1n,0,14),transform1(m2n,14,28),transform1(m3n,28,43),transform1(m3n,43,100),transform1(m1n,100,110),transform1(indetity,110,115)

};
transform1 brush2Array[] = 
{
	transform1(path1,0,14),transform1(path2,14,28),transform1(w1,28,31),
	transform1(brush21,31,34),	transform1(brush22,34,37),transform1(brush23,37,40),
	transform1(brush24,40,43),transform1(brush24,43,84),transform1(brush24,84,100),transform1(brush24,100,110)
	//transform1(brush24,0,1),transform1(brush24,1,100)
};
transform1 brush1Array[] = 
{
	transform1(path1,0,14),transform1(path2,14,28),transform1(w1,28,31),
	transform1(brush1,31,34),	transform1(brush2,34,37),transform1(brush3,37,40),
	transform1(brush4,40,43),transform1(brush4,43,84),transform1(brush4,84,100),transform1(brush4,100,110)
};
transform1 path[] = 
{
	transform1(path1,0,14),transform1(path2,14,28),
	//transform1(path3,28,35),transform1(path4,35,40),
	//transform1(air1,28,31),transform1(air2,31,34),	transform1(air3,34,37),transform1(air4,37,40),	transform1(air5,40,43),transform1(air6,43,46),
	transform1(w1,28,31),transform1(w2,31,34),	transform1(w3,34,37),transform1(b4,37,40),
	transform1(b5,40,50),transform1(b6,50,60),transform1(b7,60,74),transform1(b8,74,84),transform1(b8,84,100),transform1(indetity,100,110)
};
transform1 supportLean[] = 
{
	transform1(supportDown,0,3),transform1(indetity,3,6),transform1(supportDown,6,11),transform1(indetity,11,18),transform1(supportDown,18,24),transform1(indetity,24,35),transform1(indetity,35,100)
};
action::action(transform1* _tarray,int size,int _is_loop):tArray(_tarray),arraySize(size),index(0),is_loop(_is_loop)
{
	gapArray =(int*)malloc(size*sizeof(int)); //new int (size);
	for(int i = 0;i<size;i++)
	{
		gapArray[i] = _tarray[i].endTime;
	}
}
void action::draw(float * data,float *sourceData,int size,int isNormal)
{	
	int gap = gapArray[arraySize-1];
	if(is_loop)
		loopTime = currentTime%gap;
	else 
		loopTime = currentTime;
	if(arraySize==1)
	{
		tArray[0].draw(data,sourceData,size,loopTime,indetity,isNormal);
	}
	else
	{
		/*if(loopTime>gapArray[0])
			index = 1;
		else
			index = 0;*/
		for(int i = 0 ;i<arraySize;i++)
		{
			if(loopTime<gapArray[i])
			{
				index = i;
				break;
			}
			else
			{
				
			}
		}
		if(index==0)
			tArray[index].draw(data,sourceData,size,loopTime,indetity,isNormal);
		else 
			tArray[index].draw(data,sourceData,size,loopTime,tArray[index-1].getMatrix(),isNormal);

	}
}
action ActArray[] = 
{
	action(upDown,sizeof(upDown)/sizeof(transform1)),
	action(upDown,sizeof(upDown)/sizeof(transform1)),
	action(Lean,sizeof(Lean)/sizeof(transform1)),
	action(wobble,sizeof(wobble)/sizeof(transform1)),
	action(brashWobble,sizeof(brashWobble)/sizeof(transform1)),
	action(basinWobble,sizeof(basinWobble)/sizeof(transform1)),
	action(mirrorMoveArray,sizeof(mirrorMoveArray)/sizeof(transform1)),
	action(cup,sizeof(cup)/sizeof(transform1)),
	action(path,sizeof(path)/sizeof(transform1)),
	action(brush1Array,sizeof(brush1Array)/sizeof(transform1)),
	action(brush2Array,sizeof(brush2Array)/sizeof(transform1)),
	action(supportLean,sizeof(supportLean)/sizeof(transform1))
	
};
