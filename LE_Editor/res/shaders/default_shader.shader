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
//lyout(location = 14) out flat int o_Entity;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_position, 1.0);

    fragmentdata.Position          = vec3(u_Transform * vec4(a_position, 1.0));
    fragmentdata.Normal            = transpose(inverse(mat3(u_Transform))) * a_normal;
	fragmentdata.TexCoords         = a_texcoords;	
    fragmentdata.Tangent           = vec3(u_Transform * vec4(a_tangent,   1.0));
    fragmentdata.Bitangent         = vec3(u_Transform * vec4(a_bitangent, 1.0));
    fragmentdata.LightSpaceFragPos = u_LightSpaceMatrix * vec4(fragmentdata.Position, 1.0);
	
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 outColor;
//layout(location = 1) out int o_entity;

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

float ShadowCalculation(vec4 fragPosLightSpace, Light light) 
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_DepthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fragmentdata.Normal);
    vec3 lightDir = normalize(-light.Direction.xyz);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_DepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
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
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-light.Direction.xyz);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * light.Color.rgb  * light.Color.a;
    
	vec3 viewDir    = normalize(u_CameraPosition.xyz - fragmentdata.Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), light.Direction.a);
	vec3 specular = light.Color.xyz * spec;

    float shadow = ShadowCalculation(fragmentdata.LightSpaceFragPos, light);

    float ambientIntensity = u_AmbientLight.a;
    vec3 ambientLight = u_AmbientLight.rgb * ambientIntensity;
    
    return ambientLight + (1.0 - shadow) * (diffuse + specular);
}

void main()
{
   // o_entity = in_Entity;
	
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
   
    outColor = vec4(FinalGamma(tex.xyz * totalDiffuse), 1.0);
}