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
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0)); // 1ый аргумент связан с layout (location = 0) in cube shader
    GLCall(glEnableVertexAttribArray(0)); // аргумент связан с layout(location = 0)
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)))); // 1ый аргумент связан с layout (location = 0) in light shader
    GLCall(glEnableVertexAttribArray(1)); // аргумент связан с layout(location = 0)

    GLCall(glBindVertexArray(lampVAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, lightVerteces, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0)); // 1ый аргумент связан с layout (location = 0) in light shader
    GLCall(glEnableVertexAttribArray(0)); // аргумент связн с layout(location = 0)
    //GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)))); // 1ый аргумент связан с layout (location = 0) in light shader
    //GLCall(glEnableVertexAttribArray(1)); // аргумент связан с layout(location = 0)


    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));


    {

        

        // кубическая лампочка
        Shader lampShader("res/shaders/lamp.shader");
        lampShader.Bind(); // glUseProgram, тк у нас 1 пара шейдеров, то мы можем 1 раз сделать Bind и все

        // сам освещаемый объект
        Shader objectShader("res/shaders/object_lesson12.shader");
        objectShader.Bind(); // glUseProgram, тк у нас 1 пара шейдеров, то мы можем 1 раз сделать Bind и все


        glEnable(GL_DEPTH_TEST);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //main loop
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //glEnable(GL_DEPTH_TEST); вынесли вне цикла
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            //TODO - optimize by Cherno double using function


            lampShader.Bind();
            m4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            m4 view = camera.GetViewMatrix();
            lampShader.SetUniformMat4f("projection", projection);//Примечание: В настоящее время мы устанавливаем матрицу проекции для каждого кадра, но поскольку матрица проекции редко меняется, то рекомендуется устанавливать ее (единожды) вне основного цикла.
            lampShader.SetUniformMat4f("view", view);

            m4 model = m4(1.0f); // 
            float time = glfwGetTime();
            //lightPos.x = sin(time) / 2.0;
            //lightPos.y = cos(time * 1.0) / 2.0;

            //model = glm::translate(model, v3(sin(time) / 2.0 + 0.5, lightPos.y, cos(time) / 2.0 + 0.5 ));
            model = glm::translate(model, lightPos);
            model = glm::scale(model, v3(0.2));
            lampShader.SetUniformMat4f("model", model);
            GLCall(glBindVertexArray(lampVAO));
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));





            objectShader.Bind();
            m4 model2 = m4(1.0f);

            objectShader.SetUniformVec3f("light.position", lightPos);
            objectShader.SetUniformVec3f("cameraPos", camera.Position);
            // projection and view может быть одна и та же, projection может быть вообще мне loop
            objectShader.SetUniformMat4f("projection", projection);//Примечание: В настоящее время мы устанавливаем матрицу проекции для каждого кадра, но поскольку `матрица проекции редко меняется, то рекомендуется устанавливать ее (единожды) вне основного цикла.
            objectShader.SetUniformMat4f("view", view);
            objectShader.SetUniformMat4f("model", model2);

            v3 matAmbient = v3(0.24725, 0.1995, 0.0745);
            v3 matDiffuse = v3(0.75164, 0.60648, 0.22648);
            v3 matSpecular = v3(0.628281, 0.555802, 0.366065);
            objectShader.SetUniformVec3f("material.ambient", matAmbient);
            objectShader.SetUniformVec3f("material.diffuse", matDiffuse);
            objectShader.SetUniformVec3f("material.specular", matSpecular);
            objectShader.SetUniform1f("material.shininess", 0.4 * 128);




            v3 lightAmbient = v3(1.0f); // usually (0.2)
            v3 lightDiffuse = v3(1.0f); // usually (0.5)
            v3 lightSpecular = v3(1.0f); // usually (1.0)
            objectShader.SetUniformVec3f("light.ambient", lightAmbient);
            objectShader.SetUniformVec3f("light.diffuse", lightDiffuse);
            objectShader.SetUniformVec3f("light.specular", lightSpecular);
            glBindVertexArray(VAO);
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

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
