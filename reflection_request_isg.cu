
#include <optix.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

rtBuffer<float4, 2>         reflection_buffer;
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

rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

struct PerRayData_radiance
{
  float3 result;
  float importance;
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
RT_PROGRAM void addition_request()
{
	int index = launch_index.y * PixelWidth  + launch_index.x;
	if(index >= PixelNum)
 			return;
 	uint x,y;
 //	uint PixPos = Pixels_Buffer[index];
	uint PixPos = Pixels_Buffer[index];
    x = PixPos%1024;
	y = PixPos/1024;
	//x =  launch_index.x;
    //y =  launch_index.y;
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
    if(1)
	{
		float3 V = normalize(ray_origin-eye_pos);
		float3 normal = make_float3(tex2D(normal_texture, x, y));
		float3 ray_direction = reflect(V, normal);
		float3 L = lightPos-ray_origin;
		float dist = sqrtf(dot(L,L));
		float3 ray_direction_s = L/dist;
		optix::Ray ray_s = optix::make_Ray(ray_origin, 
		ray_direction_s, 
		shadow_ray_type, 
		scene_epsilon, 
		dist);
		rtTrace(reflectors, ray_s, prd_s);
		float shadow = (prd_s.attenuation.x>0)?1:0;
		optix::Ray ray = optix::make_Ray(ray_origin, ray_direction, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
		rtTrace(reflectors, ray, prd);
		reflection_buffer[FinalPixelPos] = make_float4(prd.result,1);
		return;	 
	}
  }
 	
}
RT_PROGRAM void reflection_request()
{
	//return;
  float3 ray_origin = make_float3(tex2D(request_texture, launch_index.x, launch_index.y));
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
    if(0)
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
		reflection_buffer[launch_index] = make_float4(prd.result,r_dis);
		return;	 
	}
	float3 V = normalize(ray_origin-eye_pos);
    float3 normal = make_float3(tex2D(normal_texture, launch_index.x, launch_index.y));
	float3 ray_direction = normalize(reflect(V, normal));
	float3 xo, yo;
    createONB(ray_direction, xo, yo);
#define N 11
	float2 randomArray[N];
	float rx[N],ry[N];
	float3 glossy_direcion[N];
	 optix::Ray ray[N];
	 PerRayData_radiance prdArray[N];
	 float3 sumColor = make_float3(0,0,0);
	 float seedx = normal.x+ray_origin.y;
	 float seedy = normal.z+ray_origin.x;
	 float exponent = 3;
	 float bsdf_val[N];
	 float bsdf_pdf[N];
	 float costheta[N];
	for(int i =0;i<N;i++)
	{
		prdArray[i] = prd;
		randomArray[i].x =  random(make_float2(i*1.0/N*seedx,(i+0.5)/N)*seedy);
		randomArray[i].y =  random(make_float2((i+0.5)/N*seedy,i*1.0/N*seedx));

		//ConcentricSampleDisk(randomArray[2*i],randomArray[2*i+1],&rx[i],&ry[i]);
		//rx[i] = randomArray[2*i];
		//ry[i] = randomArray[2*i+1];
		/*float angle = -5.5;
		rx[i] = sin(angle);
		ry[i] = cos(angle);*/
		glossy_direcion[i] = sample_phong_lobe( randomArray[i], 50, xo, yo, ray_direction, bsdf_pdf[i], bsdf_val[i] );
		costheta[i] = dot(glossy_direcion[i], normal);
		ray[i] = optix::make_Ray(ray_origin, glossy_direcion[i], radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
	}
	

	rtTrace(reflectors, ray[0], prdArray[0]);
	rtTrace(reflectors, ray[1], prdArray[1]);
 	rtTrace(reflectors, ray[2], prdArray[2]);
 	rtTrace(reflectors, ray[3], prdArray[3]);
 	rtTrace(reflectors, ray[4], prdArray[4]);
	rtTrace(reflectors, ray[5], prdArray[5]);
 	rtTrace(reflectors, ray[6], prdArray[6]);
 	rtTrace(reflectors, ray[7], prdArray[7]);
 	rtTrace(reflectors, ray[8], prdArray[8]);
 	rtTrace(reflectors, ray[9], prdArray[9]);
	rtTrace(reflectors, ray[10], prdArray[10]);
 	/*rtTrace(reflectors, ray[11], prdArray[11]);
 	rtTrace(reflectors, ray[12], prdArray[12]);
 	rtTrace(reflectors, ray[13], prdArray[13]);
 	/*rtTrace(reflectors, ray[14], prdArray[14]);
	rtTrace(reflectors, ray[15], prdArray[15]);
 	/*rtTrace(reflectors, ray[16], prdArray[16]);
 	rtTrace(reflectors, ray[17], prdArray[17]);
 	rtTrace(reflectors, ray[18], prdArray[18]);
 	rtTrace(reflectors, ray[19], prdArray[19]);
	rtTrace(reflectors, ray[20], prdArray[20]);
 	rtTrace(reflectors, ray[21], prdArray[21]);
 	rtTrace(reflectors, ray[22], prdArray[22]);
 	rtTrace(reflectors, ray[23], prdArray[23]);
 	rtTrace(reflectors, ray[24], prdArray[24]);
	rtTrace(reflectors, ray[25], prdArray[25]);
 	rtTrace(reflectors, ray[26], prdArray[26]);
 	rtTrace(reflectors, ray[27], prdArray[27]);
 	rtTrace(reflectors, ray[28], prdArray[28]);
 	rtTrace(reflectors, ray[29], prdArray[29]);
 
	/*rtTrace(reflectors, ray[30], prdArray[30]);
 	rtTrace(reflectors, ray[31], prdArray[31]);
 	rtTrace(reflectors, ray[32], prdArray[32]);
 	rtTrace(reflectors, ray[33], prdArray[33]);
 	rtTrace(reflectors, ray[34], prdArray[34]);
	rtTrace(reflectors, ray[35], prdArray[35]);
 	rtTrace(reflectors, ray[36], prdArray[36]);
 	rtTrace(reflectors, ray[37], prdArray[37]);
 	rtTrace(reflectors, ray[38], prdArray[38]);
 	rtTrace(reflectors, ray[39], prdArray[39]);
 */
 

 	float depthSum = 0;
	float depthCount = 0;
	float r ;
	float3 color ;
	sumColor = make_float3(0,0,0);
	for(int i=0;i<N;i++)
	{
	//if(prdArray[i].t_hit<8)
		float3 color = prdArray[i].result*costheta[i]*bsdf_val[i] /bsdf_pdf[i];
		sumColor +=color;
		float len = prdArray[i].t_hit;
		depthSum+=len; 

	}
	float avgDepth;
	color = (sumColor)/N; 
	avgDepth = depthSum/N;
	/*color.x = rx[0];
	color.y = ry[0];
	color.z = 0;*/
	//color = normalize(rx[0]*a*xo+ry[0]*a*yo);
	 reflection_buffer[launch_index] = make_float4(color, avgDepth);
	
/*#define  SampleCount  4
	float3 sumColor = make_float3(0,0,0);
	int count = 1;
	//int i=1;
	PerRayData_radiance prdArray[4];
	optix::Ray rayArray[4];
	for(int i =0;i<SampleCount;i++)
	{
		prdArray[i] = prd;
		float seed1= i*1.0/(SampleCount+1),seed2 = (i+1)*1.0/(SampleCount+1);
		float random1 = random(make_float2(seed1,seed2));
		float random2 = random(make_float2(seed2,seed1));
		float r1x,r1y;
		ConcentricSampleDisk(random1,random2,&r1x,&r1y);
		float3 glossy_direcion1 = ray_direction+r1x*a*xo+r1y*a*yo;
		rayArray[i] = optix::make_Ray(ray_origin, glossy_direcion1, radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
		//rtTrace(reflectors, ray2, prdArray[i]);
		//sumColor+=prdArray[i].result;
	}
	rtTrace(reflectors, rayArray[0], prdArray[0]);
	rtTrace(reflectors, rayArray[1], prdArray[1]);
	rtTrace(reflectors, rayArray[2], prdArray[2]);
	rtTrace(reflectors, rayArray[3], prdArray[3]);
	rtTrace(reflectors, ray_s, prd_s);
	float shadow = (prd_s.attenuation.x>0)?1:0;
	float3 color = prdArray[0].result;
    reflection_buffer[launch_index] = make_float4(color, shadow);
*/	
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