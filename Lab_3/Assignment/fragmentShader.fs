#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

// Number of point lights
#define NR_POINT_LIGHTS 4

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float k_c;  // constant attenuation
    float k_l;  // linear attenuation
    float k_q;  // quadratic attenuation
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;       // cos of cutoff angle
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float k_c;
    float k_l;
    float k_q;
};

// Uniforms
uniform vec3 viewPos;
uniform Material material;

// Lights
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

// Light toggles
uniform bool directionalLightOn;
uniform bool pointLightOn;
uniform bool spotLightOn;

// Component toggles
uniform bool ambientOn;
uniform bool diffuseOn;
uniform bool specularOn;

// Emissive
uniform bool isEmissive;
uniform vec3 emissiveColor;

// Function prototypes
vec3 CalcDirectionalLight(Material mat, DirectionalLight light, vec3 N, vec3 V);
vec3 CalcPointLight(Material mat, PointLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcSpotLight(Material mat, SpotLight light, vec3 N, vec3 fragPos, vec3 V);

void main()
{
    // Check if this is an emissive surface (like a light bulb)
    if (isEmissive) {
        FragColor = vec4(emissiveColor, 1.0);
        return;
    }

    // Properties
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // Directional light
    if (directionalLightOn) {
        result += CalcDirectionalLight(material, directionalLight, N, V);
    }

    // Point lights
    if (pointLightOn) {
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            result += CalcPointLight(material, pointLights[i], N, FragPos, V);
        }
    }

    // Spot light
    if (spotLightOn) {
        result += CalcSpotLight(material, spotLight, N, FragPos, V);
    }

    // Ensure minimum visibility if all lights are off
    if (!directionalLightOn && !pointLightOn && !spotLightOn) {
        result = material.ambient * 0.1;
    }

    FragColor = vec4(result, 1.0);
}

// Calculates directional light contribution
vec3 CalcDirectionalLight(Material mat, DirectionalLight light, vec3 N, vec3 V)
{
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);

    // Ambient
    vec3 ambient = vec3(0.0);
    if (ambientOn) {
        ambient = mat.ambient * light.ambient;
    }

    // Diffuse
    vec3 diffuse = vec3(0.0);
    if (diffuseOn) {
        float diff = max(dot(N, L), 0.0);
        diffuse = mat.diffuse * diff * light.diffuse;
    }

    // Specular
    vec3 specular = vec3(0.0);
    if (specularOn) {
        float spec = pow(max(dot(V, R), 0.0), mat.shininess);
        specular = mat.specular * spec * light.specular;
    }

    return (ambient + diffuse + specular);
}

// Calculates point light contribution
vec3 CalcPointLight(Material mat, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);

    // Attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * d * d);

    // Ambient
    vec3 ambient = vec3(0.0);
    if (ambientOn) {
        ambient = mat.ambient * light.ambient * attenuation;
    }

    // Diffuse
    vec3 diffuse = vec3(0.0);
    if (diffuseOn) {
        float diff = max(dot(N, L), 0.0);
        diffuse = mat.diffuse * diff * light.diffuse * attenuation;
    }

    // Specular
    vec3 specular = vec3(0.0);
    if (specularOn) {
        float spec = pow(max(dot(V, R), 0.0), mat.shininess);
        specular = mat.specular * spec * light.specular * attenuation;
    }

    return (ambient + diffuse + specular);
}

// Calculates spot light contribution
vec3 CalcSpotLight(Material mat, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);

    // Check if within spotlight cone
    float theta = dot(L, normalize(-light.direction));
    
    if (theta < light.cutOff) {
        // Outside spotlight cone - only ambient (dimmed)
        if (ambientOn) {
            return mat.ambient * light.ambient * 0.1;
        }
        return vec3(0.0);
    }

    // Attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * d * d);

    // Intensity based on angle (soft edge)
    float intensity = (theta - light.cutOff) / (1.0 - light.cutOff);
    intensity = clamp(intensity, 0.0, 1.0);

    // Ambient
    vec3 ambient = vec3(0.0);
    if (ambientOn) {
        ambient = mat.ambient * light.ambient * attenuation;
    }

    // Diffuse
    vec3 diffuse = vec3(0.0);
    if (diffuseOn) {
        float diff = max(dot(N, L), 0.0);
        diffuse = mat.diffuse * diff * light.diffuse * attenuation * intensity;
    }

    // Specular
    vec3 specular = vec3(0.0);
    if (specularOn) {
        float spec = pow(max(dot(V, R), 0.0), mat.shininess);
        specular = mat.specular * spec * light.specular * attenuation * intensity;
    }

    return (ambient + diffuse + specular);
}

