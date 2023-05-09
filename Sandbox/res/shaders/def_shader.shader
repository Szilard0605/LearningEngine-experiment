#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
//layout(location = 2) in mat4 a_transform;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput Output;

void main()
{
	Output.Color = a_color;

	gl_Position = u_ViewProjection * vec4(a_position, 1.0);
};

#shader fragment
#version 450 core

/*out vec4 color;
uniform vec4 ncolor; // we set this variable in the OpenGL code.*/

layout(location = 0) out vec4 o_color;

struct VertexOutput
{
	vec4 color;
};

layout(location = 0) in VertexOutput Input;

void main()
{
	o_color = Input.color;
}