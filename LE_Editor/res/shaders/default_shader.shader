#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_texcoords;
layout(location = 5) in int  a_entity;

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
layout(location = 14) out flat int o_Entity;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_position, 1.0);

    fragmentdata.Position = vec3(u_Transform * vec4(a_position, 1.0));
    fragmentdata.Normal = mat3(transpose(inverse(u_Transform))) * a_normal;
	fragmentdata.TexCoords = a_texcoords;
	
    fragmentdata.Tangent = normalize(vec3(u_Transform * vec4(a_tangent, 0.0)));
    fragmentdata.Bitangent = normalize(vec3(u_Transform * vec4(a_bitangent, 0.0)));
    fragmentdata.Normal = normalize(vec3(u_Transform * vec4(a_normal, 0.0)));
    
    o_Entity = a_entity;
	
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;
layout(location = 1) out int o_entity;

struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
};

layout(location = 0) in FragmentData fragmentdata;
layout(location = 14) in flat int in_Entity;


layout(binding = 0) uniform sampler2D u_Texture;

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

vec3 CalculatePointLight(Light light, vec3 Position, vec3 Normal)
{
    vec3 lightDir = normalize(light.Position.xyz - Position);
    float distance = length(light.Position.xyz - Position);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    vec3 diffuse = light.Color.rgb * max(dot(Normal, lightDir), 0.0);
    
    return diffuse * attenuation * light.Color.a;
}


vec3 CalculateDirectionalLight(Light light, vec3 Normal)
{
    vec3 lightDir = normalize(-light.Direction.xyz);
    vec3 diffuse = light.Color.rgb * max(dot(Normal, lightDir), 0.0);
    
    return diffuse * light.Color.a;
}

void main()
{
    o_entity = in_Entity;
	
	vec4 tex = texture(u_Texture, fragmentdata.TexCoords);
	
	if (tex.a < 0.1f) {
		discard;
	}

    vec3 totalDiffuse = vec3(0.0);
    
    for (int i = 0; i < u_NumLights; i++)
    {
        if (u_Lights[i].Position.a < 1.0)
        {
            totalDiffuse += CalculatePointLight(u_Lights[i], fragmentdata.Position, fragmentdata.Normal);
        }
        else
        {
            totalDiffuse += CalculateDirectionalLight(u_Lights[i], fragmentdata.Normal);
        }
    }
    
    float ambientIntensity = u_AmbientLight.a;
    vec3 ambientLight = u_AmbientLight.rgb * ambientIntensity;
    
    outColor = vec4(FinalGamma(tex.xyz * ambientLight + totalDiffuse), 1.0);

}