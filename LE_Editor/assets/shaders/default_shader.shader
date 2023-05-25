#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

layout(location = 0) out FragmentData fragmentdata;



void main()
{

	//vec4 WorldPosition = u_Transform * vec4(a_position, 1.0);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_position, 1.0);
	//gl_Position = vec4(a_position, 1.0);

	fragmentdata.Position = vec3(u_Transform * vec4(a_position, 0.0));
	fragmentdata.Normal = mat3(transpose(inverse(u_Transform))) * a_normal;
	fragmentdata.TexCoords = a_texcoords;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};


layout(location = 0) in FragmentData fragmentdata;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, fragmentdata.TexCoords);
}