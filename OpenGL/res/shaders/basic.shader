#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos; // 3 
layout(location = 1) in vec2 aTexCoord; // +2 
//layout(location = 1) in vec3 aColor; // +3

//out vec3 ourPosition;
//out vec4 ourColor;
//out vec2 ourTexCoord;

uniform mat4 transform; // compiler will delete unused uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    gl_Position = transform * projection * view * model * vec4(aPos, 1.0);
    //gl_Position = transform * vec4(aPos, 1.0);
    //ourPosition = aPos;
    //ourColor = vec4(aColor, 1.0);
    ourTexCoord = aTexCoord;

}



#shader fragment
#version 330 core

//in vec3 ourPosition;
//in vec4 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture; // можем никак не привязывать в maine если он один
uniform sampler2D ourTexture2;
uniform float alpha;

out vec4 FragColor;
void main()
{
    //FragColor = vec4(ourPosition, 1.0);
    FragColor = mix(texture(ourTexture, ourTexCoord), texture(ourTexture2, ourTexCoord), alpha);

}