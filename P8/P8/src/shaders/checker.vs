uniform mat4 mvp;
varying vec3 pixel_position;
varying vec3 pixel_color;

void main()
{	
	//send the pos of the vertex to the pixel, because we dont have it in the pixel
	pixel_position = gl_Vertex.xyz; 
	
	//send the vertex color to the pixel shader too
	pixel_color = gl_Color.xyz;
	
	//return the final vertex position 
	gl_Position = mvp * gl_Vertex;
}