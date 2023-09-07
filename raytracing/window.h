#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <iostream>

const unsigned int window_width = 1280;
const unsigned int window_height = 720;
GLFWwindow* main_window = nullptr;

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// create gl window
int create_window() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// minimum requirement
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// minimum requirement
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // create glfw window
    main_window = glfwCreateWindow(window_width, window_height, "Raytracing", NULL, NULL);
    if (main_window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(main_window);
    glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // init GL states
    glLineWidth(1.0f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                   	// set clear color
    glEnable(GL_CULL_FACE);								        // turn on backface culling
    glEnable(GL_DEPTH_TEST);								    // turn on depth tests

    return 0;
}

// the main render loop, should be called after init_window() and create_window()
void render() {
    // input
    process_input(main_window);

    // render commands
    glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // check and call events and swap buffers
    glfwSwapBuffers(main_window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process input
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#endif