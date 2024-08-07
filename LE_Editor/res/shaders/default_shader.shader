#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_texcoords;
layout(location = 5) in int  a_entity;

uniform mat4 u_ViewProjection;
uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Transform;

struct FragmentData
{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
    vec4 LightSpaceFragPos;
};

layout(location = 0) out FragmentData fragmentdata;
layout(location = 14) out flat int o_Entity;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_position, 1.0);

    fragmentdata.Position  = vec3(u_Transform * vec4(a_position, 1.0));
	fragmentdata.TexCoords = a_texcoords;	
    fragmentdata.Tangent   = normalize(vec3(u_Transform * vec4(a_tangent,   1.0)));
    fragmentdata.Bitangent = normalize(vec3(u_Transform * vec4(a_bitangent, 1.0)));
	mat3 normalMatrix = mat3(transpose(inverse(u_Transform)));
    fragmentdata.Normal    = normalize(vec3(normalMatrix * a_normal));
    fragmentdata.LightSpaceFragPos = u_LightSpaceMatrix * vec4(fragmentdata.Position, 1.0);

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
    vec4 LightSpaceFragPos;
};

layout(location = 0) in FragmentData fragmentdata;
layout(location = 14) in flat int in_Entity;


layout(binding = 0) uniform sampler2D u_Texture;
layout(binding = 1) uniform sampler2D u_DepthMap;
//uniform sampler2D u_DepthMap;
//uniform sampler2D u_LightSpaceMatrix;

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
	vec4 u_CameraPosition;
};


layout(std140, binding = 1) uniform LightBuffer
{
    Light u_Lights[MAX_LIGHTS];
};



vec3 FinalGamma(vec3 color)
{
	return pow(color, vec3(1.0 / GAMMA));
}

float ShadowCalculation(vec4 fragPosLightSpace) 
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_DepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

vec3 CalculatePointLight(Light light, vec3 Position, vec3 Normal)
{
    const float constantAtt = 0;
    const float linearAtt = 0.1;
    const float quadraticAtt = 0.1;

    // Normalize the normal vector (already normalized in vertex shader, but redundant normalization here)
    vec3 norm = normalize(Normal);
    // Calculate the direction to the light source
    vec3 lightDir = normalize(light.Position.xyz - Position);
    float distance = length(lightDir);
    float attenuation = 1.0 / (constantAtt + linearAtt + distance + quadraticAtt * distance * distance);

    // Calculate the diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.Color.rgb * attenuation;

    // Calculate the view direction
    vec3 viewDir = normalize(u_CameraPosition.xyz - Position);
    
    // Calculate the reflection direction
    vec3 reflectDir = reflect(-lightDir, norm);

    // Calculate the specular component
    float specularPower = light.Direction.a;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPower);
    vec3 specular = spec * u_AmbientLight.rgb;

    // Apply the light intensity to both diffuse and specular components
    float lightIntensity = light.Color.a;
    vec3 finalColor = (diffuse + specular) * lightIntensity;

    return finalColor;
}

vec3 CalculateDirectionalLight(Light light, vec3 Normal)
{
    vec3 lightDir = normalize(-light.Direction.xyz);
    vec3 diffuse = light.Color.rgb * max(dot(Normal, lightDir), 0.0);
    
	vec3 viewDir    = normalize(u_CameraPosition.xyz - fragmentdata.Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), light.Direction.a);
	vec3 specular = light.Color.xyz * spec;

    float shadow = ShadowCalculation(fragmentdata.LightSpaceFragPos);
	
    return (1.0 - shadow) * (diffuse + light.Color.a + specular);
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
    
    vec3 lighting = ambientLight + totalDiffuse;

	outColor = vec4(tex.xyz  * lighting, 1.0);
    //outColor = vec4(FinalGamma(tex.xyz * ambientLight + totalDiffuse), 1.0);
	//outColor = texture(u_DepthMap, projCoords.xy);
}