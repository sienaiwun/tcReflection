//#include<stdio.h>
#include"cuda.h"
#include "macro.h"
//#include<cudpp.h>
#include <helper_math.h>
#include <thrust/reduce.h>
#include <thrust/device_vector.h>
#define UseNine 0
//#include <cuda_gl_interop.h>
//#include <cutil_gl_inline.h>
//#include <helper_cuda_gl.h>
#define UseTrial   1
__constant__  float Cuda_MvpMat[16];
__constant__  float Cuda_CameraViewMat[16]; 
__constant__ float3 d_refCameraPos;
__constant__ float3 d_newCameraPos;
__constant__ float3 d_cameraVec;
__constant__ int Cuda_FramMent;
__constant__ int d_width,d_height;


//__host__ __device__  uint rastWidth = 1024,rastHeight=1024;
#define rastWidth rasterWidth
#define rastHeight rasterHeight
unsigned int  *g_PixelState;//in device address
unsigned int  *g_PixelSums;//in device address
unsigned int  *g_PixelPos; //in device address

cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc(32, 32, 32, 32, cudaChannelFormatKindFloat);

texture<float4,2,cudaReadModeElementType> cuda_WorldPos_Tex;
texture<float4,2,cudaReadModeElementType> cuda_WorlNormal_Tex;
texture<float4,2,cudaReadModeElementType> cuda_Reflect_Tex;
texture<float4,2,cudaReadModeElementType> cuda_RePro_Pixel;

float4 *cuda_PBO_Buffer;
float2 *cuda_LastVec_Buffer;
float4 *cuda_test_Buffer;
float4 *cuda_diffNormBuffer;
__device__ float4* d_cudaPboBuffer;
__device__ float4* d_cudaTestBuffer;
__device__ float2* d_cudaLastBuffer;
__device__ float4* d_cudaDiffNorlBuffer;
//float4 *cuda_RePos_Buffer;
/*CUDPPHandle theCudpp;
CUDPPHandle scanplan;
*/
thrust::device_vector<uint> thrustSumVec;
thrust::device_vector<uint> thrustStateVec;
thrust::device_vector<uint> thrustPosVec;

__device__ int* d_sum;
__device__ int* d_state;
__device__ int* d_pos;

__device__ float2 toNDC(float4 temp)
{
	temp.x = temp.x / temp.w * 0.5 + 0.5;
	temp.y = temp.y / temp.w * 0.5 + 0.5;
	return make_float2(temp.x,temp.y)* make_float2(rastWidth,rastHeight);
}


__device__ float4 cuda_Mat_Mut3(const float Mat4[16],float3 vec)
{
	float result_F[4];

	for(int i =0;i<4;i++)
		result_F[i] = (Mat4[i] * vec.x + Mat4[i+4] * vec.y +
		Mat4[i+8] * vec.z + Mat4[i+12] );
	float4 result_V = make_float4(result_F[0],result_F[1],result_F[2],result_F[3]);
	return result_V;
}

__device__ float4 Cude_Mul_Mat4(const float Mat4[16],float TmpVec4[4]){

	float result_F[4];
	for(int i =0;i<4;i++)
		result_F[i] = (Mat4[i] * TmpVec4[0] + Mat4[i+4] * TmpVec4[1] +
		Mat4[i+8] * TmpVec4[2] + Mat4[i+12] * TmpVec4[3]);
	float4 result_V = make_float4(result_F[0],result_F[1],result_F[2],result_F[3]);

	return result_V;


}
__device__ float3 transFormToNdc(float3 temp)
{
	float4 ProPos = cuda_Mat_Mut3(Cuda_MvpMat,temp);
	float2 ndcTc=  toNDC(ProPos);
	return make_float3(ndcTc.x,ndcTc.y,ProPos.z/ProPos.w);
}
__device__ float Cuda_Compute_Cos(float2 TmpUV,float3 ReflectPos,int orignId)
{
	float3 WorldPos,WorldNormal;
	float4 Tmpfloat4;
	Tmpfloat4 = tex2D(cuda_WorlNormal_Tex,TmpUV.x,TmpUV.y);
	int NowId = (int)Tmpfloat4.w;
	if(NowId != orignId)
		return -100;
	WorldNormal = make_float3(Tmpfloat4.x,Tmpfloat4.y,Tmpfloat4.z);

	Tmpfloat4 = tex2D(cuda_WorldPos_Tex,TmpUV.x,TmpUV.y);
	WorldPos = make_float3(Tmpfloat4.x,Tmpfloat4.y,Tmpfloat4.z);

	float3 TmpReMirrorPos,TmpCam_Mirror_Normal,TmpCamera_Vec_Normal,TmpInsertPoint;

	TmpReMirrorPos =  abs(dot(WorldPos - ReflectPos,WorldNormal)) * (-2) * WorldNormal + ReflectPos;
	TmpCam_Mirror_Normal = normalize(cross(d_cameraVec,normalize(d_refCameraPos - TmpReMirrorPos)));
	TmpCamera_Vec_Normal = normalize(cross(TmpCam_Mirror_Normal,d_cameraVec));
	TmpInsertPoint = abs(dot(TmpCamera_Vec_Normal,d_refCameraPos - TmpReMirrorPos)/dot(TmpCamera_Vec_Normal,normalize(WorldPos-TmpReMirrorPos))) * normalize(WorldPos-TmpReMirrorPos) + TmpReMirrorPos;
	//TmpDis1 = dot(TmpInsertPoint - d_refCameraPos,d_cameraVec);
	//TmpDis2 = dot(TmpInsertPoint - d_refCameraPos,TmpInsertPoint - d_refCameraPos);
	//return  TmpDis2 - TmpDis1*TmpDis1 ;
	//return  dot(d_cameraVec,TmpInsertPoint - d_refCameraPos);
	return  dot(d_cameraVec,normalize(TmpInsertPoint - d_refCameraPos));
}
enum planeType
{
	originType,
	validSearchType,
	invalidType,
};
#define PROJECTTHRES 0.02
class Plane
{
public:
	planeType m_type;
	float3 m_worldPos;
	float3 m_worldNormal;
	float m_dis;
	float3 m_reflectPos;
	float3 m_mirrorPos;
	float2 m_tc;
	int m_orginID;
	__device__ Plane()
	{
	}
	__device__ float2 getTc()
	{
		return m_tc;
	}
	__device__ int getID()
	{
		return m_orginID;
	}
	__device__ int isInValid()
	{
		return (m_type== invalidType);
	}
#define NORMALDIFF 0.001
#define DISDIFF 0.002
	__device__ bool isEqual(Plane& anotherPlane)
	{
		//printf("normal diff:%f, dis diff:%f\n",length(m_worldNormal-anotherPlane.m_worldNormal),dot(m_worldPos-anotherPlane.m_worldPos,m_worldNormal));
		if(length(m_worldNormal-anotherPlane.m_worldNormal)>NORMALDIFF)
		{
			//printf("normal diff,dis:%f,%d\n",length(m_worldNormal-anotherPlane.m_worldNormal),length(m_worldNormal-anotherPlane.m_worldNormal)>NORMALDIFF);
			///printf("(%f,%f,%f)\n",m_worldNormal.x,m_worldNormal.y,m_worldNormal.z);					
			//printf("(%f,%f,%f)\n",anotherPlane.m_worldNormal.x,anotherPlane.m_worldNormal.y,anotherPlane.m_worldNormal.z);
			return false;
		}
		if(dot(m_worldPos-anotherPlane.m_worldPos,m_worldNormal)>DISDIFF)
		{
			//printf("distance too mush:%f\n",dot(m_worldPos-anotherPlane.m_worldPos,m_worldNormal));
			return false;
		}
		return true;
	}
	// 查询是否是投影点 返回1 可以
	__device__ int isAbleFastProj(float2 reProjectTc)
	{
		 
		/*float4 WorldTmp = tex2D(cuda_WorlNormal_Tex,reProjectTc.x,reProjectTc.y);
		int reprjectId  = (int)WorldTmp.w;
		float3 m_worldNormal = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
		WorldTmp = tex2D(cuda_WorldPos_Tex,reProjectTc.x,reProjectTc.y);
		float3 worldPos = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
		*/
		
		Plane pixelPlane(reProjectTc);
		if(pixelPlane.isInValid())
		{
			//printf("is notVaid");
			return 0;
		}
		if(!isEqual(pixelPlane))
		{
			//printf("is NotEqual");
			return 0;
		}
		return 1;  
		// no need to do futher check; nearest sampling
		pixelPlane.setReflectedPos(*this);
		float DisPoint2Line;
		DisPoint2Line = pixelPlane.getDisToPath();
		if(PROJECTTHRES>DisPoint2Line)
		{
			//printf("within threshold");
			return 1;
		}
		//printf("finally");
		return 0;
	
	}
	__device__ Plane(float2 tc)
	{
		m_tc = tc;
		float4 WorldTmp;
		WorldTmp = tex2D(cuda_WorldPos_Tex,tc.x,tc.y);
		m_worldPos = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
		WorldTmp = tex2D(cuda_WorlNormal_Tex,tc.x,tc.y);
		m_orginID = (int)WorldTmp.w;
		m_worldNormal = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
		m_dis = tex2D(cuda_Reflect_Tex,tc.x,tc.y).w;
	
	}
	
	
	__device__ void setPreviousPos( float d)
	{
		m_dis = d;
	
	}
	__device__ float3 getReflectedPos()
	{
		return m_reflectPos;
	}
	__device__ float getMirrorDepth()
	{
		float4 ProPos = cuda_Mat_Mut3(Cuda_MvpMat,m_mirrorPos);
		return ProPos.z/ProPos.w;
	}
	__device__ void setReflectedPos(Plane p)
	{
		m_reflectPos = p.getReflectedPos();
		if(m_orginID == p.getID())
		{
			m_type = validSearchType;
		}		
		else 
		{
			m_type = invalidType;
		}
	}
	__device__ void setReflectedPos()
	{
		float3 inComeDirection = m_worldPos - d_refCameraPos;
		float3 LookVec = normalize(inComeDirection);
		//计算反射光线方向
		float3 ReflectVec = normalize(reflect(LookVec,m_worldNormal));
		m_type = originType;
		m_reflectPos = m_worldPos + ReflectVec * m_dis;	
	}
	__device__ float3 getMirrorPos()
	{
		float3 reflectPos = m_reflectPos;
		m_mirrorPos = abs(dot(m_worldPos - reflectPos,m_worldNormal)) * (-2) * m_worldNormal + reflectPos;
		return m_mirrorPos;
	}
	__device__  float3 beneathPos(float3 sourcePos, float dis)
	{
		float3 inComeDirection = m_worldPos - sourcePos;
		float3 LookVec = normalize(inComeDirection);
		float3 ReflectVec = normalize(reflect(LookVec,m_worldNormal));

		float CosReCorner = dot(ReflectVec,m_worldNormal);
		float3 reflectedPos = m_worldPos + ReflectVec * dis;
		//镜像点的坐标
		float3 ReMirrorPos = dis * CosReCorner * 2 * (-1) * (m_worldNormal) + reflectedPos;

		//相机到反射面的距离
		return ReMirrorPos;
	}
	//得出以该点为平面的反射物体的镜像点
	__device__ float3 intersectPos()
	{
		getMirrorPos();
		float3 VecEye2Ref = normalize(m_mirrorPos - d_newCameraPos);
		return dot( m_worldPos -d_newCameraPos ,m_worldNormal)/ dot(VecEye2Ref,m_worldNormal) * VecEye2Ref + d_newCameraPos;
	}
	
	__device__ float3 intersetVirtualPos()
	{
		float3 ReMirrorPos =  abs(dot(m_worldPos - m_reflectPos,m_worldNormal)) * (-2) * m_worldNormal + m_reflectPos;


		//outPut[index] = make_float4(ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z,1.0);
		//return;

		//求出该点与相机连线和平面的新交点
		float DisEye2Plane = abs(dot(d_newCameraPos - m_worldPos,m_worldNormal));
		float CoseTheta2 = abs(dot(normalize(d_newCameraPos - ReMirrorPos),m_worldNormal));
		float3 VecEye2Ref = normalize(ReMirrorPos - d_newCameraPos);
		float3 FinalPos2;
		//	FinalPos2 = d_newCameraPos + VecEye2Ref * (DisEye2Plane/CoseTheta2 );
		FinalPos2 = dot(m_worldPos -d_newCameraPos ,m_worldNormal)/ dot(VecEye2Ref,m_worldNormal) * VecEye2Ref + d_newCameraPos;
		return FinalPos2;
	}
	
	__device__ int checkConverge()
	{
#define CONVERGETHRES 5
		float3 FinalPos  = intersectPos();
		float3 ndcPos =  transFormToNdc(FinalPos);
		float2 ProPosUv =make_float2(ndcPos.x,ndcPos.y);
		//printf("!tc:(%f,%f)\n",m_tc.x,m_tc.y);
		//printf("!proPosUv(%f,%f)\n",ProPosUv.x,ProPosUv.y);
		return length(m_tc-ProPosUv)<CONVERGETHRES;
	}
	__device__ float getSpherDistance()
	{
		float3 inComeDirection = m_worldPos - m_reflectPos;
		float3 LookVec = normalize(inComeDirection);
		//计算反射光线方向
		float3 reflectVec = normalize(reflect(LookVec,m_worldNormal));
		float3 cuttingPoint = m_worldPos+reflectVec*(dot(d_newCameraPos-m_worldPos,reflectVec));
		return length(cuttingPoint-d_newCameraPos);
	}
	__device__ float getDisToPath()
	{
		if(isInValid())
		{
			return 3000.0;
		}
		//求出新的镜像点
		return getSpherDistance();
		float3 ReMirrorPos =  getMirrorPos();
		//printf("!ReMirrorPos: (%f,%f,%f)\n",ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z);
		//求相机与镜像点组成的平面的法线
		 
		float3 Cam_Mirror_Normal = normalize(cross(d_cameraVec,normalize(d_refCameraPos - ReMirrorPos)));
		//printf("!Cam_Mirror_Normal: (%f,%f,%f)\n",Cam_Mirror_Normal.x,Cam_Mirror_Normal.y,Cam_Mirror_Normal.z);

		//求相机轨迹所在法线
		float3 Camera_Vec_Normal = normalize(cross(Cam_Mirror_Normal,d_cameraVec));
		//printf("!Camera_Vec_Normal: (%f,%f,%f)\n",Camera_Vec_Normal.x,Camera_Vec_Normal.y,Camera_Vec_Normal.z);

		//分别求三个点与该平面的交点
		float3 InsertPoint = abs(dot(Camera_Vec_Normal,d_refCameraPos - ReMirrorPos)/dot(Camera_Vec_Normal,normalize(m_worldPos-ReMirrorPos))) * normalize(m_worldPos-ReMirrorPos) + ReMirrorPos;
		//printf("!InsertPoint: (%f,%f,%f)\n",InsertPoint.x,InsertPoint.y,InsertPoint.z);

		//求点与直线的距离

		float3 toNewPlace2 = d_newCameraPos-InsertPoint;
		float DisPoint2Line = length(toNewPlace2);
		//DisPoint2Line1  =  dot(normalize(InsertPoint1 - d_refCameraPos),d_cameraVec);

		//printf("!return value: (%f)\n",DisPoint2Line1);

		return DisPoint2Line;


	}
};
/*
__global__ void lineSearchKernel(int width,int height)
{


	//return;



	//return;
	int x = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	int y = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;

	if( x >width || y> height)
		//if(x !=696 || y !=640)
			//if(x !=373 || y !=139)
				return;
	int index = y * width + x;

	//提取数据
	float3 WorldPos,WorldNormal;
	float ReflectDis;
	float4 WorldTmp;

	WorldTmp = tex2D(cuda_WorldPos_Tex,x+0.5,y+0.5);

	WorldPos = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
	float4 WorldNormalTmp = tex2D(cuda_WorlNormal_Tex,x+0.5,y+0.5);

	int Origin_Id = (int)WorldNormalTmp.w;
	WorldNormal = normalize(make_float3(WorldNormalTmp.x,WorldNormalTmp.y,WorldNormalTmp.z));

	ReflectDis = tex2D(cuda_Reflect_Tex,x,y).w;

	Plane fittingPlane(&WorldPos,&WorldNormal);
	fittingPlane.setPreviousPos(&d_refCameraPos,ReflectDis,&d_newCameraPos);

	float3 ReflectPos =fittingPlane.reflectCamera();
	//printf("origin pos:(%f,%f,%f)\n",ReflectPos.x,ReflectPos.y,ReflectPos.z);
	//printf("0Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);

	float RefelctInCameraDepth;
	RefelctInCameraDepth = ReflectDis /10000.0;

	//RefelctInCameraDepth  = TMpReflecPosInCamera.z / TMpReflecPosInCamera.w;
	//镜像点的坐标
	float3 ReMirrorPos =fittingPlane.mirrorPos();

	//printf("1Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);
	//相机到反射面的距离
	float DisEye2Plane = abs(dot(d_newCameraPos - WorldPos,WorldNormal));
	//镜像点与相机的向量
	float3 VecEye2Ref = normalize(ReMirrorPos - d_newCameraPos);

	//camera2_pos = new camera position
	//camera1_pos = last camera position
	//镜像点与相机连线和反射平面的交点
	float3 FinalPos ;//= d_newCameraPos + DisEye2Plane/abs(dot(VecEye2Ref,WorldNormal))* VecEye2Ref;

	FinalPos = fittingPlane.intersectPos();

	float4 ProPos = cuda_Mat_Mut3(Cuda_MvpMat,FinalPos);

	//Pro 位置
	float2 ProPosUv;

	float2 TmpUv;

	ProPosUv = toNDC(ProPos)* make_float2(width,height);


	float2 MoveVec = ProPosUv - make_float2(x*1.0+0.5,y*1.0+0.5);
	TmpUv = make_float2(x*1.0,y*1.0);
	int IterTime = 0;

	float LastCosLength = 0;

	float ChengShu = 1.0f;

	float2 TmpMov = make_float2(0,0);

	float2 TheComputVU = make_float2(-1.0f,-1.0f);
	if(MoveVec.x >=0)
		TheComputVU.x =1;
	if(MoveVec.y >=0)
		TheComputVU.y =1;

	float Tmpfloat = Cuda_Compute_Cos(TheComputVU + TmpUv,ReflectPos,Origin_Id);
	if(Tmpfloat>-50 && Tmpfloat < 0)
		ChengShu = -1;

	float LastCoslength;
	//printf("2Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);

	while(IterTime<5 && dot(MoveVec,MoveVec) >=2.5)
	{
		//printf("3Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);

		float2 TheComputVU = make_float2(-1.0f,-1.0f);
		if(MoveVec.x >=0)
			TheComputVU.x =1;
		if(MoveVec.y >=0)
			TheComputVU.y =1;
		//TheComputVU = TheComputVU + TmpUv;


		float2 TmpUv1,TmpUv2,TmpUv3;
		// 		if(ChengShu >0){
		// 		float Tmpfloat = Cuda_Compute_Cos(TheComputVU + TmpUv,ReflectPos,Origin_Id);
		// 		if(Tmpfloat < -50 && Tmpfloat<0 )
		// 			ChengShu = -1;
		// 		else
		// 			ChengShu = 1;
		// 		}

		//int2 TmpUv1,TmpUv2,TmpUv3; 

		TmpUv1 = TmpUv;
		TmpUv2 = TmpUv;
		TmpUv3 = TmpUv;

		//MoveVec = ChengShu * MoveVec;

#if UseNine
		float2 TmpUvs[8];
		int NowSum = 0;
		for(int xi = -1;xi<=1;xi++)
			for(int yi = -1;yi<=1;yi++)
			{
				if((xi != 0 || yi !=0) )//&& length(TmpMov + make_float2(xi,yi)) > 0.5)
					TmpUvs[NowSum++] = TmpUv + make_float2(xi*1.0f,yi*1.0f);
				//TmpUvs[tmpIndex++] =make_float2((x+50)*1.0f,(y+50)*1.0f);
			}
			//outPut[index] = make_float4((TmpUvs[0].x+50)/1024.0,(TmpUvs[7].y+50)/1024.0,Origin_Id,RefelctInCameraDepth);
			//return;

			//取出每个采样点对应的世界坐标
			float3 WorldPosEs[8],WorldNormals[8];
			float4 TmpFloat;
			int ModelIds[8];
			for(int i =0;i<8;i++)
			{
				TmpFloat = tex2D(cuda_WorldPos_Tex,TmpUvs[i].x,TmpUvs[i].y);
				WorldPosEs[i] = make_float3(TmpFloat.x,TmpFloat.y,TmpFloat.z);
				TmpFloat = tex2D(cuda_WorlNormal_Tex,TmpUvs[i].x,TmpUvs[i].y);
				WorldNormals[i] = make_float3(TmpFloat.x,TmpFloat.y,TmpFloat.z);
				ModelIds[i] = (int) TmpFloat.w;

			}
			//outPut[index] = make_float4(TmpUv.x/1024.0,TmpUv.y/1024.0,Origin_Id,RefelctInCameraDepth);


#else
		if(MoveVec.x > 0){
			TmpUv1.x += 1.0;
			TmpUv3.x += 1.0;
		}
		else{
			TmpUv1.x -= 1.0;
			TmpUv3.x -= 1.0;
		}

		if(MoveVec.y >0){
			TmpUv2.y += 1.0;
			TmpUv3.y += 1.0;

		}
		else{
			TmpUv2.y -= 1.0;
			TmpUv3.y -= 1.0;
		}
		//取出每个采样点对应的世界坐标
		float4 TmpFloat4;
		float3 WorldPos1,WorldPos2,WorldPos3;
		float3 WorldNormal1,WorldNormal2,WorldNormal3;
		int Now_ModelId1,Now_ModelId2,Now_ModelId3;

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv1.x,TmpUv1.y);
		WorldPos1 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv2.x,TmpUv2.y);
		WorldPos2 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv3.x,TmpUv3.y);
		WorldPos3 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv1.x,TmpUv1.y);
		WorldNormal1 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId1 = (int)TmpFloat4.w;

		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv3.x,TmpUv3.y);
		WorldNormal3 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId3 = (int)TmpFloat4.w;

		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv2.x,TmpUv2.y);
		WorldNormal2 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId2 = (int)TmpFloat4.w;

#endif



		//求出新的镜像点
		float DisPoint2Line1,DisPoint2Line2,DisPoint2Line3;
		DisPoint2Line1 = fittingPlane.getDisToPath(WorldPos1,WorldNormal1);
		DisPoint2Line2 = fittingPlane.getDisToPath(WorldPos2,WorldNormal2);
		DisPoint2Line3 = fittingPlane.getDisToPath(WorldPos3,WorldNormal3);

		//
		//printf("DisPoint2Line1:%f\n",DisPoint2Line1);
		float TmpDis;
		if(DisPoint2Line2<DisPoint2Line1)
		{
			TmpUv = TmpUv1;
			TmpUv1 = TmpUv2;
			TmpUv2 = TmpUv;
			WorldPos = WorldPos1;
			WorldPos1 = WorldPos2;
			WorldPos2 = WorldPos;

			TmpDis = DisPoint2Line1;
			DisPoint2Line1 = DisPoint2Line2;
			DisPoint2Line2 = TmpDis;

			TmpDis = Now_ModelId1;
			Now_ModelId1 = Now_ModelId2;
			Now_ModelId2 = TmpDis;

		}

		if(DisPoint2Line3< DisPoint2Line1){
			TmpUv = TmpUv1;
			TmpUv1 = TmpUv3;
			TmpUv3 = TmpUv;
			WorldPos = WorldPos1;
			WorldPos1 = WorldPos3;
			WorldPos3 = WorldPos;

			TmpDis = DisPoint2Line1;
			DisPoint2Line1 = DisPoint2Line2;
			DisPoint2Line2 = TmpDis;

			TmpDis = Now_ModelId1;
			Now_ModelId1 = Now_ModelId2;
			Now_ModelId2 = TmpDis;
		}

		if(DisPoint2Line3< DisPoint2Line2){
			TmpUv = TmpUv2;
			TmpUv2 = TmpUv3;
			TmpUv3 = TmpUv;
			WorldPos = WorldPos2;
			WorldPos2 = WorldPos3;
			WorldPos3 = WorldPos;

			TmpDis = DisPoint2Line1;
			DisPoint2Line1 = DisPoint2Line2;
			DisPoint2Line2 = TmpDis;

			TmpDis = Now_ModelId1;
			Now_ModelId1 = Now_ModelId2;
			Now_ModelId2 = TmpDis;
		}


		//TmpUv = TmpUv1;

		if(Now_ModelId1 == Origin_Id){
			TmpUv = TmpUv1;
			WorldPos = WorldPos1;
		}
		// 		else{
		// 		outPut[index] = make_float4(-10.0f,-10.0f,Origin_Id,RefelctInCameraDepth);
		// 			return;
		// 		}

		else if(Now_ModelId2 == Origin_Id){
			TmpUv = TmpUv2;
			WorldPos = WorldPos2;
		}
		else if(Now_ModelId3 == Origin_Id){
			TmpUv = TmpUv3;
			WorldPos = WorldPos3;
		}
		else{
			d_cudaPboBuffer[index] = make_float4(-10.0f,-10.0f,Origin_Id,RefelctInCameraDepth);
			return;
		}
		TmpUv = TmpUv+TheComputVU;

		//LastCoslength = Cuda_Compute_Cos(TmpUv,ReflectPos,Origin_Id);
		//LastCoslength = 0;
		//if(LastCoslength > length(Cuda_CameraPos1[1] - Cuda_CameraPos2[1])-0.5)
		//break;



		//得出该点法线

		float4 TmpFloat41= tex2D(cuda_WorlNormal_Tex,TmpUv.x,TmpUv.y);
		WorldNormal  = make_float3(TmpFloat41.x,TmpFloat41.y,TmpFloat41.z);


		float NowModelId = TmpFloat41.w;
		fittingPlane = Plane(&WorldPos,&WorldNormal);



		float3 FinalPos2 =fittingPlane.intersetPos2();
		//printf("!WorldNormal: (%f,%f,%f)\n",WorldNormal.x,WorldNormal.y,WorldNormal.z);
		//printf("!WorldPos: (%f,%f,%f)\n",WorldPos.x,WorldPos.y,WorldPos.z);
		//printf("!FinalPos2: (%f,%f,%f)\n",FinalPos2.x,FinalPos2.y,FinalPos2.z);

		float4 ProPos2;
		ProPos2 = cuda_Mat_Mut3(Cuda_MvpMat,FinalPos2);
		ProPosUv = toNDC(ProPos2)* make_float2(width,height);


		MoveVec = ProPosUv - TmpUv;
		//printf("!ProPosUv: (%f,%f)\n",ProPosUv.x,ProPosUv.y);
		//printf("!TmpUv: (%f,%f)\n",TmpUv.x,TmpUv.y);
		//printf("!moveVec:(%f,%f)\n",MoveVec.x,MoveVec.y);
		IterTime++;

	}

	//if(length(TmpUv - make_float2(x,y))> 30){
	//outPut[index] = make_float4(-10.0,-10.0,Origin_Id,RefelctInCameraDepth);
	//return;

	//}

	//if(dot(MoveVec,MoveVec) >=20){
	//outPut[index] = make_float4(-10.0f,-10.0f,Origin_Id,RefelctInCameraDepth);
	//return;

	//}


	//outPut[index] = ProPos;
	//if(TmpUv.x>=0 && TmpUv.x<width && TmpUv.y>=0 && TmpUv.y <= height)
	//	outPut[(int)(TmpUv.y * width + TmpUv.x)] = tex2D(cuda_Reflect_Tex,x+0.5y+0.5);
	//WorlPosInCameraDepth = 10;
	//if(dot(MoveVec,MoveVec)>200)
	//WorlPosInCameraDepth = -10;
	//outPut[index] = make_float4((TmpUv.x - x)/1024.0,(TmpUv.y - y)/1024.0,Origin_Id,RefelctInCameraDepth);
	d_cudaPboBuffer[index] = make_float4(TmpUv.x/(float)width,TmpUv.y/(float)height,Origin_Id,RefelctInCameraDepth);

}*/

__device__ int isInSamePixel(float2 moveVec)
{
	if(abs(moveVec.x)<0.5&&abs(moveVec.y)<0.5)
		return 1;
	else
		return 0;
}
__device__ int nextStep(float2 moveVec,float2* pStepVec)
{
#define PI 3.141592654f
	if(length(moveVec)<0.717)
		return 0;
	if(moveVec.x==0)
	{
		*pStepVec = make_float2(0,copysignf(1,moveVec.y));
		return 1;
	}
	float absTangleValue = abs(moveVec.y/moveVec.x);
	if(absTangleValue<tanf(PI/4/2))
	{
		
		*pStepVec = make_float2(copysignf(1,moveVec.x),0);

	}
	else if (absTangleValue>tanf(PI*3/4/2))
	{
		
		*pStepVec = make_float2(0,copysignf(1,moveVec.y));
	}
	else 
	{
		*pStepVec = make_float2(copysignf(1,moveVec.x),copysignf(1,moveVec.y));
	}
	return 1;
}
__device__ int nextThreeStep(float2 moveVec,float2* candicate1,float2* candicate2,float2* candicate3)
{
	#define PI 3.141592654f
	if(length(moveVec)<0.717)
		return 0;
	if(moveVec.x==0)
	{
		*candicate1 = make_float2(0,copysignf(1,moveVec.y));
		*candicate2 = make_float2(-1,copysignf(1,moveVec.y));
		*candicate3 = make_float2(1,copysignf(1,moveVec.y));
		return 1;
	}
	float absTangleValue = abs(moveVec.y/moveVec.x);
	if(absTangleValue<tanf(PI/4/2))
	{
		
		*candicate1 = make_float2(copysignf(1,moveVec.x),0);
		*candicate2 = make_float2(copysignf(1,moveVec.x),-1);
		*candicate3 = make_float2(copysignf(1,moveVec.x),1);

	}
	else if (absTangleValue>tanf(PI*3/4/2))
	{
		
		*candicate1 = make_float2(0,copysignf(1,moveVec.y));
		*candicate2 = make_float2(-1,copysignf(1,moveVec.y));
		*candicate3 = make_float2(1,copysignf(1,moveVec.y));
	}
	else 
	{
		*candicate1 = make_float2(copysignf(1,moveVec.x),copysignf(1,moveVec.y));
		*candicate2 = make_float2(copysignf(1,moveVec.x),0);
		*candicate3 = make_float2(0,copysignf(1,moveVec.y));
	}
	return 1;
}
#define CONVERGE 5
#define OUTRANGE 2
#define OUTOBJECT 3
#define FASTPROJT 4
__device__ int Floor2Int(float f)
{
	return (int)f+0.5;
}
__device__ int isOutOfRange(float2 tc)
{
	return tc.x>rasterWidth-0.5||tc.y>rasterHeight-0.5||tc.x<0||tc.y<0;
}
__device__ int threePointSearch(float2 currentPlace,float2* moveToVec)
{
#define STEPNUMBER 25
	int x =Floor2Int(currentPlace.x-0.5);
	int y =Floor2Int(currentPlace.y-0.5);
	int index = y * rasterWidth + x;
	float2 currentUv = make_float2(currentPlace.x,currentPlace.y);

	Plane fittingPlane(currentUv);
	
	//if(x!=399||y!=703)
	//   return;
	
	//printf("1Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);
	//�����������ľ���
	//= d_newCameraPos + DisEye2Plane/abs(dot(VecEye2Ref,WorldNormal))* VecEye2Ref;
	fittingPlane.setReflectedPos();
	float3 FinalPos  = fittingPlane.intersectPos();
	float3 ndcPos =  transFormToNdc(FinalPos);
	float2 ProPosUv =make_float2(ndcPos.x,ndcPos.y);
	float rejectDepth = fittingPlane.getMirrorDepth();
	float2 MoveVec = ProPosUv - make_float2(currentUv.x,currentUv.y);
	float formerDis = fittingPlane.getDisToPath();

	
	float3 worldPos = fittingPlane.m_worldPos;
	float3 worldNormal = fittingPlane.m_worldNormal;
	
	/*
	printf("x,y:%d,%d\n",x,y);
	printf("pos:(%f,%f,%f)\n",worldPos.x,worldPos.y,worldPos.z);
	printf("normal:(%f,%f,%f)\n",worldNormal.x,worldNormal.y,worldNormal.z);
	printf("reflected pos:(%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);
	printf("intersect pos:(%f,%f,%f)\n",FinalPos.x,FinalPos.y,FinalPos.z);
	printf("refCamera:(%f,%f,%f)\n",d_refCameraPos.x,d_refCameraPos.y,d_refCameraPos.z);
	printf("newCameraPos:(%f,%f,%f)\n",d_newCameraPos.x,d_newCameraPos.y,d_newCameraPos.z);
	printf("reject Pos:(%f,%f),depth:%f\n",ProPosUv.x,ProPosUv.y,rejectDepth);
	*/
	if(fittingPlane.isAbleFastProj(ProPosUv))
	{
		if(fabs(ProPosUv.x-currentUv.x)<0.5&&(fabs(ProPosUv.y-currentUv.y)<0.5))
		{
			d_cudaPboBuffer[index] =  make_float4(currentUv.x/(float)rasterWidth,currentUv.y/(float)rasterHeight,rejectDepth,CONVERGE);
			return CONVERGE;
		}
		d_cudaPboBuffer[index] =   make_float4(ProPosUv.x/(float)rasterWidth,ProPosUv.y/(float)rasterHeight,rejectDepth,FASTPROJT);
		return FASTPROJT;
	}
	int IterTime = 0;
	while(IterTime<STEPNUMBER/1024.0*rasterWidth)
	{
		//printf("Item:%d currentUv:(%f,%f)\n",IterTime,currentUv.x,currentUv.y);
		
		float2 TmpUv1,TmpUv2,TmpUv3,TmpUv;
		
		TmpUv1 = currentUv;
		TmpUv2 = currentUv;
		TmpUv3 = currentUv;

		//MoveVec = ChengShu * MoveVec;


		if(MoveVec.x > 0)
		{
			TmpUv1.x += 1.0;
			TmpUv3.x += 1.0;
		}
		else{
			TmpUv1.x -= 1.0;
			TmpUv3.x -= 1.0;
		}

		if(MoveVec.y >0){
			TmpUv2.y += 1.0;
			TmpUv3.y += 1.0;

		}
		else{
			TmpUv2.y -= 1.0;
			TmpUv3.y -= 1.0;
		}
		//取出每个采样点对应的世界坐标
		//nextThreeStep(MoveVec,&TmpUv1,&TmpUv2,&TmpUv3);
		/*TmpUv1+=currentUv;
		TmpUv2+=currentUv;
		TmpUv3+=currentUv;*/
		if(isOutOfRange(TmpUv1)||isOutOfRange(TmpUv2)||isOutOfRange(TmpUv3))
		{

			d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,rejectDepth,OUTOBJECT);
		  
			return OUTOBJECT;
		}
		Plane pixelPlane1(TmpUv1),pixelPlane2(TmpUv2),pixelPlane3(TmpUv3);
		pixelPlane1.setReflectedPos(fittingPlane);
		pixelPlane2.setReflectedPos(fittingPlane);
		pixelPlane3.setReflectedPos(fittingPlane);

		float DisPoint2Line1,DisPoint2Line2,DisPoint2Line3;
		DisPoint2Line1 = pixelPlane1.getDisToPath();
		DisPoint2Line2 = pixelPlane2.getDisToPath();
		DisPoint2Line3 = pixelPlane3.getDisToPath();
		
		for(int dx = -1;dx<=1;dx++)
		{
			for(int dy = -1;dy<=1;dy++)
			{
				float2 uv = currentUv+make_float2(dx,dy);
				Plane testPlan(uv);
				testPlan.setReflectedPos(fittingPlane);
				float dis = testPlan.getDisToPath();
				//printf("point(%f,%f,%f)\n",uv.x,uv.y,dis);

			}
		}
		//printf("currentProUV(%f,%f)\n",ProPosUv.x,ProPosUv.y);
		//printf("currentMove(%f,%f)\n",MoveVec.x,MoveVec.y);
		//printf("point1(%f,%f,%f),point2(%f,%f,%f),point3(%f,%f,%f)\n",TmpUv1.x,TmpUv1.y,DisPoint2Line1,TmpUv2.x,TmpUv2.y,DisPoint2Line2,TmpUv3.x,TmpUv3.y,DisPoint2Line3);
		
		Plane minPlane;
		float minDis;
		//
		//printf("DisPoint2Line1:%f\n",DisPoint2Line1);
		if(pixelPlane1.isInValid()&&pixelPlane2.isInValid()&&pixelPlane2.isInValid())
		{	
			//printf("invalid\n");
			
			d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,rejectDepth,OUTOBJECT);
		   //printf("@x:%d y:%d: %f,%f,%f,%f\n",x,y,d_cudaPboBuffer[index] .x,d_cudaPboBuffer[index] .y,d_cudaPboBuffer[index] .z,d_cudaPboBuffer[index] .w);
	
			return OUTOBJECT;
		}
		if(DisPoint2Line2<DisPoint2Line1)
		{
			if(DisPoint2Line3< DisPoint2Line2)
			{
				minPlane = pixelPlane3;
				minDis = DisPoint2Line3;
				// min = 3
			}	
			else
			{
				minPlane = pixelPlane2;
				minDis = DisPoint2Line2;
				//min  = 2
			}
		}
		else
		{
			if(DisPoint2Line3< DisPoint2Line1)
			{
				minPlane = pixelPlane3;
				minDis = DisPoint2Line3;
				// min = 3
			}	
			else
			{
				minPlane = pixelPlane1;		
				minDis = DisPoint2Line1;
				//min  = 1
			}
		}
		if(minDis>formerDis)
		{
			*moveToVec = currentUv;
			if(length(MoveVec)<15/1024.0*rasterWidth&&minDis<5)
			{
				d_cudaPboBuffer[index] =  make_float4(currentUv.x/(float)rasterWidth,currentUv.y/(float)rasterHeight,rejectDepth,CONVERGE);
				//printf("minDis:%f,formerDis:%f,Converge\n",minDis,formerDis);
				return CONVERGE;
			}
			else
			{
				d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,-0.1,OUTOBJECT);
				//printf("not right\n");
				
				return OUTOBJECT;
			
			}
		}
		//TmpUv = TmpUv1;
		
		currentUv = minPlane.getTc();
		formerDis = minDis;
		fittingPlane = minPlane;

		FinalPos  = fittingPlane.intersectPos();
		ndcPos =  transFormToNdc(FinalPos);
	    ProPosUv =make_float2(ndcPos.x,ndcPos.y);
		rejectDepth = fittingPlane.getMirrorDepth();
		MoveVec = ProPosUv - make_float2(currentUv.x,currentUv.y);
		/*
		printf("nextUv minDis:(%f,%f),currentFormerDis:%f\n",currentUv.x,currentUv.y,formerDis);
		printf("ProUV(%f,%f)\n",ProPosUv.x,ProPosUv.y);
		printf("Move(%f,%f)\n",MoveVec.x,MoveVec.y);
		printf("worldPos(%f,%f,%f),normal:(%f,%f,%f):%f\n",fittingPlane.m_worldPos.x,fittingPlane.m_worldPos.y,fittingPlane.m_worldPos.z,fittingPlane.m_worldNormal.x,fittingPlane.m_worldNormal.y,fittingPlane.m_worldNormal.z);
		*/
		IterTime++;
	}
	d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,rejectDepth,OUTRANGE);
				
//	printf("！x:%d y:%d: %f,%f,%f,%f\n",x,y,d_cudaPboBuffer[index] .x,d_cudaPboBuffer[index] .y,d_cudaPboBuffer[index] .z,d_cudaPboBuffer[index] .w);
	
	
	return OUTRANGE;
	
}

__device__ int ninePointSearch(float2 currentPlace,float2* moveToVec)
{
#define STEPNUMBER 25
	int x =Floor2Int(currentPlace.x-0.5);
	int y =Floor2Int(currentPlace.y-0.5);
	int index = y * rasterWidth + x;
	float2 currentUv = make_float2(currentPlace.x,currentPlace.y);

	Plane fittingPlane(currentUv);
	
	//if(x!=140||y!=294)
	 //  return;
	
	//printf("1Class: (%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);
	//�����������ľ���
	//= d_newCameraPos + DisEye2Plane/abs(dot(VecEye2Ref,WorldNormal))* VecEye2Ref;
	fittingPlane.setReflectedPos();
	float3 FinalPos  = fittingPlane.intersectPos();
	float3 ndcPos =  transFormToNdc(FinalPos);
	float2 ProPosUv =make_float2(ndcPos.x,ndcPos.y);
	float rejectDepth = fittingPlane.getMirrorDepth();
	float2 MoveVec = ProPosUv - make_float2(currentUv.x,currentUv.y);
	float formerDis = fittingPlane.getDisToPath();

	
	float3 worldPos = fittingPlane.m_worldPos;
	float3 worldNormal = fittingPlane.m_worldNormal;
	
	/*
	printf("x,y:%d,%d\n",x,y);
	printf("pos:(%f,%f,%f)\n",worldPos.x,worldPos.y,worldPos.z);
	printf("normal:(%f,%f,%f)\n",worldNormal.x,worldNormal.y,worldNormal.z);
	printf("reflected pos:(%f,%f,%f)\n",fittingPlane.m_reflectPos.x,fittingPlane.m_reflectPos.y,fittingPlane.m_reflectPos.z);
	printf("intersect pos:(%f,%f,%f)\n",FinalPos.x,FinalPos.y,FinalPos.z);
	printf("refCamera:(%f,%f,%f)\n",d_refCameraPos.x,d_refCameraPos.y,d_refCameraPos.z);
	printf("newCameraPos:(%f,%f,%f)\n",d_newCameraPos.x,d_newCameraPos.y,d_newCameraPos.z);
	printf("reject Pos:(%f,%f),depth:%f\n",ProPosUv.x,ProPosUv.y,rejectDepth);
	*/
	if(fittingPlane.isAbleFastProj(ProPosUv))
	{
		//printf("fitting\n");
		if(fabs(ProPosUv.x-currentUv.x)<0.5&&(fabs(ProPosUv.y-currentUv.y)<0.5))
		{
			d_cudaPboBuffer[index] =  make_float4(currentUv.x/(float)rasterWidth,currentUv.y/(float)rasterHeight,rejectDepth,CONVERGE);
			return CONVERGE;
		}
				
		d_cudaPboBuffer[index] =   make_float4(ProPosUv.x/(float)rasterWidth,ProPosUv.y/(float)rasterHeight,rejectDepth,FASTPROJT);
		return FASTPROJT;
	}
	int IterTime = 0;
	while(IterTime<STEPNUMBER/1024.0*rasterWidth)
	{
		float minDis = 3000;
		float2 minUv;
		Plane minPlane;
		for(int dx = -1;dx<=1;dx++)
		{
			for(int dy = -1;dy<=1;dy++)
			{
				float2 uv = currentUv+make_float2(dx,dy);
				if(dx==0&&dy==0)
					continue;
				if(isOutOfRange(uv))
					continue;
				Plane testPlan(uv);
				testPlan.setReflectedPos(fittingPlane);
				
				if(testPlan.isInValid())
					continue;
				float dis = testPlan.getDisToPath();
				//printf("point1(%f,%f,%f)\n",uv.x,uv.y,dis);
				if(minDis>dis)
				{
					//printf("min\n");
					minDis = dis;
					minUv = uv;
					minPlane = testPlan;
				}

			}
		}
			//
		//printf("DisPoint2Line1:%f\n",DisPoint2Line1);
		if(minDis == 3000)
		{
			d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,-0.1,OUTOBJECT);
			return OUTOBJECT;
		}
		if(minDis>formerDis)
		{
			*moveToVec = currentUv;
			if(length(MoveVec)<15/1024.0*rasterWidth&&minDis<5)
			{
				//printf("convenge\n");
				d_cudaPboBuffer[index] =  make_float4(currentUv.x/(float)rasterWidth,currentUv.y/(float)rasterHeight,rejectDepth,CONVERGE);
				return CONVERGE;
			}
			else
			{
				//printf("out\n");
				
				d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,-0.1,OUTOBJECT);
				return OUTOBJECT;
			}
		}
		
		currentUv = minPlane.getTc();
		formerDis = minDis;
		fittingPlane = minPlane;

		FinalPos  = fittingPlane.intersectPos();
		ndcPos =  transFormToNdc(FinalPos);
	    ProPosUv =make_float2(ndcPos.x,ndcPos.y);
		rejectDepth = fittingPlane.getMirrorDepth();
		MoveVec = ProPosUv - make_float2(currentUv.x,currentUv.y);
		/*
		printf("nextUv minDis:(%f,%f),currentFormerDis:%f\n",currentUv.x,currentUv.y,formerDis);
		printf("ProUV(%f,%f)\n",ProPosUv.x,ProPosUv.y);
		printf("Move(%f,%f)\n",MoveVec.x,MoveVec.y);
		printf("worldPos(%f,%f,%f),normal:(%f,%f,%f):%f\n",fittingPlane.m_worldPos.x,fittingPlane.m_worldPos.y,fittingPlane.m_worldPos.z,fittingPlane.m_worldNormal.x,fittingPlane.m_worldNormal.y,fittingPlane.m_worldNormal.z);
		*/
		IterTime++;
	}
	d_cudaPboBuffer[index] =   make_float4(-10.0,-10.0,rejectDepth,OUTRANGE);		
	return OUTRANGE;
	
}

__global__ void MyNewKernel(int width,int height)
{
	int x = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	int y = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;
	if( x >width || y> height)
			return;
	//if(x!=91||y!=623)
	//	return;
	
	//printf("x:%d,y:%d\n",x,y);
	/*if(x==1024)
	{
		printf("x:%d,y:%d\n",x,y);
	}*/
	int index = y * width + x;
	float2 currentUv = make_float2(x+0.5,y+0.5);
	
//	d_cudaPboBuffer[index] =   make_float4(currentUv.x/(float)rasterWidth,currentUv.y/(float)rasterHeight,-0.1,FASTPROJT);
//	return;		
	float2 resultValue;
	ninePointSearch(currentUv,&resultValue);
	

}
__global__ void MyKernel(int width,int height)
{
	//+0.5 thresheld


	//return;
	int x = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	int y = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;

	//if( x >width || y> height)
		//int x = 321,y =169;  

	//if(x !=0 || y !=0)
	//if(x!=321||y !=169)

	//if(x!=571||y!=702)
	//if(x!=950||y!=990)
	//if( x >width || y> height)
	//	return;
	int index = y * width + x;
	
	/*if(x>=264||x<258)
	{
		d_cudaPboBuffer[index] = make_float4(-10.0f,-10.0f,1,0.1);
		return;
	}
	if(y>=100||y<90)
	{
		d_cudaPboBuffer[index] = make_float4(-10.0f,-10.0f,1,0.1);
		return;
	};*/
	if( x >width || y> height)
			return;


	//printf("%d,%d\n",x,y);
	//提取数据
	
	float3 WorldPos,WorldNormal;
	float ReflectDis;
	float4 WorldTmp;

	WorldTmp = tex2D(cuda_WorldPos_Tex,x+0.5,y+0.5);

	WorldPos = make_float3(WorldTmp.x,WorldTmp.y,WorldTmp.z);
	float4 WorldNormalTmp = tex2D(cuda_WorlNormal_Tex,x+0.5,y+0.5);

	int Origin_Id = (int)WorldNormalTmp.w;
	WorldNormal = normalize(make_float3(WorldNormalTmp.x,WorldNormalTmp.y,WorldNormalTmp.z));

	ReflectDis = tex2D(cuda_Reflect_Tex,x+0.5,y+0.5).w;


	float3 LookVec = normalize(WorldPos - d_refCameraPos);
	//计算反射光线方向
	float3 ReflectVec = normalize(reflect(LookVec,WorldNormal));
	//计算被反射物体坐标
	float3 ReflectPos = WorldPos + ReflectVec * ReflectDis;
	float RefelctInCameraDepth;

	RefelctInCameraDepth = ReflectDis /10000.0;

	float WorlPosInCameraDepth = 1000;
	//RefelctInCameraDepth  = TMpReflecPosInCamera.z / TMpReflecPosInCamera.w;


	float CosReCorner = dot(ReflectVec,WorldNormal);
	//镜像点的坐标
	float3 ReMirrorPos = ReflectDis * CosReCorner * 2 * (-1) * WorldNormal + ReflectPos;


	//相机到反射面的距离
	float DisEye2Plane = abs(dot(d_newCameraPos- WorldPos,WorldNormal));
	//镜像点与相机的向量
	float3 VecEye2Ref = normalize(ReMirrorPos - d_newCameraPos);

	//镜像点与相机连线和反射平面的交点
	float3 FinalPos ;//= d_newCameraPos + DisEye2Plane/abs(dot(VecEye2Ref,WorldNormal))* VecEye2Ref;

	FinalPos = dot(WorldPos -d_newCameraPos ,WorldNormal)/ dot(VecEye2Ref,WorldNormal) * VecEye2Ref + d_newCameraPos;

	//printf("^^^^^^^^^^^^^^^^^^^MyKernel^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	//printf("^^^^^^^^WorldPos^^^:%f,%f,%f\n",WorldPos.x,WorldPos.y,WorldPos.z);
	//printf("^^^^^^^^WorldNormal^^^:%f,%f,%f\n",WorldNormal.x,WorldNormal.y,WorldNormal.z);
	//printf("^^^^^^^^d_newCameraPos^^^:%f,%f,%f\n",d_newCameraPos.x,d_newCameraPos.y,d_newCameraPos.z);

	//printf("^^^^^^^^ReflectPos^^^:%f,%f,%f\n",ReflectPos.x,ReflectPos.y,ReflectPos.z);
	//printf("^^^^^^^^ReMirrorPos^^^:%f,%f,%f\n",ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z);
	//printf("^^^^^^^^VecEye2Ref^^^:%f,%f,%f\n",VecEye2Ref.x,VecEye2Ref.y,VecEye2Ref.z);

	//printf("^^^^^^^^FinalPos_F^^^:%f,%f,%f\n",FinalPos.x,FinalPos.y,FinalPos.z);
	//printf("^^^^^^^^MirrorPos：%f,%f,%f\n",ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z);
	
	//FinalPos = abs(dot(Camera_Vec_Normal3,d_refCameraPos - ReMirrorPos3)/dot(Camera_Vec_Normal3,normalize(WorldPos3-ReMirrorPos3))) * normalize(WorldPos3-ReMirrorPos3) + d_newCameraPos;

	float FinalPos_F[4];
	FinalPos_F[0] = FinalPos.x;
	FinalPos_F[1] = FinalPos.y;
	FinalPos_F[2] = FinalPos.z;
	FinalPos_F[3] = 1.0;
	float4 ProPos = Cude_Mul_Mat4(Cuda_MvpMat,FinalPos_F);
	//printf("^^^^^^^^ProPos^^^:%f,%f,%f,%f\n",ProPos.x,ProPos.y,ProPos.z,ProPos.w);

	//outPut[index] = make_float4(ProPos.x,ProPos.y,ProPos.z,1.0);

	//return;

	//outPut[index] = WorldTmp;
	//return ;
	//Pro 位置
	ProPos.x = ProPos.x / ProPos.w;
	ProPos.y = ProPos.y / ProPos.w;

	//ProPos.x = (ProPos.x * 0.5 +0.5 ) * width;
	//ProPos.y = (ProPos.y * 0.5 +0.5 ) * height;

	//float2 TmpUv ,ProPosUv;
	float2 ProPosUv;

	float2 cuurrentUv = make_float2((x+0.5)*1.0,(y+0.5)*1.0);;
	//int2 cuurrentUv;

	ProPosUv.x = (ProPos.x * 0.5 +0.5 ) * width;
	ProPosUv.y = (ProPos.y * 0.5 +0.5 ) * height;
	//float2 writenPlace = make_float2(ProPosUv.x-cuurrentUv.x,ProPosUv.y-cuurrentUv.y);
	//d_cudaPboBuffer[index] =  make_float4(ProPosUv.x/(float)width,ProPosUv.y/(float)height,Origin_Id,RefelctInCameraDepth);
	//printf("refCam:%f,%f,%f\n",d_refCameraPos.x,d_refCameraPos.y,d_refCameraPos.z);
	//printf("newCam:%f,%f,%f\n",d_newCameraPos.x,d_newCameraPos.y,d_newCameraPos.z);

	//printf("result(%f,%f)\n",ProPosUv.x,ProPosUv.y);

	//printf("ProPosUV:%f,%f\n",ProPosUv.x,ProPosUv.y);
	
	//outPut[index] = make_float4(WorldNormal.x,WorldNormal.y,WorldNormal.z,1.0);
	//return;
	float4 TmpFloat4;

	TmpFloat4 = tex2D(cuda_WorldPos_Tex,ProPosUv.x,ProPosUv.y);
	float3	test = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
	//printf("value:(%f,%f,%f)\n",test.x,test.y,test.z);
	float2 MoveVec = ProPosUv - make_float2((x+0.5)*1.0,(y+0.5)*1.0);

	//outPut[index] = make_float4(MoveVec.x,MoveVec.y,dot(MoveVec,MoveVec),ReflectDis);
	//return;

	//if(Cuda_FramMent[0]%10 ==1)

	//else
	//cuurrentUv = LastVec[index];

	//outPut[index] = make_float4((cuurrentUv.x+50)/1024.0,(cuurrentUv.y+50)/1024.0,Origin_Id,RefelctInCameraDepth);
	//return;

	//outPut[index] = make_float4(MoveVec.x/1024.0,MoveVec.y/1024.0,Origin_Id,RefelctInCameraDepth);
	//outPut[index] = make_float4(ProPosUv.x/1024.0,ProPosUv.y/1024.0,Origin_Id,RefelctInCameraDepth);
	//return;
	//cuurrentUv = make_int2(x,y);
	//return ;
	int IterTime = 0;

	float LastCosLength = 0;

	float ChengShu = 1.0f;

	float2 TmpMov = make_float2(0,0);

	float2 TheComputVU = make_float2(-1.0f,-1.0f);
	if(MoveVec.x >=0)
		TheComputVU.x =1;
	if(MoveVec.y >=0)
		TheComputVU.y =1;

	float Tmpfloat = Cuda_Compute_Cos(TheComputVU + cuurrentUv,ReflectPos,Origin_Id);
	if(Tmpfloat>-50 && Tmpfloat < 0)
		ChengShu = -1;
	//	else
	//ChengShu = 1;



	//if(Tmpfloat >-50 &&Tmpfloat <0)
	//ChengShu = -1;
	float LastCoslength;
	//while(IterTime<Cuda_FramMent[0] * 5 && dot(MoveVec,MoveVec) >=2.5){


	//outPut[index] = make_float4(cuurrentUv.x/1024.0,cuurrentUv.y/1024.0,Origin_Id,RefelctInCameraDepth);
	//return;
	//printf("^^^^^^^^cuurrentUv^:%f,%f\n",cuurrentUv.x,cuurrentUv.y);
	//printf("^^^^^^^^MoveVec^^^:%f,%f\n",MoveVec.x,MoveVec.y);
	float3	Cam_Mirror_Normal = normalize(cross(d_cameraVec,normalize(d_refCameraPos - ReMirrorPos)));
	float3	Camera_Vec_Normal = normalize(cross(Cam_Mirror_Normal,d_cameraVec));
	float3 insertPoint = abs(dot(Camera_Vec_Normal,d_refCameraPos - ReMirrorPos)/dot(Camera_Vec_Normal,normalize(WorldPos-ReMirrorPos))) * normalize(WorldPos-ReMirrorPos) + ReMirrorPos;
	//printf("^^^^^^^^insertPoint^^^:%f,%f,%f\n",insertPoint.x,insertPoint.y,insertPoint.z);
	//printf("^^^^^^^^d_refCameraPos^^^:%f,%f,%f\n",d_refCameraPos.x,d_refCameraPos.y,d_refCameraPos.z);


	//float temp1 = dot(InsertPoint - d_refCameraPos,d_cameraVec);

	//float temp2 = dot(InsertPoint - d_refCameraPos,InsertPoint - d_refCameraPos);
	float3 toNewPlace = d_newCameraPos-insertPoint;
	//float	formerDis = dot(toNewPlace,d_cameraVec);
	float formerDis = length(toNewPlace);
	//printf("^^^^^^^^temp1,temp2,formerDis^^^:,%f\n",formerDis);
	float currentMaxDis = -10000;
		
	while(IterTime<25)
	{
		//while(1){
		//printf("in the loop  %d\n",IterTime);
		float2 TheComputVU = make_float2(-1.0f,-1.0f);
		if(MoveVec.x >=0)
			TheComputVU.x =1;
		if(MoveVec.y >=0)
			TheComputVU.y =1;
		//TheComputVU = TheComputVU + cuurrentUv;
		//printf("^^^^^^^^^^^^^^^^formerDis:%f^^^^^^^^^^\n",formerDis);
		//printf("IterTime:%d ^^^^^^^^cuurrentUv^^^:%f,%f\n",IterTime,cuurrentUv.x,cuurrentUv.y);
		
		/*if(1==nextStep(MoveVec,&TheComputVU))
		{
			printf("^^^^^^^^TheComputVU^^^:%f,%f\n",TheComputVU.x,TheComputVU.y);
			cuurrentUv += TheComputVU;
			printf("^^^^^^^^cuurrentUv^^^:%f,%f\n",cuurrentUv.x,cuurrentUv.y);
			float4 TmpFloat41= tex2D(cuda_WorlNormal_Tex,cuurrentUv.x,cuurrentUv.y);
			WorldNormal  = make_float3(TmpFloat41.x,TmpFloat41.y,TmpFloat41.z);
			TmpFloat41 = tex2D(cuda_WorldPos_Tex,cuurrentUv.x,cuurrentUv.y);
			WorldPos  = make_float3(TmpFloat41.x,TmpFloat41.y,TmpFloat41.z);
			float NowModelId = TmpFloat41.w;



			//outPut[index] = make_float4(WorldNormal.x,WorldNormal.y,WorldNormal.z,1.0);
			//return;
			//得出以该点为平面的反射物体的镜像点
			ReMirrorPos =  abs(dot(WorldPos - ReflectPos,WorldNormal)) * (-2) * WorldNormal + ReflectPos;



			//outPut[index] = make_float4(ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z,1.0);
			//return;

			//求出该点与相机连线和平面的新交点
			DisEye2Plane = abs(dot(d_newCameraPos - WorldPos,WorldNormal));
			float CoseTheta2 = abs(dot(normalize(d_newCameraPos - ReMirrorPos),WorldNormal));
			VecEye2Ref = normalize(ReMirrorPos - d_newCameraPos);
			float3 FinalPos2;
			//	FinalPos2 = d_newCameraPos + VecEye2Ref * (DisEye2Plane/CoseTheta2 );
			FinalPos2 = dot(WorldPos -d_newCameraPos ,WorldNormal)/ dot(VecEye2Ref,WorldNormal) * VecEye2Ref + d_newCameraPos;

			//FinalPos2.y =  d_newCameraPos.y + VecEye2Ref.y *(DisEye2Plane/CoseTheta2);

			FinalPos_F[0] = FinalPos2.x;
			FinalPos_F[1] = FinalPos2.y;
			FinalPos_F[2] = FinalPos2.z;
			FinalPos_F[3] = 1.0;
			float4 ProPos2;
			ProPos2 = Cude_Mul_Mat4(Cuda_MvpMat,FinalPos_F);

			ProPosUv.x = (ProPos2.x / ProPos2.w * 0.5 + 0.5) * width;
			ProPosUv.y = (ProPos2.y / ProPos2.w * 0.5 + 0.5) * height;
			printf("now proposUv:(%f,%f)\n",ProPosUv.x,ProPosUv.y);
			//MoveVec = ProPosUv - make_float2(cuurrentUv.x,cuurrentUv.y);
			MoveVec = ProPosUv - cuurrentUv;
			printf("MoveVec:(%f,%f)\n",MoveVec.x,MoveVec.y);
			
			//outPut[index] = make_float4(ProPosUv.x,ProPosUv.y,ProPos2.z,ProPos2.w);

			//return;
			IterTime++;
		}
		else
		{
			printf("$$$$$$$$$$$$$$$$$$$$$$result (%f,%f)\n",cuurrentUv.x,cuurrentUv.y);
			d_cudaPboBuffer[index] =  make_float4(cuurrentUv.x/(float)width,cuurrentUv.y/(float)height,Origin_Id,RefelctInCameraDepth);
			return;
		}
		*/
		
		float2 TmpUv1,TmpUv2,TmpUv3;
		// 		if(ChengShu >0){
		// 		float Tmpfloat = Cuda_Compute_Cos(TheComputVU + cuurrentUv,ReflectPos,Origin_Id);
		// 		if(Tmpfloat < -50 && Tmpfloat<0 )
		// 			ChengShu = -1;
		// 		else
		// 			ChengShu = 1;
		// 		}

		//int2 TmpUv1,TmpUv2,TmpUv3; 

		TmpUv1 = cuurrentUv;
		TmpUv2 = cuurrentUv;
		TmpUv3 = cuurrentUv;

		//MoveVec = ChengShu * MoveVec;


		if(MoveVec.x > 0)
		{
			TmpUv1.x += 1.0;
			TmpUv3.x += 1.0;
		}
		else
		{
			TmpUv1.x -= 1.0;
			TmpUv3.x -= 1.0;
		}

		if(MoveVec.y >0)
		{
			TmpUv2.y += 1.0;
			TmpUv3.y += 1.0;

		}
		else
		{
			TmpUv2.y -= 1.0;
			TmpUv3.y -= 1.0;
		}
		float2 TmpUv4 = cuurrentUv;
		//TmpUv1.x = ProPosUv.x;
		//TmpUv1.y = ProPosUv.y;

		//printf("TmpUv1 :%f,%f\n",TmpUv1.x,TmpUv1.y);
		//printf("TmpUv2 :%f,%f\n",TmpUv2.x,TmpUv2.y);
		//printf("TmpUv3 :%f,%f\n",TmpUv3.x,TmpUv3.y);
		//printf("TmpUv4 :%f,%f\n",TmpUv4.x,TmpUv4.y);

		//取出每个采样点对应的世界坐标
		float4 TmpFloat4;
		float3 WorldPos1,WorldPos2,WorldPos3;
		float3 WorldNormal1,WorldNormal2,WorldNormal3;
		int Now_ModelId1,Now_ModelId2,Now_ModelId3;

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv1.x,TmpUv1.y);
		WorldPos1 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv2.x,TmpUv2.y);
		WorldPos2 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv3.x,TmpUv3.y);
		WorldPos3 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);

		TmpFloat4 = tex2D(cuda_WorldPos_Tex,TmpUv4.x,TmpUv4.y);
		float3 WorldPos4 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);


		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv1.x,TmpUv1.y);
		WorldNormal1 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId1 = (int)TmpFloat4.w;

		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv3.x,TmpUv3.y);
		WorldNormal3 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId3 = (int)TmpFloat4.w;

		TmpFloat4 = tex2D(cuda_WorlNormal_Tex,TmpUv2.x,TmpUv2.y);
		WorldNormal2 = make_float3(TmpFloat4.x,TmpFloat4.y,TmpFloat4.z);
		Now_ModelId2 = (int)TmpFloat4.w;



		//printf("WorldPos1 :%f,%f,%f\n",WorldPos1.x,WorldPos1.y,WorldPos1.z);
		//printf("WorldPos2 :%f,%f,%f\n",WorldPos2.x,WorldPos2.y,WorldPos2.z);
		//printf("WorldPos3 :%f,%f,%f\n",WorldPos3.x,WorldPos3.y,WorldPos3.z);
		//printf("WorldPos4 :%f,%f,%f\n",WorldPos4.x,WorldPos4.y,WorldPos4.z);



		//outPut[index] = make_float4(TmpUv2.x,TmpUv2.y,WorldPos1.z,1.0);
		//outPut[index] = make_float4((cuurrentUv.x - x)/1024.0,(cuurrentUv.y - y)/1024.0,Origin_Id,RefelctInCameraDepth);
		//return;


		//求出新的镜像点
		float3 ReMirrorPos1,ReMirrorPos2,ReMirrorPos3;

		ReMirrorPos1 =  abs(dot(WorldPos1 - ReflectPos,WorldNormal1)) * (-2) * WorldNormal1 + ReflectPos;

		ReMirrorPos2 =  abs(dot(WorldPos2 - ReflectPos,WorldNormal2)) * (-2) * WorldNormal2 + ReflectPos;

		ReMirrorPos3 =  abs(dot(WorldPos3 - ReflectPos,WorldNormal3)) * (-2) * WorldNormal3 + ReflectPos;

		//printf("ReMirrorPos1(%f,%f,%f)\n",ReMirrorPos1.x,ReMirrorPos1.y,ReMirrorPos1.z);
		//printf("ReMirrorPos2(%f,%f,%f)\n",ReMirrorPos2.x,ReMirrorPos2.y,ReMirrorPos2.z);
		//printf("ReMirrorPos3(%f,%f,%f)\n",ReMirrorPos3.x,ReMirrorPos3.y,ReMirrorPos3.z);

		//求相机与镜像点组成的平面的法线
		float3 Cam_Mirror_Normal1,Cam_Mirror_Normal2,Cam_Mirror_Normal3;
		Cam_Mirror_Normal1 = normalize(cross(d_cameraVec,normalize(d_refCameraPos - ReMirrorPos1)));
		Cam_Mirror_Normal2 = normalize(cross(d_cameraVec,normalize(d_refCameraPos - ReMirrorPos2)));
		Cam_Mirror_Normal3 = normalize(cross(d_cameraVec,normalize(d_refCameraPos - ReMirrorPos3)));

		//求相机轨迹所在法线
		float3 Camera_Vec_Normal1,Camera_Vec_Normal2,Camera_Vec_Normal3;
		Camera_Vec_Normal1 = normalize(cross(Cam_Mirror_Normal1,d_cameraVec));
		Camera_Vec_Normal2 = normalize(cross(Cam_Mirror_Normal2,d_cameraVec));
		Camera_Vec_Normal3 = normalize(cross(Cam_Mirror_Normal3,d_cameraVec));


		//printf("Camera_Vec_Normal1(%f,%f,%f)\n",Camera_Vec_Normal1.x,Camera_Vec_Normal1.y,Camera_Vec_Normal1.z);
		//printf("Camera_Vec_Normal2(%f,%f,%f)\n",Camera_Vec_Normal2.x,Camera_Vec_Normal2.y,Camera_Vec_Normal2.z);
		//printf("Camera_Vec_Normal3(%f,%f,%f)\n",Camera_Vec_Normal3.x,Camera_Vec_Normal3.y,Camera_Vec_Normal3.z);

		//分别求三个点与该平面的交点
		float3 InsertPoint1 = abs(dot(Camera_Vec_Normal1,d_refCameraPos - ReMirrorPos1)/dot(Camera_Vec_Normal1,normalize(WorldPos1-ReMirrorPos1))) * normalize(WorldPos1-ReMirrorPos1) + ReMirrorPos1;
		float3 InsertPoint2 = abs(dot(Camera_Vec_Normal2,d_refCameraPos - ReMirrorPos2)/dot(Camera_Vec_Normal2,normalize(WorldPos2-ReMirrorPos2))) * normalize(WorldPos2-ReMirrorPos2) + ReMirrorPos2;
		float3 InsertPoint3 = abs(dot(Camera_Vec_Normal3,d_refCameraPos - ReMirrorPos3)/dot(Camera_Vec_Normal3,normalize(WorldPos3-ReMirrorPos3))) * normalize(WorldPos3-ReMirrorPos3) + ReMirrorPos3;

		//printf("dis:(%f,%f,%f)\n",length(InsertPoint1-d_newCameraPos),length(InsertPoint2-d_newCameraPos),length(InsertPoint3-d_newCameraPos));
		//求点与直线的距离


		float3 toNewPlace1 = d_newCameraPos-InsertPoint1;
		//float	DisPoint2Line1 = dot(toNewPlace1,d_cameraVec); ;
		float DisPoint2Line1 = length(toNewPlace1);
		//DisPoint2Line1  =  dot(normalize(InsertPoint1 - d_refCameraPos),d_cameraVec);

		float3 toNewPlace2 = d_newCameraPos-InsertPoint2;
		// float	DisPoint2Line2 = dot(toNewPlace2,d_cameraVec); ;
		float DisPoint2Line2 = length(toNewPlace2);
		//DisPoint2Line2  =  dot(normalize(InsertPoint2 - d_refCameraPos),d_cameraVec);

		float3 toNewPlace3 = d_newCameraPos-InsertPoint3;
		//float	DisPoint2Line3 = dot(toNewPlace3,d_cameraVec); ;
		float DisPoint2Line3 = length(toNewPlace3);
		//DisPoint2Line3  =  dot(normalize(InsertPoint3 - d_refCameraPos),d_cameraVec);
		//printf("1:(%f,%f,%f) 2:(%f,%f,%f),3:(%f,%f,%f)\n",TmpUv1.x,TmpUv1.y,DisPoint2Line1,TmpUv2.x,TmpUv2.y,DisPoint2Line2,TmpUv3.x,TmpUv3.y,DisPoint2Line3);
		//
		if(formerDis<currentMaxDis)
		{
			//printf("$$$$$$$$$$$$$$$$$$$$$$search converge (%f,%f)\n",cuurrentUv.x,cuurrentUv.y);
			d_cudaPboBuffer[index] =  make_float4(cuurrentUv.x/(float)width,cuurrentUv.y/(float)height,1,-0.1);
	
			return;
		}
		float TmpDis;
		if(DisPoint2Line2<DisPoint2Line1)
		{
		TmpUv1 = TmpUv2;
		TmpUv2 = cuurrentUv;
		WorldPos = WorldPos1;
		WorldPos1 = WorldPos2;
		WorldPos2 = WorldPos;

		TmpDis = DisPoint2Line1;
		DisPoint2Line1 = DisPoint2Line2;
		DisPoint2Line2 = TmpDis;

		TmpDis = Now_ModelId1;
		Now_ModelId1 = Now_ModelId2;
		Now_ModelId2 = TmpDis;

		}

		if(DisPoint2Line3< DisPoint2Line1)
		{

		TmpUv1 = TmpUv3;
		TmpUv3 = cuurrentUv;
		WorldPos = WorldPos1;
		WorldPos1 = WorldPos3;
		WorldPos3 = WorldPos;

		TmpDis = DisPoint2Line1;
		DisPoint2Line1 = DisPoint2Line3;
		DisPoint2Line3 = TmpDis;

		TmpDis = Now_ModelId1;
		Now_ModelId1 = Now_ModelId3;
		Now_ModelId3 = TmpDis;
		}

		if(DisPoint2Line3< DisPoint2Line2)
		{

		TmpUv2 = TmpUv3;
		TmpUv3 = cuurrentUv;
		WorldPos = WorldPos2;
		WorldPos2 = WorldPos3;
		WorldPos3 = WorldPos;

		TmpDis = DisPoint2Line3;
		DisPoint2Line3 = DisPoint2Line2;
		DisPoint2Line2 = TmpDis;

		TmpDis = Now_ModelId3;
		Now_ModelId3 = Now_ModelId2;
		Now_ModelId2 = TmpDis;
		}


		//cuurrentUv = TmpUv1;
		if(Now_ModelId1 == Origin_Id)
		{
		currentMaxDis = DisPoint2Line1;
		cuurrentUv = TmpUv1;
		WorldPos = WorldPos1;
		}
		// 		else{
		// 		outPut[index] = make_float4(-10.0f,-10.0f,Origin_Id,RefelctInCameraDepth);
		// 			return;
		// 		}

		else if(Now_ModelId2 == Origin_Id)
		{

		//printf("equal id 2\n");

		currentMaxDis = DisPoint2Line2;
		cuurrentUv = TmpUv2;
		WorldPos = WorldPos2;
		}
		else if(Now_ModelId3 == Origin_Id)
		{

		//printf("equal id 3\n");
		currentMaxDis = DisPoint2Line3;
		cuurrentUv = TmpUv3;
		WorldPos = WorldPos3;
		}
		else
		{
		d_cudaPboBuffer[index] = make_float4(-10.0f,-10.0f,1,-0.1);
	
		return;
		}
		


		//printf("^^^^^^^^DisPoint2Line:%f,%f,%f,%f\n",DisPoint2Line1,DisPoint2Line2,DisPoint2Line3,formerDis);
		formerDis = currentMaxDis;
		//LastCoslength = Cuda_Compute_Cos(cuurrentUv,ReflectPos,Origin_Id);
		//LastCoslength = 0;
		//if(LastCoslength > length(Cuda_CameraPos1[1] - Cuda_CameraPos2[1])-0.5)
		//break;

		//printf("^^^^^^^^^^^^^^^^^^^^result(%f,%f)^^^^^\n",cuurrentUv.x,cuurrentUv.y);




		//得出该点法线
		float4 TmpFloat41= tex2D(cuda_WorlNormal_Tex,cuurrentUv.x,cuurrentUv.y);
		WorldNormal  = make_float3(TmpFloat41.x,TmpFloat41.y,TmpFloat41.z);

		float NowModelId = TmpFloat41.w;



		//outPut[index] = make_float4(WorldNormal.x,WorldNormal.y,WorldNormal.z,1.0);
		//return;
		//得出以该点为平面的反射物体的镜像点
		ReMirrorPos =  abs(dot(WorldPos - ReflectPos,WorldNormal)) * (-2) * WorldNormal + ReflectPos;



		//outPut[index] = make_float4(ReMirrorPos.x,ReMirrorPos.y,ReMirrorPos.z,1.0);
		//return;

		//求出该点与相机连线和平面的新交点
		DisEye2Plane = abs(dot(d_newCameraPos - WorldPos,WorldNormal));
		float CoseTheta2 = abs(dot(normalize(d_newCameraPos - ReMirrorPos),WorldNormal));
		VecEye2Ref = normalize(ReMirrorPos - d_newCameraPos);
		float3 FinalPos2;
		//	FinalPos2 = d_newCameraPos + VecEye2Ref * (DisEye2Plane/CoseTheta2 );
		FinalPos2 = dot(WorldPos -d_newCameraPos ,WorldNormal)/ dot(VecEye2Ref,WorldNormal) * VecEye2Ref + d_newCameraPos;

		//FinalPos2.y =  d_newCameraPos.y + VecEye2Ref.y *(DisEye2Plane/CoseTheta2);

		FinalPos_F[0] = FinalPos2.x;
		FinalPos_F[1] = FinalPos2.y;
		FinalPos_F[2] = FinalPos2.z;
		FinalPos_F[3] = 1.0;
		float4 ProPos2;
		ProPos2 = Cude_Mul_Mat4(Cuda_MvpMat,FinalPos_F);

		ProPosUv.x = (ProPos2.x / ProPos2.w * 0.5 + 0.5) * width;
		ProPosUv.y = (ProPos2.y / ProPos2.w * 0.5 + 0.5) * height;
		
		//MoveVec = ProPosUv - make_float2(cuurrentUv.x,cuurrentUv.y);
		MoveVec = ProPosUv - cuurrentUv;

		//printf("now proposUv:(%f,%f)\n",ProPosUv.x,ProPosUv.y);
	
		//outPut[index] = make_float4(ProPosUv.x,ProPosUv.y,ProPos2.z,ProPos2.w);

		//return;
		IterTime++;
		

	}

	//if(length(cuurrentUv - make_float2(x,y))> 30){
	//outPut[index] = make_float4(-10.0,-10.0,Origin_Id,RefelctInCameraDepth);
	//return;

	//}

	//if(dot(MoveVec,MoveVec) >=20){
	//outPut[index] = make_float4(-10.0f,-10.0f,Origin_Id,RefelctInCameraDepth);
	//return;

	//}


	//outPut[index] = ProPos;
	//if(cuurrentUv.x>=0 && cuurrentUv.x<width && cuurrentUv.y>=0 && cuurrentUv.y <= height)
	//	outPut[(int)(cuurrentUv.y * width + cuurrentUv.x)] = tex2D(cuda_Reflect_Tex,x,y);
	//WorlPosInCameraDepth = 10;
	//if(dot(MoveVec,MoveVec)>200)
	//WorlPosInCameraDepth = -10;
	//outPut[index] = make_float4((cuurrentUv.x - x)/1024.0,(cuurrentUv.y - y)/1024.0,Origin_Id,RefelctInCameraDepth);

	d_cudaPboBuffer[index] =  make_float4(cuurrentUv.x/(float)width,cuurrentUv.y/(float)height,1,-0.1);
	//printf("！x:%d y:%d: %f,%f,%f,%f\n",x,y,1024*d_cudaPboBuffer[index] .x,1024*d_cudaPboBuffer[index] .y,d_cudaPboBuffer[index] .z,d_cudaPboBuffer[index] .w);
	

	//float4 test = tex2D(cuda_Reflect_Tex,TmpUv.x,TmpUv.y);
	//d_cudaPboBuffer[index] =  test;

	//outPut[index] = make_float4((12)/256.0,(13)/256.0,0,1);
	//outPut[index] = make_float4(TmpUv.x,TmpUv.y,IterTime,1);


}
extern "C" void MappingContantPeremeter(int* w,int* h)
{


	checkCudaErrors(cudaMemcpyToSymbol(d_width,w,sizeof(int)));
	checkCudaErrors(cudaMemcpyToSymbol(d_height,h,sizeof(int)));


}
extern "C" void TransConstData(const float *MvpMat,const float *ViewMat,float3 *pRefCamera,float3 *pNewCameraPos)
{

	checkCudaErrors(cudaMemcpyToSymbol(Cuda_MvpMat,MvpMat,sizeof(float)*16,0,cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpyToSymbol(Cuda_CameraViewMat,ViewMat,sizeof(float)*16,0,cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpyToSymbol(d_refCameraPos,pRefCamera,sizeof(float3),0,cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpyToSymbol(d_newCameraPos,pNewCameraPos,sizeof(float3),0,cudaMemcpyHostToDevice));

	float3 CameraVec = normalize(*pNewCameraPos - *pRefCamera);
	checkCudaErrors(cudaMemcpyToSymbol(d_cameraVec,&CameraVec,sizeof(float3),0,cudaMemcpyHostToDevice));

}


__device__ bool colorEqual(float3 soourceColor, float3 targetColor)
{
	return length(soourceColor-targetColor)<0.1;
}
__global__ void MyFirstPassKernel(int width,int height,uint *PixelState,float4 * cuda_PBO_Buffer)
{

	int x = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	int y = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;
	if(x > width || y > height)
		//if(x!=224||y!=267)
			return;

	//if(x !=224 || y !=267)
	//	if(x !=117 || y !=712)
	//	return;

	int index = y * width + x;

	//printf("$%d,%d\n",x,y);

	float4 StateValue =tex2D(cuda_RePro_Pixel,x+0.5,y+0.5);
	float3 reflectColor = make_float3(StateValue.x,StateValue.y,StateValue.z);

	if(colorEqual(reflectColor,make_float3(1,0,0))||colorEqual(reflectColor,make_float3(0,1,0))||reflectColor.x>1.01||reflectColor.y>1.01)  // fetch the color in additional tex
	{
		d_state[index] = 1;
	}
	else
	{
		//printf("ddddddd\n");
		d_state[index] = 0;
	}

}
extern "C" void cudaPredict(int width,int height)
{
	dim3 blockSize(16,16,1);
	dim3 gridSize(width/blockSize.x,height/blockSize.y,1);
	MyNewKernel<<<gridSize,blockSize>>>(width,height);
}

void mapThustResourse()
{
	thrust::device_ptr<uint> dp = &thrustStateVec[0];
	uint * raw_ptr = thrust :: raw_pointer_cast ( dp );  
}
__global__ void MyThridPass(uint *PixePos,uint *PixelState,uint *PixelSums);

extern "C" void RunFirstPass(dim3 BlockSize,dim3 GridSize,int width,int height)
{
	thrust::device_ptr<uint> dp = &thrustStateVec[0];
	uint * raw_ptr = thrust :: raw_pointer_cast ( dp );  
	MyFirstPassKernel<<<GridSize,BlockSize>>>(width,height,raw_ptr,cuda_PBO_Buffer);
	


}


__global__ void MyThridPass(uint *PixePos,uint *PixelState,uint *PixelSums)
{

	int x = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	int y = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;

	if(x > rastWidth || y > rastHeight)
		return;
	uint index = y * rasterWidth + x;
	if(PixelState[index] == 1)
	{
		uint Pos1 = PixelSums[index];
		PixePos[Pos1] = index;

	}
}
void InitThrust()
{	
	thrustSumVec = thrust::device_vector<uint>(rastWidth*rastHeight); 
	thrustStateVec = thrust::device_vector<uint>(rastWidth*rastHeight);
	thrustPosVec = thrust::device_vector<uint>(rastWidth*rastHeight);

	thrust::device_ptr<uint> d_pstate = &thrustStateVec[0];
	g_PixelState = thrust :: raw_pointer_cast ( d_pstate );  
	checkCudaErrors(cudaMemcpyToSymbol(d_state, &g_PixelState,sizeof(int*)));

	thrust::device_ptr<uint> d_psum = &thrustSumVec[0];
	g_PixelSums = thrust :: raw_pointer_cast ( d_psum );
	checkCudaErrors(cudaMemcpyToSymbol(d_sum, &g_PixelSums,sizeof(int*)));

	thrust::device_ptr<uint> dp = &thrustPosVec[0];
	g_PixelPos= thrust :: raw_pointer_cast ( dp );
	checkCudaErrors(cudaMemcpyToSymbol(d_pos, &g_PixelPos,sizeof(int*)));

}



extern "C" void RunThridPass(dim3 BlockSize ,dim3 GridSize,int totalNum)
{	  	
	thrust::device_ptr<uint> dp = &thrustPosVec[0];
	uint * raw_ptr_pos = thrust :: raw_pointer_cast ( dp );  

	thrust::device_ptr<uint> d_pstate = &thrustStateVec[0];
	uint * raw_ptr_state = thrust :: raw_pointer_cast ( d_pstate );  

	thrust::device_ptr<uint> d_psum = &thrustSumVec[0];
	uint * raw_ptr_sum = thrust :: raw_pointer_cast ( d_psum );
	MyThridPass<<<GridSize,BlockSize>>>(raw_ptr_pos,raw_ptr_state,raw_ptr_sum);

	uint* h_state = thrust::raw_pointer_cast(d_pstate);

	//thrust::host_vector <uint> h_pos =thrustSumVec;
	thrust::host_vector <uint> h_pos =thrustPosVec;
	/* 
	for(int y = 0;y<1024;y++)
	{
		for(int x = 0;x<1024;x++)
		{
			int index = 1024*y+x;
			if(h_state[index] == 1)
			{

			int stateValue = h_state[index];
			int sumValue = h_sum[index];
			printf("index(%d,%d,%d) v:(%d,%d)\n",x,y,index,stateValue,sumValue);
			}
		}
	}
	*/
	/*for(int i =totalNum*10/11.0;i<totalNum;i++)
	{

		int value = h_pos[i];
		int	y	 =  value/1024;
 		int x  = value%1024;
		printf("(%d,%d,pos:%d，index:%d)\n",x,y,h_pos[i],i);
	}*/

	
}
int thrustReduction(int width,int height)
{

	dim3 blockSize(16,16,1);
	dim3 GridSize(width/blockSize.x,height/blockSize.y,1);
	RunFirstPass(blockSize,GridSize,width,height);
	thrust::inclusive_scan(thrustStateVec.begin(), thrustStateVec.end() , thrustSumVec.begin());
	thrust::host_vector<uint> hSum = thrustSumVec;

	int totalNum = thrustSumVec[width*height-1]+thrustStateVec[width*height-1];
	RunThridPass(blockSize,GridSize,totalNum);
	//return totalNum;

	return totalNum;
}
extern "C"  void cudaRelateTex(CudaTexResourse * pResouce)
{

	cudaArray *tmpcudaArray;
	cudaGraphicsResource ** pCudaTex = pResouce->getResPoint(); 
	checkCudaErrors(cudaGraphicsSubResourceGetMappedArray(&tmpcudaArray,*pCudaTex,0,0));
	if(worldPosRef_t == pResouce->getType())
	{
		checkCudaErrors(cudaBindTextureToArray(cuda_WorldPos_Tex,tmpcudaArray,channelDesc));
		cuda_WorldPos_Tex.filterMode = cudaFilterModeLinear;
	}
	else if(worldNormalRef_t == pResouce->getType())
	{
		checkCudaErrors(cudaBindTextureToArray(cuda_WorlNormal_Tex,tmpcudaArray,channelDesc));
		cuda_WorlNormal_Tex.filterMode = cudaFilterModePoint;
	}
	else if(reflecionRef_t ==  pResouce->getType())
	{
		checkCudaErrors(cudaBindTextureToArray(cuda_Reflect_Tex,tmpcudaArray,channelDesc));
		cuda_Reflect_Tex.filterMode = cudaFilterModePoint;
	}
	else if(finalEffect_t == pResouce->getType())
	{
		checkCudaErrors(cudaBindTextureToArray(cuda_RePro_Pixel,tmpcudaArray,channelDesc));
	}

}
extern "C" void cudaRelateArray(CudaPboResource * pResource)
{
	size_t numBytes;
	cudaGraphicsResource ** pCudaTex = pResource->getResPoint(); 
	/*
	moveVector_t,
	lastVector_t,
	*/
	if(float4_t == pResource->getType())
	{
		checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&cuda_PBO_Buffer,&numBytes,*pCudaTex));
		checkCudaErrors(cudaMemcpyToSymbol(d_cudaPboBuffer, &cuda_PBO_Buffer,sizeof(float4*)));
	}
	else if(float2_t == pResource->getType())
	{
		checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&cuda_LastVec_Buffer,&numBytes,*pCudaTex));
		checkCudaErrors(cudaMemcpyToSymbol(d_cudaLastBuffer, &cuda_LastVec_Buffer,sizeof(float2*)));

	}
	else if(test_t == pResource->getType())
	{
		checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&cuda_test_Buffer,&numBytes,*pCudaTex));
		checkCudaErrors(cudaMemcpyToSymbol(d_cudaTestBuffer, &cuda_test_Buffer,sizeof(float4*)));

	}
	else if(diff_normal_t == pResource->getType())
	{
		checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&cuda_diffNormBuffer,&numBytes,*pCudaTex));
		checkCudaErrors(cudaMemcpyToSymbol(d_cudaTestBuffer, &cuda_diffNormBuffer,sizeof(float4*)));

	}
}