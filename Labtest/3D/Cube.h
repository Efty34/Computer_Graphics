#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Cube {
public:
    unsigned int VAO, VBO, EBO;

    Cube() {
        setupMesh();
    }

    // Pass the Shader Program ID and the Model Matrix specific to this cube
    void draw(unsigned int shaderProgram, glm::mat4 modelMatrix) {
        // 1. Send the model matrix to the shader
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);

        // 2. Bind VAO and Draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 3. Unbind (Good practice)
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    void setupMesh() {
        // Format: x, y, z, r, g, b
        float vertices[] = {
            // Front face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-left (Red)
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Bottom-right (Green)
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Top-right (Blue)
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Top-left (Yellow)
            // Back face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Bottom-left (Magenta)
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Bottom-right (Cyan)
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // Top-right (White)
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f  // Top-left (Black)
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0, // Front
            4, 5, 6, 6, 7, 4, // Back
            7, 3, 0, 0, 4, 7, // Left
            6, 2, 1, 1, 5, 6, // Right
            0, 1, 5, 5, 4, 0, // Bottom
            3, 2, 6, 6, 7, 3  // Top
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Attribute 0: Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Attribute 1: Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif