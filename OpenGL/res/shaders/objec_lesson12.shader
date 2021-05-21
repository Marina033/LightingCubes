#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos; // 3 
layout(location = 1) in vec3 aNormal; // +3

out vec3 Normal;
out vec3 FragPos;


// compiler will delete unused uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



void main()
{
    
    gl_Position = projection * view * model* vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0)); // перевели в мировые координаты
    Normal = mat3(transpose(inverse(model))) * aNormal; // именно mat3 чтобы выкинуть последний столбец для трансляции
   
}



#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;




//uniform sampler2D ourTexture; // можем никак не привязывать sampler2D в maine если он один
//uniform sampler2D ourTexture2;
//uniform float alpha;

uniform vec3 cameraPos;

struct Material {
    vec3 ambient; // k1
    vec3 diffuse; // k2
    vec3 specular; // k3
    float shininess; 
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

// lightColor and maybe objectColor we define in main


void main()
{
    
    vec3 ambient = light.ambient * material.ambient ; // вместо lightColor - light.ambient // k1 * i1

    vec3 toLight = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(0.0f, dot(toLight, norm)); 
    vec3 diffuse = light.diffuse * material.diffuse * diff; // вместо lightColor - light.diffuse // k2 * i2 * cos

    vec3 reflectLight = reflect(-toLight, norm);
    vec3 viewVec = normalize(cameraPos - FragPos);
    float spec = pow(max(0.0f, dot(reflectLight, viewVec)), material.shininess); // степень характеризует блики 
    vec3 specular = light.specular * material.specular * spec; // вместо lightColor - light.specular // k3 * i3 * cos^shine


    vec3 result = ambient + diffuse + specular; // objectColor define by light and its lighting
    FragColor = vec4(result, 1.0);

    
}
