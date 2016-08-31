//Normal Shader
//********************
//This shader puts the normal as the color of every pixel, this is posible because normals are
//in the range from -1 to 1 and colors are between 0 a 1, so the positive parts will be colored
//according to the direction the object is pointing. If the vertex is pointing up it will look
//green (0,1,0), etc.

//this will store the color for the pixel shader
varying vec3 pixel_color;

uniform mat4 normal_model;
uniform mat4 mvp;

void main()
{	
	vec3 normal =  gl_Normal.xyz;
	
	//calcule the normal in world space (the normal_model is like the Model but without traslation)
	//if you comment the next line it will show the normals in local space instead of world space
	normal = (normal_model * vec4(normal,1.0) ).xyz;
	
	//store the color in the varying var to use it from the pixel shader
	pixel_color = normal;
	
	//calcule the position of the vertex using the matrices
	gl_Position = mvp * gl_Vertex;
}