//global info about the light
uniform vec3 eye_position;
uniform vec3 light_position;

//Light properties
uniform vec3 light_ambient;
uniform vec3 material_ambient;

uniform vec3 light_diffuse;
uniform vec3 material_diffuse;

uniform vec3 light_specular;
uniform vec3 material_specular;
uniform float alpha;

//this will store the color for the pixel shader
varying vec3 pixel_color;

uniform mat4 model;
uniform mat4 normal_model;
uniform mat4 mvp;

void main()
{	
	//calcule the normal in world space (the normal_model is like the model but without traslation)
	vec3 normal = (normal_model * vec4(gl_Normal,1.0) ).xyz;
	
	//calcule the vertex in world space (because the light position is in world space and the vertex comes in local space)
	vec3 vertex_pos = (model * gl_Vertex).xyz;

	//calcule the light_vector
	vec3 light_vector = light_position - vertex_pos;
	
	//normalize the light vector
	light_vector = normalize(light_vector);
	
	//calcule the NdotL (factor difuso)
	//avoid negative values (it doesnt makes sense to have negative light)
	float NdotL = clamp (dot (light_vector, normal), 0.0 , 1.0);
	
	//Calculos para el specular
	vec3 reflected_vector = ((2 * dot (normal, light_vector)) * normal) - light_vector;
	reflected_vector = normalize(reflected_vector);
	
	vec3 vision_vector = eye_position - vertex_pos;
	vision_vector = normalize(vision_vector);
	
	float RdotV = clamp (dot (reflected_vector, vision_vector), 0.0, 1.0);
	
	//store the resulting color in the varying var to use it from the pixel shader
	pixel_color = (light_ambient * material_ambient)  + (NdotL * light_diffuse * material_diffuse) + pow(RdotV, alpha) * light_specular * material_specular; //(factor difuso por la luz difusa)
	
	//compute the position of the vertex in screen space using the MVP
	gl_Position = mvp * gl_Vertex;
}