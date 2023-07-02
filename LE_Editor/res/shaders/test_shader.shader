#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uViewProj;

void main()
{
	gl_Position = vec4(position, 1.0);
};

#shader fragment
#version 330 core

out vec4 color;
uniform vec4 ncolor; // we set this variable in the OpenGL code.

void main()
{
	color = ncolor;
}