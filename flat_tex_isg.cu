
#include <optix.h>
#include <optixu/optixu_math_namespace.h>

using namespace optix;

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

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );

rtTextureSampler<float4, 2> diffuse_texture;
rtDeclareVariable(float3,   texcoord, attribute texcoord, );
rtDeclareVariable(float3,   lightPos, , );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );
rtDeclareVariable(uint, max_depth, , );
rtDeclareVariable(uint, radiance_ray_type, , );
rtDeclareVariable(uint, shadow_ray_type, , );
rtDeclareVariable(float, scene_epsilon, , );
rtDeclareVariable(rtObject, reflectors, , );
rtDeclareVariable(int, id, ,);

RT_PROGRAM void closest_hit_radiance()
{
  prd_radiance.t_hit = t_hit;
   float3 hit_point = ray.origin + t_hit * ray.direction;
  float3 L =  normalize(lightPos-hit_point);
  float3 world_shade_normal = normalize(rtTransformNormal( RT_OBJECT_TO_WORLD, shading_normal));
  float kd=fmax(dot(world_shade_normal,L),0);
	 float ka=0.2;
  float3 color =make_float3((kd+ka)*tex2D(diffuse_texture, texcoord.x, texcoord.y));

  float fresnel = fresnel_schlick(dot(-ray.direction, shading_normal), 5.f, 0.9f);

  PerRayData_radiance refl_prd;
  refl_prd.importance = prd_radiance.importance * fresnel * optix::luminance(color);
  refl_prd.depth = prd_radiance.depth + 1;
  float3 result;
  // if(0)
 if(refl_prd.depth <= max_depth && refl_prd.importance > 0.05) 
  {

    optix::Ray refl_ray = optix::make_Ray(hit_point, reflect(ray.direction, shading_normal), 
                                          radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);
    rtTrace(reflectors, refl_ray, refl_prd);
     result = refl_prd.result * 0.2 + 0.8*color;
  } 
  else
  {
    result = make_float3(1.f,0.f,0.f);
  }
  prd_radiance.t_hit = t_hit;
  prd_radiance.objectId = id;
 
  prd_radiance.result= color ;//	prd_radiance.result = make_float3(1,0,0);

}
RT_PROGRAM void any_hit_shadow()
{
   prd_shadow.attenuation = make_float3(1,1,1);
   rtTerminateRay();
}
