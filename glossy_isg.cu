
#include <optix.h>
#include <optixu/optixu_math_namespace.h>
#include "commonStructs.h"
#include "helpers.h"

using namespace optix;

rtDeclareVariable(uint, radiance_ray_type, , );
rtDeclareVariable(uint, shadow_ray_type, , );
rtDeclareVariable(float, scene_epsilon, , );
rtDeclareVariable(rtObject, reflectors, , );
rtDeclareVariable(uint, max_depth, , );	

rtBuffer<BasicLight> lights;

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );

struct PerRayData_radiance
{
  
  float3 result;
  float  importance;
  int    objectId;
  int depth;
  float t_hit;
};

struct PerRayData_shadow
{
  float3 attenuation;
};

rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );
rtDeclareVariable(PerRayData_shadow,   prd_shadow,   rtPayload, );
rtDeclareVariable(float3,   lightPos, , );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );
rtDeclareVariable(int3, index_color, attribute index_color, ); 

rtDeclareVariable(float3,diffuse_Color,,);
rtDeclareVariable(int, id, ,);
RT_PROGRAM void closest_hit_radiance()
{
  prd_radiance.t_hit = t_hit;

  float3 hit_point = ray.origin + t_hit * ray.direction;
  
   float3 L =  normalize(lightPos-hit_point);
  float3 world_shade_normal = normalize(rtTransformNormal( RT_OBJECT_TO_WORLD, shading_normal));
  float kd=fmax(dot(world_shade_normal,L),0);
	 float ka=0.2;
  float3 color = (kd+ka)*make_float3(diffuse_Color.x, diffuse_Color.y, diffuse_Color.z);
	
  float fresnel = fresnel_schlick(dot(-ray.direction, shading_normal), 5.f, 0.9f);

  PerRayData_radiance refl_prd;
  refl_prd.importance = prd_radiance.importance * fresnel * optix::luminance(color);
  refl_prd.depth = prd_radiance.depth + 1;
  refl_prd.objectId = id+1;
  float3 result;
  if(fabs(diffuse_Color.z- 0.66)<0.001)
  {
	   prd_radiance.result = color;
	   return;
  }
 // if(0)
  if((refl_prd.depth <= max_depth && refl_prd.importance > 0.05) )
  {

    optix::Ray refl_ray = optix::make_Ray(hit_point, reflect(ray.direction, shading_normal), 
                                          radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
    rtTrace(reflectors, refl_ray, refl_prd);
    result = refl_prd.result * 0.20 + 0.80*color;
  } else
  {
    result = color;
  }
  prd_radiance.t_hit = t_hit;
   prd_radiance.objectId = id;
 // color = make_float3(index_color.x,index_color.y,index_color.z);
  prd_radiance.result = color;
}

RT_PROGRAM void any_hit_shadow()
{
   prd_shadow.attenuation = make_float3(1,1,1);
  rtTerminateRay();
}
