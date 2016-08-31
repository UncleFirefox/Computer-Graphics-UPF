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

//Variable to pass to PS
varying vec3 light_position_ps;
varying vec3 eye_position_ps;
varying vec3 normal_ps;
varying vec3 point_position_ps;

varying vec3 light_ambient_ps;
varying vec3 material_ambient_ps;

varying vec3 light_diffuse_ps;
varying vec3 material_diffuse_ps;

varying vec3 light_specular_ps;
varying vec3 material_specular_ps;
varying float alpha_ps;

//Matrix and other stuff
uniform mat4 model;
uniform mat4 normal_model;
uniform mat4 mvp;

void main()
{	

	light_position_ps = light_position;
	eye_position_ps = eye_position;
	
	light_ambient_ps = light_ambient;
	material_ambient_ps = material_ambient;
	
	light_diffuse_ps = light_diffuse;
	material_diffuse_ps = material_diffuse;
	
	light_specular_ps = light_specular;
	material_specular_ps = material_specular;
	alpha_ps = alpha;
	
	//calcule the normal in world space (the normal_model is like the model but without traslation)
	normal_ps = (normal_model * vec4(gl_Normal,1.0) ).xyz;
	
	//calcule the vertex in world space (because the light position is in world space and the vertex comes in local space)
	point_position_ps = (model * gl_Vertex).xyz;
		
	//compute the position of the vertex in screen space using the MVP
	gl_Position = mvp * gl_Vertex;
}