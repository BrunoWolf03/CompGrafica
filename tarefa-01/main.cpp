#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "error.h"
#include "shader.h"
#include "polygon.h"

static PolygonPtr pol;
static ShaderPtr shd;

static void initialize() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    shd = Shader::Make();
    shd->AttachVertexShader("vertex.glsl");
    shd->AttachFragmentShader("fragment.glsl");
    shd->Link();
    float coord[] = {
      -0.5f,-0.5f,
      -0.90f, 0.75f, //novo vertice 2
       0.0f, 0.5f,
       0.75f, 0.75f, //novo vertice 1
       0.5f,-0.5f,

    };
    unsigned char color[] = {
      255,0,0,
      0,255,0,
      0,0,255,
      255,0,255, //cor do novo 1 
      192,192,192 //cor do novo 2 
    };
    unsigned int index[] = {
        0,1,2,3, 4
    };

    pol = Polygon::Make(5, coord, color, index);

    Error::Check("initialize");
}

static void display(GLFWwindow* win) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shd->UseProgram();
    pol->Draw();
    Error::Check("display");
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

static void error(int code, const char* msg) {
    printf("GLFW error %d: %s\n", code, msg);
    glfwTerminate();
    exit(0);
}

int main() {
    glfwSetErrorCallback(error);
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Could not initialize GLFW" << std::endl;
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);     // required for macOS
#if __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);  // option for macOS
#endif

    GLFWwindow* win = glfwCreateWindow(600, 400, "Polygon", nullptr, nullptr);
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
    while (!glfwWindowShouldClose(win)) {
        display(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
