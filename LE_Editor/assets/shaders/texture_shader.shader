#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;
layout(location = 3) in float a_texindex;

uniform mat4 u_ViewProjection;



struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat float v_TexIndex;


void main()
{
	Output.Color = a_color;
	Output.TexCoord = a_texcoord;
	v_TexIndex = a_texindex;

	
	
	gl_Position = u_ViewProjection * vec4(a_position, 1.0);
};

#shader fragment
#version 450 core


layout(location = 0) out vec4 o_color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};


layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat float v_TexIndex;


layout(binding = 0) uniform sampler2D u_Textures[32];


void main()
{
	vec4 texColor = Input.Color;

	switch (int(v_TexIndex))
	{															  
		case  0: texColor *= texture(u_Textures[0], Input.TexCoord); break;
		case  1: texColor *= texture(u_Textures[1], Input.TexCoord); break;
		case  2: texColor *= texture(u_Textures[2], Input.TexCoord); break;
		case  3: texColor *= texture(u_Textures[3], Input.TexCoord); break;
		case  4: texColor *= texture(u_Textures[4], Input.TexCoord); break;
		case  5: texColor *= texture(u_Textures[5], Input.TexCoord); break;
		case  6: texColor *= texture(u_Textures[6], Input.TexCoord); break;
		case  7: texColor *= texture(u_Textures[7], Input.TexCoord); break;
		case  8: texColor *= texture(u_Textures[8], Input.TexCoord); break;
		case  9: texColor *= texture(u_Textures[9], Input.TexCoord); break;
		case 10: texColor *= texture(u_Textures[10], Input.TexCoord); break;
		case 11: texColor *= texture(u_Textures[11], Input.TexCoord); break;
		case 12: texColor *= texture(u_Textures[12], Input.TexCoord); break;
		case 13: texColor *= texture(u_Textures[13], Input.TexCoord); break;
		case 14: texColor *= texture(u_Textures[14], Input.TexCoord); break;
		case 15: texColor *= texture(u_Textures[15], Input.TexCoord); break;
		case 16: texColor *= texture(u_Textures[16], Input.TexCoord); break;
		case 17: texColor *= texture(u_Textures[17], Input.TexCoord); break;
		case 18: texColor *= texture(u_Textures[18], Input.TexCoord); break;
		case 19: texColor *= texture(u_Textures[19], Input.TexCoord); break;
		case 20: texColor *= texture(u_Textures[20], Input.TexCoord); break;
		case 21: texColor *= texture(u_Textures[21], Input.TexCoord); break;
		case 22: texColor *= texture(u_Textures[22], Input.TexCoord); break;
		case 23: texColor *= texture(u_Textures[23], Input.TexCoord); break;
		case 24: texColor *= texture(u_Textures[24], Input.TexCoord); break;
		case 25: texColor *= texture(u_Textures[25], Input.TexCoord); break;
		case 26: texColor *= texture(u_Textures[26], Input.TexCoord); break;
		case 27: texColor *= texture(u_Textures[27], Input.TexCoord); break;
		case 28: texColor *= texture(u_Textures[28], Input.TexCoord); break;
		case 29: texColor *= texture(u_Textures[29], Input.TexCoord); break;
		case 30: texColor *= texture(u_Textures[30], Input.TexCoord); break;
		case 31: texColor *= texture(u_Textures[31], Input.TexCoord); break;
	}															  

	if (texColor.a == 0.0)
		discard;
	
	o_color = texColor;
}