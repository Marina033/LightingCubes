#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos; // 3 

//out vec3 ourPosition;
//out vec4 ourColor;
//out vec2 ourTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
    
}





#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);

}