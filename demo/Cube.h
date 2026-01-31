#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Cube {
public:
    glm::vec3 color;
    glm::mat4 model;

    Cube() {
        setUpCube();
        model = glm::mat4(1.0f);
        color = glm::vec3(1.0f);
    }

    ~Cube() {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    // Draw with explicit transformations
    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, glm::vec3 colorVec) {
        glm::mat4 t = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        glm::mat4 rX = glm::rotate(t, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rY = glm::rotate(rX, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rZ = glm::rotate(rY, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 m = glm::scale(rZ, glm::vec3(sx, sy, sz));

        // Center the cube so scaling/rotation happens around the center
        // Original vertices are 0.0 to 0.5. We translate by -0.25 to center it at 0,0,0
        // But for walls/floors, corner pivot is often easier. I will keep your original corner pivot (0,0,0) logic
        // and just pass the model matrix.

        shader.setMat4("model", m);
        shader.setVec3("customColor", colorVec); // We will add this uniform to shader

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int cubeVAO, cubeVBO, cubeEBO;

    void setUpCube() {
        // Centered Unit Cube Vertices (-0.5 to 0.5)
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f
        };
        // Reuse indices for a cube
        unsigned int indices[] = {
            0, 3, 2, 2, 1, 0, // Front
            4, 5, 6, 6, 7, 4, // Back
            4, 0, 1, 1, 5, 4, // Bottom
            3, 7, 6, 6, 2, 3, // Top
            4, 7, 3, 3, 0, 4, // Left
            1, 2, 6, 6, 5, 1  // Right
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);

        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};
#endif