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

struct Light {
    vec3 position;

    //по сути это интенсиность света
    vec3 ambient; // обычно низкое значение // i1
    vec3 diffuse; // i2
    vec3 specular; // i3

};

uniform Material material;
uniform Light light;

vec3 calcEmission() {
    //vec3 show = step(vec3(0.2), texture(material.specular, TexCoord).rgb);
    vec3 show = step(vec3(1.0), vec3(1.0) - texture(material.specular, TexCoord).rgb);
    return texture(material.emission, TexCoord).rgb * show;
}

void main()
{
    
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb 
        + calcEmission(); // вместо lightColor - light.ambient // k1 * i1

    vec3 toLight = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(0.0f, dot(toLight, norm));
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoord).rgb * diff
       + calcEmission(); // вместо lightColor - light.diffuse // k2 * i2 * cos


  
    vec3 reflectLight = normalize(reflect(-toLight, norm));
    vec3 viewVec = normalize(cameraPos - FragPos);
    float spec = pow(max(0.0f, dot(reflectLight, viewVec)), material.shininess); 
    vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec;
    // here multiply Color

    vec3 result = ambient + diffuse + specular; // objectColor define by light and its lighting
    FragColor = vec4(result, 1.0);


}
