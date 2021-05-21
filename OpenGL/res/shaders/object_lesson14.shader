#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos; // 3 
layout(location = 1) in vec3 aNormal; // +3
layout(location = 2) in vec2 aTexCoord; // +2
layout(location = 3) in vec3 aColor; // +3

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 Color;


// compiler will delete unused uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



void main()
{

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0)); // перевели в мировые координаты
    Normal = mat3(transpose(inverse(model))) * aNormal; // именно mat3 чтобы выкинуть последний столбец для трансляции
    TexCoord = aTexCoord;
    Color = aColor;
}



#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Color;


uniform vec3 cameraPos;

struct Material {
    //vec3 ambient; // k1 не нужен
    sampler2D diffuse; // k2
    sampler2D specular; // k3
    sampler2D emission;
    float shininess; // степень характеризует блики 
};
uniform Material material;

struct Light {
    vec3 ambient; // обычно низкое значение // i1
    vec3 diffuse; // i2 по сути это интенсиность света
    vec3 specular; // i3
};
uniform Light light;

struct DirLight {
    vec3 direction; 
    vec3 ambient; // i1 интенсивность
    vec3 diffuse; // i2
    vec3 specular; // i3
};
uniform DirLight dirLight;

struct PointLight  {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float lin;
    float quadratic;
};
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLight[NR_POINT_LIGHTS];


struct SpotLight {
   
    vec3 direction;
    vec3 position;
    float cutOff; // it is cos
    float outerCutOff; // it is cos

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float lin;
    float quadratic;
};
uniform SpotLight spotLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewVec);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewVec, vec3 fragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewVec, vec3 fragPos);

void main()
{
  
    vec3 result = vec3(0.0);
    vec3 normal = normalize(Normal);
    vec3 viewVec = normalize(FragPos - cameraPos);

    float q = spotLight.lin + spotLight.constant + spotLight.quadratic;

    result += CalcDirLight(dirLight, normal, viewVec); // sun
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLight[i], normal, viewVec, FragPos);
    }

    result += CalcSpotLight(spotLight, normal, viewVec, FragPos); // spot


    FragColor = vec4(result, 1.0);
  
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewVec) { // общий свет, солнце

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    //vec3 toLight = normalize(light.position - FragPos); // для точечного света
    vec3 toLight = normalize(-light.direction); // для общего света
    vec3 norm = normalize(normal);
    float diff = max(0.0f, dot(toLight, norm));
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoord).rgb * diff;

    //vec3 viewVec = normalize(cameraPos - FragPos); we don't need do this
    vec3 reflectLight = normalize(reflect(-toLight, norm));
    float spec = pow(max(0.0f, dot(reflectLight, viewVec)), material.shininess);
    vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec; // here multiply Color

    return ambient + diffuse + specular; // objectColor define by light and its lighting

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewVec, vec3 fragPos) { // точечный свет, точка

    float d = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.lin * d + light.quadratic * d * d); // faster d*d = dot(light.position - FragPos, light.position - FragPos);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    //vec3 toLight = normalize(-light.direction); // для общего света
    vec3 toLight = normalize(light.position - FragPos); // для точечного света
    vec3 norm = normalize(normal);
    float diff = max(0.0f, dot(toLight, norm));
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoord).rgb * diff;

    //vec3 viewVec = normalize(cameraPos - FragPos);
    vec3 reflectLight = normalize(reflect(-toLight, norm));
    float spec = pow(max(0.0f, dot(reflectLight, viewVec)), material.shininess);
    vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec; // here multiply Color

    ambient *= attenuation; // все затухает
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular; // objectColor define by light and its lighting

}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewVec, vec3 fragPos) { // точечный свет, точка

    float tetta = dot(normalize(FragPos - light.position), normalize(light.direction));
    float intensity = clamp((tetta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);

    float d = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.lin * d + light.quadratic * d * d); // faster d*d = dot(light.position - FragPos, light.position - FragPos);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    vec3 toLight = normalize(light.position - FragPos); // для точечного света
    vec3 norm = normalize(normal);
    float diff = max(0.0f, dot(toLight, norm));
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoord).rgb * diff;

    //vec3 viewVec = normalize(cameraPos - FragPos);
    vec3 reflectLight = normalize(reflect(-toLight, norm));
    float spec = pow(max(0.0f, dot(reflectLight, viewVec)), material.shininess);
    vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec; // here multiply Color

    ambient *= attenuation; // все затухает
    diffuse *= attenuation;
    specular *= attenuation;

    // ambient *= intensity // оставляем немного света, не умножаем на интенсивность
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular; // objectColor define by light and its lighting

}