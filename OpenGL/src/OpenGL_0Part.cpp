//Для второй текстуры мы будем использовать изображение вашего выражения лица во время изучения OpenGL : =)

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

using namespace glm;
typedef glm::vec3 v3;
typedef glm::mat4 m4;

float alpha = 0.2f;
const int WIDTH = 800, HEIGHT = 600;
float lastX = WIDTH / 2, lastY = HEIGHT / 2;

v3 cameraPos(0.0, 0.0, 3.0);
v3 dir(0.0, 0.0, -1.0); // like viewVec, cameraFront
v3 viewVec(0.0, 0.0, -1.0); // like dir, cameraFront
v3 cameraUp(0.0, 1.0, 0.0);

float deltaTime = 0.0, lastFrame = 0.0;
float yaw = -90.0, pitch = 0.0;
bool firstMouse = true;
float fov = 45.0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera camera;


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




    /* float vertices[] = {
         // coords              colors            textureCoords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // bottom right
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // bottom left
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
     };*/
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
         0, 1, 2,
         0, 2, 3
    };
    glm::vec3 cubePositions[] = {
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


    //initilizing different buffers 
    unsigned int VAO, VBO, EBO; // maybe calling GLCall means wraping this line into scope {}
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glGenBuffers(1, &EBO));


    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0)); // 1ый аргумент связан с layout (location = 0)
    GLCall(glEnableVertexAttribArray(0)); // аргумент связан с layout(location = 0)
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)))); // 1ый аргумент связан с layout (location = 1)
    GLCall(glEnableVertexAttribArray(1)); // аргумент связан с layout (location = 1)
    //GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)))); // 1ый аргумент связан с layout (location = 2)
    //GLCall(glEnableVertexAttribArray(2)); // аргумент связан с layout (location = 2)


    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    //glBindBuffer(GL_ARRAY_BUFFER, 0); we don't need do this
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); we can't do this
    //glBindVertexArray(0); we don't need do this



    {

        Texture texture("res/textures/container.jpg");
        texture.Bind(); //строчка нужна для связывания нескольких текстур
        Texture texture2("res/textures/face.png");
        texture2.Bind(1);

        Shader shaders("res/shaders/basic.shader");
        shaders.Bind(); // glUseProgram, тк у нас 1 пара шейдеров, то мы можем 1 раз сделать Bind и все
        shaders.SetUniform1i("ourTexture", 0);
        shaders.SetUniform1i("ourTexture2", 1);
        shaders.SetUniform1f("alpha", alpha);


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //main loop
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            shaders.Bind();
            texture.Bind();
            texture2.Bind(1);
            shaders.SetUniform1f("alpha", alpha);
            glm::mat4 trans = glm::mat4(1.0f);

            //trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
            //trans = glm::rotate(trans, glm::sin(float(glfwGetTime())), glm::vec3(0.0f, 0.0f, 1.0f));
            shaders.SetUniformMat4f("transform", trans);



            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            shaders.SetUniformMat4f("projection", projection);//Примечание: В настоящее время мы устанавливаем матрицу проекции для каждого кадра, но поскольку матрица проекции редко меняется, то рекомендуется устанавливать ее (единожды) вне основного цикла.


            //glm::mat4 view = glm::mat4(1.0f);
            //viewVec = glm::normalize(-cameraPos);
            //v3 target = cameraPos + viewVec;
            //view = glm::lookAt(cameraPos, target, cameraUp);
            glm::mat4 view = camera.GetViewMatrix();
            shaders.SetUniformMat4f("view", view);


            GLCall(glBindVertexArray(VAO));
            for (int i = 0; i < 10; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = glm::radians(20.0f * i);
                //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f+(float)i * 10), glm::vec3(0.5f, 1.0f, 0.0f));
                model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));
                shaders.SetUniformMat4f("model", model);
                GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
            }







            //GLCall(glBindVertexArray(VAO)); // important string, but we don't need do this
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

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

    camera.ProcessMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        alpha += 0.001f; // измените это значение соответствующим образом (может быть слишком медленым или слишком быстрым в зависимости от вашего системного оборудования)
        if (alpha >= 1.0f)
            alpha = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        alpha -= 0.001f; // измените это значение соответствующим образом (может быть слишком медленым или слишком быстрым в зависимости от вашего системного оборудования)
        if (alpha <= 0.0f)
            alpha = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}

/*void mouse_callback(GLFWwindow * window, double xpos, double ypos) // этот вариант не работает, мышка не участвует
{
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

    float sensitivity = 0.1f; // измените это значение по своему усмотрению
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Убеждаемся, что когда значение тангаж выходит за пределы, экран не переворачивается
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    viewVec = glm::normalize(front);
}*/

