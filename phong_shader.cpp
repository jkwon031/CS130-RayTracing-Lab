#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    Light* current_light;
    vec3 t;
    //vec3 view;
    vec3 reflection;
    //TODO; //determine the color
    vec3 ambient = world.ambient_color * world.ambient_intensity * color_ambient;
    vec3 diffuse;
    vec3 specular;
    Hit intersect;

    for(unsigned int i = 0; i < world.lights.size(); i++){
	current_light = world.lights[i];
	
	t = current_light->position - intersection_point;
	Ray shadow_ray(intersection_point, t.normalized());
	//view = ray.endpoint - intersection_point;
	
	intersect = world.Closest_Intersection(shadow_ray);
	//diffuse += color_diffuse * current_light->Emitted_Light(-t) * std::max(dot(normal.normalized(),shadow_ray.direction), 0.0);

	reflection = (t - 2 * dot(t, normal.normalized()) * normal).normalized();
		
	if(!world.enable_shadows || (world.enable_shadows && (!intersect.object || intersect.dist >= t.magnitude()))){
		
		
		specular += color_specular * current_light->Emitted_Light(-t) * pow(std::max(dot(ray.direction, reflection), 0.0), specular_power);
		diffuse += color_diffuse * current_light->Emitted_Light(-t) * std::max(dot(normal.normalized(),shadow_ray.direction), 0.0);	
    	}
    }
    color = ambient + diffuse + specular;
    return color;
}
