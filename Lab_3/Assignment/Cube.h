#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

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

        shader.setMat4("model", m);
        
        // Set material properties based on color
        shader.setVec3("material.ambient", colorVec * 0.3f);
        shader.setVec3("material.diffuse", colorVec);
        shader.setVec3("material.specular", glm::vec3(0.3f, 0.3f, 0.3f));
        shader.setFloat("material.shininess", 32.0f);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int cubeVAO, cubeVBO, cubeEBO;

    void setUpCube() {
        // Cube vertices with positions and normals
        // Each face has 4 vertices with the same normal
        float vertices[] = {
            // Position          // Normal
            // Front face (z = 0, normal pointing -Z)
            0.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            0.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            
            // Back face (z = 1, normal pointing +Z)
            0.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
            1.0f, 0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
            1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
            0.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
            
            // Bottom face (y = 0, normal pointing -Y)
            0.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
            1.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            
            // Top face (y = 1, normal pointing +Y)
            0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
            1.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
            1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
            0.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
            
            // Left face (x = 0, normal pointing -X)
            0.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
            0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
            0.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
            0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
            
            // Right face (x = 1, normal pointing +X)
            1.0f, 0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
            1.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
            1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
            1.0f, 1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        };
        
        // Indices for each face (6 faces * 2 triangles * 3 vertices)
        unsigned int indices[] = {
            // Front face
            0, 1, 2,  2, 3, 0,
            // Back face
            4, 6, 5,  6, 4, 7,
            // Bottom face
            8, 9, 10,  10, 11, 8,
            // Top face
            12, 14, 13,  14, 12, 15,
            // Left face
            16, 17, 18,  18, 19, 16,
            // Right face
            20, 22, 21,  22, 20, 23
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);

        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};
#endif