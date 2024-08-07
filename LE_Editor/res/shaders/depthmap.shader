#shader vertex
#version 450 core

layout (location = 0) in vec3 a_position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_position, 1.0);
}


#shader fragment
#version 450 core


void main()
{

}