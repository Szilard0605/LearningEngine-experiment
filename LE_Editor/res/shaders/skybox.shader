#shader vertex
#version 450 core

layout (location = 0) in vec3 a_position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 TexCoords;

void main()
{
	TexCoords = vec3(a_position.x, a_position.y, -a_position.z);
	vec4 pos = u_Projection *u_View  * vec4(a_position, 1.0);
	gl_Position = pos.xyww;
};

#shader fragment
#version 450 core

in vec3 TexCoords;

layout(location = 0) out vec4 o_color;
layout(binding = 0) uniform sampler2D u_Texture;

uniform samplerCube skybox;

void main()
{
	o_color = texture(skybox, TexCoords);
}