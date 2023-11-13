#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_texcoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


uniform float u_Test;

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
    fragmentdata.Normal = mat3(transpose(inverse(u_Transform))) * a_normal;
	fragmentdata.TexCoords = a_texcoords;
	
    fragmentdata.Tangent = normalize(vec3(u_Transform * vec4(a_tangent, 0.0)));
    fragmentdata.Bitangent = normalize(vec3(u_Transform * vec4(a_bitangent, 0.0)));
    fragmentdata.Normal = normalize(vec3(u_Transform * vec4(a_normal, 0.0)));
    
	
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


layout(binding = 0) uniform sampler2D u_Texture;
layout(binding = 1) uniform sampler2D u_NormalMap;

#define GAMMA 2.2

#define MAX_LIGHTS 100

struct Light
{
    vec4 Color;
    vec4 Position;
    vec4 Direction;
};


//uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

layout(std140, binding = 0) uniform RenderData
{
    vec4 u_AmbientLight;
    int u_NumLights;
};

layout(std140, binding = 1) uniform LightBuffer
{
    Light u_Lights[MAX_LIGHTS];
};



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


    for (int i = 0; i < u_NumLights; i ++)
    {
        if (u_Lights[i].Position.a < 1.0)
        {
            vec3 lightDir = normalize(u_Lights[i].Position.xyz - fragmentdata.Position.xyz);
            float distance = length(u_Lights[i].Position.xyz - fragmentdata.Position.xyz);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
            vec3 diffuse = u_Lights[i].Color.xyz * max(dot(fragmentdata.Normal, lightDir), 0.0);
            totalDiffuse += diffuse * attenuation * u_Lights[i].Color.a;
        }
        else
        {
            vec3 lightDir = normalize(-u_Lights[i].Direction.xyz);
            vec3 diffuse = u_Lights[i].Color.xyz * max(dot(fragmentdata.Normal, lightDir), 0.0);
            totalDiffuse += diffuse * u_Lights[i].Color.a;
        }
    }
    
    float ambientIntensity = u_AmbientLight.a;
    vec4 ambientLight = vec4(u_AmbientLight.xyz, 1.0f) * ambientIntensity;
    
    outColor = glm::vec4(FinalGamma(tex.xyz * ambientLight + totalDiffuse), 1.0);

}