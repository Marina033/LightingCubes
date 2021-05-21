//Для второй текстуры мы будем использовать изображение вашего выражения лица во время изучения OpenGL : =)

// TODO поменять 4 определения функции в main после обработки ошибок
 //неважно что сначала рисовать
//#include <gl/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// глобальные переменные распространяются на все файлы

using namespace glm;
typedef glm::vec3 v3;
typedef glm::mat4 m4;

//float alpha = 0.2f;
const int WIDTH = 800, HEIGHT = 600;
float lastX = WIDTH / 2, lastY = HEIGHT / 2;
float deltaTime = 0.0, lastFrame = 0.0;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera camera;
//v3 lightPos(1.2f, 1.0f, 2.0f);
v3 lightPos(1.2f, 0.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int COUNT_LIGHTS = 4;
const unsigned int COUNT_OBJECTS = 10;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    


  
    float vertices[] = {
        // координаты        // нормали           // текстурные координаты  // цвета вершин
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,               1.0f, 1.0f, 0.0f, //y
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r
                                                                                             
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,               1.0f, 1.0f, 0.0f, //y
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,               1.0f, 1.0f, 0.0f, //y
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,               1.0f, 1.0f, 0.0f, //y
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,               1.0f, 0.0f, 0.0f, //r
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,               1.0f, 0.0f, 0.0f, //r
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,               1.0f, 1.0f, 0.0f, //y

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,               0.0f, 0.0f, 1.0f, //b

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,               1.0f, 0.0f, 0.0f, //r
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,               0.0f, 1.0f, 0.0f, //g 
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,               0.0f, 1.0f, 0.0f, //g 
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,               1.0f, 0.0f, 0.0f, //r 
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,               1.0f, 0.0f, 0.0f, //r

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,               1.0f, 1.0f, 0.0f, //y
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,               0.0f, 0.0f, 1.0f, //b
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,               0.0f, 0.0f, 1.0f, //b
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,               1.0f, 1.0f, 0.0f, //y
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,               1.0f, 1.0f, 0.0f, //y
    };
    unsigned int indices[] = {
         0, 1, 2,
         0, 2, 3
    };
   
   
  
    //initilizing different buffers 
    unsigned int VAO, VBO, EBO, lampVAO; // maybe calling GLCall means wraping this line into scope {}
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glGenVertexArrays(1, &lampVAO));
    


    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0)); // 1ый аргумент связан с layout (location = 0) in cube shader
    GLCall(glEnableVertexAttribArray(0)); // аргумент связан с layout(location = 0)
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)))); // 1ый аргумент связан с layout (location = 1) in light shader
    GLCall(glEnableVertexAttribArray(1)); // аргумент связан с layout(location = 1)
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)))); // 1ый аргумент связан с layout (location = 2) in light shader
    GLCall(glEnableVertexAttribArray(2)); // аргумент связан с layout(location = 2)
    GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (8 * sizeof(float)))); // 1ый аргумент связан с layout (location = 2) in light shader
    GLCall(glEnableVertexAttribArray(3)); // аргумент связан с layout(location = 2)


    GLCall(glBindVertexArray(lampVAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, lightVerteces, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0)); // 1ый аргумент связан с layout (location = 0) in light shader
    GLCall(glEnableVertexAttribArray(0)); // аргумент связн с layout(location = 0)
    //GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)))); // 1ый аргумент связан с layout (location = 0) in light shader
    //GLCall(glEnableVertexAttribArray(1)); // аргумент связан с layout(location = 0)


    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    v3 cubePositions[] = {
           glm::vec3(0.0f,  0.0f,  0.0f),
           glm::vec3(2.0f,  5.0f, -15.0f),
           glm::vec3(-1.5f, -2.2f, -2.5f),
           glm::vec3(-3.8f, -2.0f, -12.3f),
           glm::vec3(2.4f, -0.4f, -3.5f),
           glm::vec3(-1.7f,  3.0f, -7.5f),
           glm::vec3(1.3f, -2.0f, -2.5f),
           glm::vec3(1.5f,  2.0f, -2.5f),
           glm::vec3(1.5f,  0.2f, -1.5f),
           glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    v3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.6f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0, 0.0),
    glm::vec3(0.2f, 0.2f, 1.0f)
    };
    {

        Texture texture("res/textures/containerFramed.png");
        texture.Bind(); //строчка нужна для связывания нескольких текстур
        Texture texture1("res/textures/mapDiffuse.png");
        texture1.Bind(1);
        Texture texture2("res/textures/mapEmission.jpg");
        texture2.Bind(2);

        // кубическая лампочка
        Shader lampShader("res/shaders/lamp.shader");
        lampShader.Bind(); // glUseProgram, тк у нас 1 пара шейдеров, то мы можем 1 раз сделать Bind и все
        m4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        lampShader.SetUniformMat4f("projection", projection);//Примечание: В настоящее время мы устанавливаем матрицу проекции для каждого кадра, но поскольку матрица проекции редко меняется, то рекомендуется устанавливать ее (единожды) вне основного цикла.

        // сам освещаемый объект
        Shader objectShader("res/shaders/object_lesson14.shader");
        objectShader.Bind(); // glUseProgram, тк у нас 1 пара шейдеров, то мы можем 1 раз сделать Bind и все
        objectShader.SetUniformMat4f("projection", projection);//В настоящее время мы устанавливаем матрицу проекции для каждого кадра, но поскольку `матрица проекции редко меняется, то рекомендуется устанавливать ее (единожды) вне основного цикла.
        objectShader.SetUniform1i("material.diffuse", 0); // не вектор, а номер текстуры
        objectShader.SetUniform1i("material.specular", 1); 
        objectShader.SetUniform1i("material.emission", 2);

        
        glEnable(GL_DEPTH_TEST);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //main loop
        while (!glfwWindowShouldClose(window)) {
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            //glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
            glClearColor(0.9f, 0.9f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            //TODO - optimize by Cherno double using function


            lampShader.Bind();
            m4 view = camera.GetViewMatrix();
            lampShader.SetUniformMat4f("view", view);

            m4 model = m4(1.0f); // 
            float time = glfwGetTime();
            //lightPos.x = sin(time) / 2.0;
            //lightPos.y = cos(time * 1.0) / 2.0;

            //model = glm::translate(model, v3(sin(time) / 2.0 + 0.5, lightPos.y, cos(time) / 2.0 + 0.5 ));
            
            GLCall(glBindVertexArray(lampVAO));
            for (unsigned int i = 0; i < 4; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, v3(0.2)); // все будут такие
                lampShader.SetUniformMat4f("model", model);
                lampShader.SetUniformVec3f("lightColor", pointLightColors[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }



            objectShader.Bind();
            texture.Bind();
            texture1.Bind(1);
            texture2.Bind(2);
            
            m4 model2 = m4(1.0f);
            objectShader.SetUniformVec3f("cameraPos", camera.Position); // projection and view может быть одна и та же, projection может быть вообще мне loop
            objectShader.SetUniformMat4f("view", view);
            objectShader.SetUniformMat4f("model", model2);
            objectShader.SetUniform1f("material.shininess", 64);
         
            objectShader.SetUniformVec3f("dirLight.direction", camera.Front);
            objectShader.SetUniformVec3f("dirLight.ambient",  v3(0.1f));
            objectShader.SetUniformVec3f("dirLight.diffuse",  v3(0.5f));
            objectShader.SetUniformVec3f("dirLight.specular", v3(1.0f));

            for (GLuint i = 0; i < COUNT_LIGHTS; i++) {
                std::string number = std::to_string(i);

                objectShader.SetUniformVec3f(("pointLight[" + number + "].position").c_str(), pointLightPositions[i]); // use lightColor
                objectShader.SetUniformVec3f(("pointLight[" + number + "].ambient").c_str(), pointLightColors[i] * v3(0.2f)); // use lightColor
                objectShader.SetUniformVec3f(("pointLight[" + number + "].diffuse").c_str(), pointLightColors[i] * v3(1.0f)); // use lightColor
                objectShader.SetUniformVec3f(("pointLight[" + number + "].specular").c_str(), v3(1.0f));
                objectShader.SetUniform1f(("pointLight[" + number + "].constant").c_str(), 1.0f);
                objectShader.SetUniform1f(("pointLight[" + number + "].lin").c_str(), 0.09f); // 0.09 optimal // от этого зависит какой цвет в итоге долетит до предмета
                objectShader.SetUniform1f(("pointLight[" + number + "].quadratic").c_str(), 0.032f); // 0.032 optimal
            }
            
            
            objectShader.SetUniformVec3f("spotLight.direction", camera.Front);
            objectShader.SetUniformVec3f("spotLight.position",  camera.Position);
            objectShader.SetUniform1f("spotLight.cutOff",      cos(radians(5.5f)));
            objectShader.SetUniform1f("spotLight.outerCutOff", cos(radians(10.5f)));
            objectShader.SetUniformVec3f("spotLight.ambient",  v3(0.2f));
            objectShader.SetUniformVec3f("spotLight.diffuse",  v3(0.5f));
            objectShader.SetUniformVec3f("spotLight.specular", v3(1.0f));
            objectShader.SetUniform1f("spotLight.constant",  1.0f);
            objectShader.SetUniform1f("spotLight.lin",       0.09f);
            objectShader.SetUniform1f("spotLight.quadratic", 0.032f);
           
            GLCall(glBindVertexArray(VAO));
            for (unsigned int i = 0; i < COUNT_OBJECTS; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                objectShader.SetUniformMat4f("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            

            //GLCall(glBindVertexArray(VAO)); // important string, but we don't need do this
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
          
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &lampVAO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    } // here shader deletes because destructor calls // glDeleteProgram(...);
    
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset); // господи, здесь должно было быть смещение, а не позиция
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
 

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
      camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
      camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
      camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
      camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}
