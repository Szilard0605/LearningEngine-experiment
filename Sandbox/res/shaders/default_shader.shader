#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_texcoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
};

layout(location = 0) out FragmentData fragmentdata;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_position, 1.0);

	fragmentdata.Position = vec3(u_Transform * vec4(a_position, 1.0));
	fragmentdata.Normal =  mat3(transpose(inverse(u_Transform))) * a_normal;
	fragmentdata.TexCoords = a_texcoords;
	fragmentdata.Tangent = mat3(u_Transform) * a_tangent;
	fragmentdata.Bitangent = mat3(u_Transform) * a_bitangent;
	
    fragmentdata.Tangent =   normalize(vec3(u_Transform * vec4(a_tangent,   0.0)));
    fragmentdata.Bitangent = normalize(vec3(u_Transform * vec4(a_bitangent, 0.0)));
    fragmentdata.Normal =    normalize(vec3(u_Transform * vec4(a_normal,    0.0)));
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;

struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
};

layout(location = 0) in FragmentData fragmentdata;

uniform float u_AmbientStrength;
uniform vec4 u_AmbientColor;

uniform float u_DiffuseStrength;
uniform vec4 u_DiffuseColor;
uniform vec3 u_DiffusePosition;

uniform float u_SpecularStrength;
uniform vec3 u_ViewPos;

layout(binding = 0) uniform sampler2D u_Texture;
layout(binding = 1) uniform sampler2D u_NormalMap;

#define GAMMA 2.2

vec3 FinalGamma(vec3 color)
{
	return pow(color, vec3(1.0 / GAMMA));
}


void main()
{	

	vec4 tex = texture(u_Texture, fragmentdata.TexCoords);
	if (tex.a < 0.1f) {
		discard;
	}

	vec3 norm = normalize(fragmentdata.Normal);

	vec4 ambient = u_AmbientStrength * u_AmbientColor;
	
	vec3 lightDir = normalize(u_DiffusePosition - fragmentdata.Position);  	
	float diff = max(0.0, dot(norm, lightDir));
	vec4 diffuse = diff * u_DiffuseColor * u_DiffuseStrength;
	
	vec3 viewDir = normalize(u_ViewPos - fragmentdata.Position);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = u_SpecularStrength * spec * u_AmbientColor.xyz;  
	
    vec4 result = (ambient + diffuse + vec4(specular, 0.0));

    outColor = vec4(FinalGamma(result.xyz), 1.0f) * tex;

}