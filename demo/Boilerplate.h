#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Application {
private:
    GLFWwindow* window;
    unsigned int width;
    unsigned int height;
    const char* title;

    static void framebuffer_size_callback_internal(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

public:
    Application(unsigned int w = 800, unsigned int h = 600, const char* windowTitle = "OpenGL Application")
        : width(w), height(h), title(windowTitle), window(nullptr) {
    }

    bool initialize() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Configure GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create window
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_internal);

        // Load OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        return true;
    }

    void run(std::function<void()> setupCallback,
        std::function<void(float)> updateCallback,
        std::function<void()> renderCallback) {

        // Call setup once
        if (setupCallback) setupCallback();

        float lastFrame = 0.0f;

        // Main render loop
        while (!glfwWindowShouldClose(window)) {
            // Calculate delta time
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Update logic
            if (updateCallback) updateCallback(deltaTime);

            // Clear buffers
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Render
            if (renderCallback) renderCallback();

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void shutdown() {
        glfwTerminate();
    }

    GLFWwindow* getWindow() const { return window; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

    ~Application() {
        if (window) {
            shutdown();
        }
    }
};

#endif
