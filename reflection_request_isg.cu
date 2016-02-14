
#include <optix.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtBuffer<float4, 2>         reflection_buffer;
rtBuffer<float4, 2>         addition_buffer;;
rtBuffer<uint,1>          Pixels_Buffer;
//rtBuffer<float4,2>          LastReflection_buffer;

rtTextureSampler<float4, 2> normal_texture;
rtTextureSampler<float4, 2> request_texture;
rtTextureSampler<float4,2> Last_reflection;

rtDeclareVariable(uint, radiance_ray_type, , );
rtDeclareVariable(uint, shadow_ray_type, , );
rtDeclareVariable(float, scene_epsilon, , );
rtDeclareVariable(rtObject, reflectors, , );
rtDeclareVariable(float3, eye_pos, , );
rtDeclareVariable(float3,   lightPos, , );

rtDeclareVariable(int,   FrameCount, , );

rtDeclareVariable(int,   PixelNum, , );
rtDeclareVariable(int,   PixelWidth, , );
rtDeclareVariable(int,   hasGlossy, , );


rtDeclareVariable(int2,   rasterSize, , );

rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );


rtDeclareVariable(uint, launch_index1D, rtLaunchIndex, );
struct PerRayData_radiance
{
 
  float3 result;
  float  importance;
  int   objectId;
  int depth;
  float t_hit;
};
struct PerRayData_shadow
{
	float3 attenuation;
};

rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );



__device__ __host__ float random( float2 p )
{
   // We need irrationals for pseudo randomness.
   // Most (all?) known transcendental numbers will (generally) work.
   const float2 r = make_float2(
  23.1406926327792690,  // e^pi (Gelfond's constant)
   2.6651441426902251); // 2^sqrt(2) (Gelfond¨CSchneider constant)
   float temp =  cos( fmod( 123456789., 1e-7 + 256. * dot(p,r) ) ) ;
   return temp - floor(temp);
}

__device__ __host__ void ConcentricSampleDisk(float u1, float u2, float *dx,float*dy)
{
    float r, theta;
    // Map uniform random numbers to $[-1,1]^2$
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;
	
    // Map square to $(r,\theta)$

    // Handle degeneracy at the origin
    if (sx == 0.0 && sy == 0.0) {
        *dx = 0.0;
        *dy = 0.0;
        return;
    }
    if (sx >= -sy) {
        if (sx > sy) {
            // Handle first region of disk
            r = sx;
            if (sy > 0.0) theta = sy/r;
            else          theta = 8.0f + sy/r;
        }
        else {
            // Handle second region of disk
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else {
        if (sx <= sy) {
            // Handle third region of disk
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else {
            // Handle fourth region of disk
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }
    theta *= M_PI / 4.f;
     *dx = r * cosf(theta);
     *dy = r * sinf(theta);

}
static
__host__ __device__ __inline__ optix::float3 sample_phong_lobe( const optix::float2 &sample, float exponent, 
                                                                const optix::float3 &U, const optix::float3 &V, const optix::float3 &W, 
                                                                float &pdf, float &bdf_val )
{
  const float cos_theta = powf(sample.y, 1.0f/(exponent+1.0f) );

  const float phi = sample.x * 2.0f * M_PIf;
  const float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);
  
  const float x = cosf(phi)*sin_theta;
  const float y = sinf(phi)*sin_theta;
  const float z = cos_theta;

  const float powered_cos = powf( cos_theta, exponent );
  pdf = (exponent+1.0f) / (2.0f*M_PIf) * powered_cos;
  bdf_val = (exponent+2.0f) / (2.0f*M_PIf) * powered_cos;  

  return x*U + y*V + z*W;
}
static
__device__ __inline__ void createONB( const optix::float3& n,
                                      optix::float3& U,
                                      optix::float3& V)
{
  using namespace optix;

  U = cross( n, make_float3( 0.0f, 1.0f, 0.0f ) );
  if ( dot(U, U) < 1.e-3f )
    U = cross( n, make_float3( 1.0f, 0.0f, 0.0f ) );
  U = normalize( U );
  V = cross( n, U );
}
#define N 1024
RT_PROGRAM void addition_request()
{
	int index =launch_index1D;
	if(index >= PixelNum)
 			return;
 	uint x,y;
	uint PixPos = Pixels_Buffer[index];
    x = PixPos%rasterSize.x;
	y = PixPos/rasterSize.x;
	uint2 FinalPixelPos = make_uint2(x,y);
	 float3 ray_origin = make_float3(tex2D(request_texture, x, y));
	float reflectValue = tex2D(request_texture, x, y).w;
  PerRayData_radiance prd;
  PerRayData_shadow prd_s;
  prd_s.attenuation = make_float3(0);
  prd.result = make_float3(0);
  prd.importance = 1.f;
  prd.depth = 0;
  prd.t_hit = -1.f;
	
  // PerRayData_radiance prd2 = prd;
 //    PerRayData_radiance prd3 = prd;
 
 // PerRayData_radiance prd4 = prd;
 //	   PerRayData_radiance prd5 = prd;
 
  if( !isnan(ray_origin.x) ) 
  {
    if(!hasGlossy)
	{
		
		float3 V = normalize(ray_origin-eye_pos);
		float3 normal = make_float3(tex2D(normal_texture, x, y));
		float3 ray_direction = reflect(V, normal);
		float shadow = (prd_s.attenuation.x>0)?1:0;
		optix::Ray ray = optix::make_Ray(ray_origin, ray_direction, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
		rtTrace(reflectors, ray, prd);
		reflection_buffer[FinalPixelPos] = make_float4(prd.result,1);
		return;	 
	}
	float3 V = normalize(ray_origin-eye_pos);
    float3 normal = make_float3(tex2D(normal_texture, x, y));
	float3 ray_direction = normalize(reflect(V, normal));
	float3 xo, yo;
    createONB(ray_direction, xo, yo);

	float2 randomArray;
	float3 glossy_direcion;
	 optix::Ray ray;
	 PerRayData_radiance prdArray;
	 float3 sumColor = make_float3(0,0,0);
	 float seedx = normal.x+ray_origin.y;
	 float seedy = normal.z+ray_origin.x;
	 float exponent = 30;
	 float bsdf_val,bsdf_pdf,costheta; 
	 float depthSum = 0;
	 float3 color ;
	 int usefulSample = 0;
	for(int i =0;i<N;i++)
	{
			prdArray = prd;
			randomArray.x =  random(make_float2(i*1.0/N*seedx,(i+0.5)/N)*seedy);
			randomArray.y =  random(make_float2((i+0.5)/N*seedy,i*1.0/N*seedx));
			glossy_direcion = sample_phong_lobe( randomArray, exponent, xo, yo, ray_direction, bsdf_pdf, bsdf_val );
			costheta = dot(glossy_direcion, normal);
			if(bsdf_pdf > 0.0f&&costheta>0)
			{
				ray = optix::make_Ray(ray_origin, glossy_direcion, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
				rtTrace(reflectors, ray, prdArray);
				sumColor += prdArray.result*costheta*bsdf_val /bsdf_pdf;;
				depthSum += prdArray.t_hit*costheta*bsdf_val /bsdf_pdf;
				usefulSample++;
			}
	}
	
	
	float avgDepth;
	color = (sumColor)/usefulSample; 
	avgDepth = depthSum/usefulSample;
	reflection_buffer[FinalPixelPos] = make_float4(color, avgDepth);
  }
}
RT_PROGRAM void reflection_request()
{
	//return;
  float3 ray_origin = make_float3(tex2D(request_texture, launch_index.x, launch_index.y));
  //if(launch_index.x!=91||launch_index.y!=623)
	//  return;
 // rtPrintf("x,y %d,%d\n",launch_index.x, launch_index.y);
 /* if(launch_index.x<=91||launch_index.x>=94)
	  return;
  if(launch_index.y<=91||launch_index.y>=94)
	  return;*/
  float reflectValue = tex2D(request_texture, launch_index.x, launch_index.y).w;
  PerRayData_radiance prd;
  PerRayData_shadow prd_s;
  prd_s.attenuation = make_float3(0);
  prd.result = make_float3(0);
  prd.importance = 1.f;
  prd.depth = 0;
  prd.t_hit = -1.f;
	
  // PerRayData_radiance prd2 = prd;
 //    PerRayData_radiance prd3 = prd;
 
 // PerRayData_radiance prd4 = prd;
 //	   PerRayData_radiance prd5 = prd;
 
  if( !isnan(ray_origin.x) ) 
  {
    if(!hasGlossy)
	{

		float3 V = normalize(ray_origin-eye_pos);
		float3 normal = make_float3(tex2D(normal_texture, launch_index.x, launch_index.y));
   
		float3 ray_direction = normalize(reflect(V, normal));

		/*float3 L = lightPos-ray_origin;
		float dist = sqrtf(dot(L,L));
		float3 ray_direction_s = L/dist;
		optix::Ray ray_s = optix::make_Ray(ray_origin, 
		ray_direction_s, 
		shadow_ray_type, 
		scene_epsilon, 
		dist);
		rtTrace(reflectors, ray_s, prd_s);
		float shadow = (prd_s.attenuation.x>0)?1:0;*/
		optix::Ray ray = optix::make_Ray(ray_origin, ray_direction, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
		rtTrace(reflectors, ray, prd);
	//shadow = 0;
		float r_dis = prd.t_hit;
		float3 reflectPos = ray_origin+ray_direction*r_dis;
	/*rtPrintf("eye_pos:(%f,%f£¬%f)\n",eye_pos.x,eye_pos.y,eye_pos.z);
		rtPrintf("wordldPos:(%f,%f£¬%f)\n",ray_origin.x,ray_origin.y,ray_origin.z);
		rtPrintf("reflectPos:(%f,%f£¬%f)\n",reflectPos.x,reflectPos.y,reflectPos.z);*/
		reflection_buffer[launch_index] = make_float4(prd.result,r_dis);
		addition_buffer[launch_index] = make_float4(prd.objectId,0,0,1);
		//rtPrintf("object id:%d",prd.objectId);
		return;	 
	}
	float3 V = normalize(ray_origin-eye_pos);
    float3 normal = make_float3(tex2D(normal_texture, launch_index.x, launch_index.y));
	float3 ray_direction = normalize(reflect(V, normal));
	float3 xo, yo;
    createONB(ray_direction, xo, yo);

	float2 randomArray;
	float3 glossy_direcion;
	 optix::Ray ray;
	 PerRayData_radiance prdArray;
	 float3 sumColor = make_float3(0,0,0);
	 float seedx = normal.x+ray_origin.y;
	 float seedy = normal.z+ray_origin.x;
	 float exponent = 30;
	 float bsdf_val,bsdf_pdf,costheta; 
	 float depthSum = 0;
	 float3 color ;
	 int usefulSample = 0;
	for(int i =0;i<N;i++)
	{
			prdArray = prd;
			randomArray.x =  random(make_float2(i*1.0/N*seedx,(i+0.5)/N)*seedy);
			randomArray.y =  random(make_float2((i+0.5)/N*seedy,i*1.0/N*seedx));
			glossy_direcion = sample_phong_lobe( randomArray, exponent, xo, yo, ray_direction, bsdf_pdf, bsdf_val );
			costheta = dot(glossy_direcion, normal);
			if(bsdf_pdf > 0.0f&&costheta>0)
			{
				ray = optix::make_Ray(ray_origin, glossy_direcion, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
				rtTrace(reflectors, ray, prdArray);
				sumColor += prdArray.result*costheta*bsdf_val /bsdf_pdf;;
				depthSum += prdArray.t_hit*costheta*bsdf_val /bsdf_pdf;
				usefulSample++;
			}
	}
	
	
	float avgDepth;
	color = (sumColor)/usefulSample; 
	avgDepth = depthSum/usefulSample;
	

	reflection_buffer[launch_index] = make_float4(color, avgDepth);
  }
}
RT_PROGRAM void reflection_exception()
{
  reflection_buffer[launch_index] = make_float4(1.f,0.f,0.f,-1.f);
}

RT_PROGRAM void reflection_miss()
{
  prd_radiance.t_hit = RT_DEFAULT_MAX;
  prd_radiance.result = make_float3(1.f, 1.f, 1.f);
}