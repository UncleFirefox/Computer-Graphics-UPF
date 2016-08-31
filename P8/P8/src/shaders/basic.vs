varying vec3 pixel_color;
uniform mat4 mvp;

void main()
{	
	//send the vertex color to the pixel shader
	pixel_color = gl_Color.xyz;
	
	//compute the vertex position in screen space
	gl_Position = mvp * gl_Vertex;
}