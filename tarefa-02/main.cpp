#include <windows.h>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "error.h"
#include "shader.h"
#include "Disk.h"

static DiskPtr disco;
static ShaderPtr shd;
static float Angle;

static void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    shd = Shader::Make();
    shd->AttachVertexShader("vertex.glsl");
    shd->AttachFragmentShader("fragment.glsl");
    shd->Link();

    disco = Disk::Make(64);

    Error::Check("initialize");
}


static void update(double T) {
    Angle += T * 45.0f;
}
static void display(GLFWwindow * win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shd->UseProgram();
    glm::vec3 color( 1.0f, 1.0f, 0.0f); 
    shd->SetUniform("uColor", color);
    glm::mat4 sol=glm::mat4(1.0f);
    sol = glm::scale(sol, glm::vec3(0.5f,0.5f,0.5f));
    shd->SetUniform("M", sol);
    disco->Draw();

    glm::vec3 colorTerra(0.0f, 0.5f, 1.0f);
    shd->SetUniform("uColor", colorTerra);
    glm::mat4 terra = glm::mat4(1.0f);
    terra = glm::rotate(terra, glm::radians(Angle),glm::vec3(0.0f,0.0f,-1.0f));
    terra = glm::translate(terra, glm::vec3(0.85f,0.0f,0.0f));
    terra = glm::scale(terra, glm::vec3(0.2f, 0.2f, 0.2f));
    shd->SetUniform("M", terra);
    disco->Draw();
}

static void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize(GLFWwindow * win, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void error (int code, const char* msg)
{
    printf("GLFW error %d: %s\n", code, msg);
    glfwTerminate();
    exit(0);
}

int main()
{
    glfwSetErrorCallback(error);
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Could not initialize GLFW" << std::endl;
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);     // required for mac os
#if __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);  /
#endif


    GLFWwindow* win = glfwCreateWindow(400, 400, "Simulacao de Órbita", nullptr, nullptr);
    if (!win) {
        std::cerr << "Could not create GLFW window" << std::endl;
        return 0;
    }
    glfwMakeContextCurrent(win);

#ifdef __glad_h_
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD OpenGL context\n");
        exit(1);
    }
#endif

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetFramebufferSizeCallback(win, resize);  // resize callback
    glfwSetKeyCallback(win, keyboard);   // keyboard callback

    initialize();
    double t0 = glfwGetTime();
    double t;
    while (!glfwWindowShouldClose(win)) {
        t = glfwGetTime();
        update(t - t0);
        t0 = t;
        display(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

