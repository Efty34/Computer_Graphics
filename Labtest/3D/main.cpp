#include "LabUtils.h"     // Setup Window
#include "Shader.h"       // Shaders
#include "Cube.h"         // The Cube
#include "Cylinder.h"     // The Cylinder
#include "MyTransform.h"  // Your Custom Math
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// --- GLOBAL VARIABLES (4 Sets of State) ---

// Set A: GLM Arbitrary (Top-Left)
float tx_A = 0.0f, rot_A = 0.0f, sc_A = 0.0f;

// Set B: Custom Arbitrary (Top-Right)
float tx_B = 0.0f, rot_B = 0.0f, sc_B = 0.0f;

// Set C: GLM Pivot (Bottom-Left)
float tx_C = -2.0f, rot_C = 0.0f, sc_C = 0.5f;

// Set D: Custom Pivot (Bottom-Right)
float tx_D = 2.0f, rot_D = 0.0f, sc_D = 0.5f;


// Helper function: Scale along a specific axis direction
glm::mat4 scaleAlongAxis(glm::vec3 axis, float scaleFactor) {
    // Formula: S = I + (s-1) * (n ⊗ n)
    // where n is the normalized axis and s is the scale factor
    glm::vec3 n = glm::normalize(axis);
    float s = scaleFactor;
    
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix[0][0] = 1.0f + (s - 1.0f) * n.x * n.x;
    scaleMatrix[0][1] = (s - 1.0f) * n.x * n.y;
    scaleMatrix[0][2] = (s - 1.0f) * n.x * n.z;
    
    scaleMatrix[1][0] = (s - 1.0f) * n.y * n.x;
    scaleMatrix[1][1] = 1.0f + (s - 1.0f) * n.y * n.y;
    scaleMatrix[1][2] = (s - 1.0f) * n.y * n.z;
    
    scaleMatrix[2][0] = (s - 1.0f) * n.z * n.x;
    scaleMatrix[2][1] = (s - 1.0f) * n.z * n.y;
    scaleMatrix[2][2] = 1.0f + (s - 1.0f) * n.z * n.z;
    
    return scaleMatrix;
}

void printInstructions() {
    std::cout << "==============================================\n";
    std::cout << "      12-KEY CONTROL SCHEME (NO MODES)        \n";
    std::cout << "==============================================\n";
    std::cout << "GROUP A (GLM Standard)   -> [Q] Move  [W] Rotate  [E] Scale\n";
    std::cout << "GROUP B (Custom Standard)-> [R] Move  [T] Rotate  [Y] Scale\n";
    std::cout << "GROUP C (GLM Pivot)      -> [A] Move  [S] Rotate  [D] Scale\n";
    std::cout << "GROUP D (Custom Pivot)   -> [F] Move  [G] Rotate  [H] Scale\n";
    std::cout << "==============================================\n";
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 0.01f;
    float rotSpeed = 1.0f;

    // --- GROUP A: GLM Standard (Q, W, E) ---
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) tx_A += speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rot_A += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) sc_A += speed;

    // --- GROUP B: Custom Standard (R, T, Y) ---
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) tx_B += speed;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) rot_B += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) sc_B += speed;

    // --- GROUP C: GLM Pivot (A, S, D) ---
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) tx_C += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) rot_C += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sc_C += speed;

    // --- GROUP D: Custom Pivot (F, G, H) ---
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) tx_D += speed;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) rot_D += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) sc_D += speed;
}

int main() {
    // 1. Setup
    GLFWwindow* window = setupWindow(800, 600, "Lab Test: 12 Keys");
    if (!window) return -1;

    printInstructions(); // Show keys in terminal

    Shader ourShader("vertex.vs", "fragment.fs");
    Cube myCube; // We use one object definition to draw 4 times

    // 2. Loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // Camera setup
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Common Constants
        glm::vec3 arbAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
        glm::vec3 pivot = glm::vec3(0.5f, 0.5f, 0.5f);

        // ---------------------------------------------------------
        // CUBE A: GLM Standard (Top-Left)
        // ---------------------------------------------------------
        glm::mat4 modelA = glm::mat4(1.0f);
        modelA = glm::translate(modelA, glm::vec3(-2.0f, 1.5f, 0.0f)); // Base position at Y=+1.5
        modelA = glm::translate(modelA, arbAxis * tx_A); // Translate along rotation axis
        modelA = glm::rotate(modelA, glm::radians(rot_A), arbAxis);
        modelA = modelA * scaleAlongAxis(arbAxis, 1.0+sc_A); // Scale along axis
        myCube.draw(ourShader.ID, modelA);

        // ---------------------------------------------------------
        // CUBE B: Custom Standard (Top-Right)
        // ---------------------------------------------------------
        glm::mat4 modelB = myIdentity();
        modelB = myTranslate(modelB, glm::vec3(2.0f, 1.5f, 0.0f)); // Base position at Y=+1.5
        modelB = myTranslate(modelB, arbAxis * tx_B); // Translate along rotation axis
        modelB = myRotate(modelB, glm::radians(rot_B), arbAxis);
        modelB = modelB * scaleAlongAxis(arbAxis, 1.0+sc_B); // Scale along axis
        myCube.draw(ourShader.ID, modelB);

        // ---------------------------------------------------------
        // CUBE C: GLM Pivot (Bottom-Left)
        // ---------------------------------------------------------
        glm::mat4 modelC = glm::mat4(1.0f);
        modelC = glm::translate(modelC, glm::vec3(tx_C, -1.5f, 0.0f)); // Start at Y=-1.5
        // Pivot Logic
        modelC = glm::translate(modelC, pivot);
        modelC = glm::rotate(modelC, glm::radians(rot_C), glm::vec3(0.0f, 0.0f, 1.0f));
        modelC = glm::translate(modelC, -pivot);
        // Scale
        modelC = glm::scale(modelC, glm::vec3(sc_C));
        myCube.draw(ourShader.ID, modelC);

        // ---------------------------------------------------------
        // CUBE D: Custom Pivot (Bottom-Right)
        // ---------------------------------------------------------
        glm::mat4 modelD = myIdentity();
        modelD = myTranslate(modelD, glm::vec3(tx_D, -1.5f, 0.0f)); // Start at Y=-1.5
        // Pivot Logic
        modelD = myTranslate(modelD, pivot);
        modelD = myRotate(modelD, glm::radians(rot_D), glm::vec3(0.0f, 0.0f, 1.0f));
        modelD = myTranslate(modelD, -pivot);
        // Scale
        modelD = myScale(modelD, glm::vec3(sc_D));
        myCube.draw(ourShader.ID, modelD);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    myCube.cleanup();
    glfwTerminate();
    return 0;
}