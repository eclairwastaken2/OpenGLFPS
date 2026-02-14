#version 330 core
#define MAX_LIGHTS 16
layout (location = 0) out vec4 FragColor; 
layout (location = 1) out vec4 BrightColor; 

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position; 
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 

    float constant; 
    float linear; 
    float quadratic; 
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light flashlight;
uniform int numPointLights; 
uniform PointLight pointLights[MAX_LIGHTS]; 
uniform vec3 emissiveColor; 
uniform float emissiveStrength; 
uniform bool isEmissive; 

void main()
{
    vec3 color = texture(material.diffuse, TexCoords).rgb;
    vec3 result = vec3(0.0);

    //FLASHLIGHT
    // ambient
    vec3 ambient = flashlight.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(flashlight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = flashlight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = flashlight.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-flashlight.direction)); 
    float epsilon = (flashlight.cutOff - flashlight.outerCutOff);
    float intensity = clamp((theta - flashlight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(flashlight.position - FragPos);
    float attenuation = 1.0 / (flashlight.constant + flashlight.linear * distance + flashlight.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   

    //GEM POINT LIGHTS
    for (int i = 0; i < numPointLights; i++)
    {
        PointLight currentLight = pointLights[i]; 
        
        vec3 currentLightDir = normalize(currentLight.position - FragPos); 
        float currentDiff = max(dot(norm, currentLightDir), 0.0); 

        vec3 currentReflectDir = reflect(-currentLightDir, norm); 
        float currSpec = pow(max(dot(viewDir, currentReflectDir), 0.0), material.shininess);
        
        float currDist = length(currentLight.position - FragPos); 
        float currAttenuation = 1.0 / (currentLight.constant + currentLight.linear * currDist + currentLight.quadratic * currDist * currDist); 

        vec3 currAmbient = currentLight.ambient * color; 
        vec3 currDiffuse = currentLight.diffuse * currentDiff * color; 
        vec3 currSpecular = currentLight.specular * currSpec;

        result += (currAmbient + currDiffuse+ currSpecular) * currAttenuation; 
    }   
   

    //EMISSIVE
        
    vec3 emissive = emissiveColor * emissiveStrength;
    result += ambient + diffuse + specular + emissive;
    FragColor = vec4(result, 1.0);

    if (isEmissive)
    {
        BrightColor = vec4(emissive, 1.0); 
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0); 
    }

} 