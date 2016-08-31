//global info about the light
uniform vec3 light_position;
uniform vec3 light_color;

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
	
	//calcule the NdotL
	float NdotL = dot (light_vector, normal);
	
	//avoid negative values (it doesnt makes sense to have negative light)
	if (NdotL < 0.0) NdotL = 0.0;
	
	//store the resulting color in the varying var to use it from the pixel shader
	pixel_color = NdotL * light_color;
	
	//compute the position of the vertex in screen space using the MVP
	gl_Position = mvp * gl_Vertex;
}