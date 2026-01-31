#ifndef WEDGE_H
#define WEDGE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

// A triangular prism / wedge shape - great for futuristic angular designs
class Wedge {
public:
    Wedge() {
        setUpWedge();
    }

    ~Wedge() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, glm::vec3 colorVec) {
        glm::mat4 t = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        glm::mat4 rX = glm::rotate(t, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rY = glm::rotate(rX, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rZ = glm::rotate(rY, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 m = glm::scale(rZ, glm::vec3(sx, sy, sz));

        shader.setMat4("model", m);
        shader.setVec3("customColor", colorVec);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;

    void setUpWedge() {
        // Triangular prism centered at origin
        // Triangle in XY plane, extruded along Z
        float vertices[] = {
            // Front triangle (z = -0.5)
            -0.5f, -0.5f, -0.5f,  // 0: bottom left
             0.5f, -0.5f, -0.5f,  // 1: bottom right
             0.0f,  0.5f, -0.5f,  // 2: top center
            // Back triangle (z = 0.5)
            -0.5f, -0.5f,  0.5f,  // 3: bottom left
             0.5f, -0.5f,  0.5f,  // 4: bottom right
             0.0f,  0.5f,  0.5f   // 5: top center
        };

        unsigned int indices[] = {
            // Front face
            0, 1, 2,
            // Back face
            3, 5, 4,
            // Bottom face
            0, 3, 4, 4, 1, 0,
            // Left face
            0, 2, 5, 5, 3, 0,
            // Right face
            1, 4, 5, 5, 2, 1
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};

#endif
