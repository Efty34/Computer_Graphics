#ifndef HEXAGON_H
#define HEXAGON_H

#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

// Hexagonal prism - for futuristic tech panels and structures
class Hexagon {
public:
    Hexagon() {
        setUpHexagon();
    }

    ~Hexagon() {
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
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    int indexCount;

    void setUpHexagon() {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        
        const float PI = 3.14159265359f;
        const int sides = 6;
        const float height = 1.0f;
        
        // Top center
        vertices.push_back(0.0f);
        vertices.push_back(height / 2.0f);
        vertices.push_back(0.0f);
        
        // Bottom center
        vertices.push_back(0.0f);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(0.0f);
        
        // Top ring vertices (index 2 to 7)
        for (int i = 0; i < sides; i++) {
            float angle = 2.0f * PI * i / sides;
            vertices.push_back(0.5f * cos(angle));
            vertices.push_back(height / 2.0f);
            vertices.push_back(0.5f * sin(angle));
        }
        
        // Bottom ring vertices (index 8 to 13)
        for (int i = 0; i < sides; i++) {
            float angle = 2.0f * PI * i / sides;
            vertices.push_back(0.5f * cos(angle));
            vertices.push_back(-height / 2.0f);
            vertices.push_back(0.5f * sin(angle));
        }
        
        // Top cap indices
        for (int i = 0; i < sides; i++) {
            indices.push_back(0);
            indices.push_back(2 + i);
            indices.push_back(2 + (i + 1) % sides);
        }
        
        // Bottom cap indices
        for (int i = 0; i < sides; i++) {
            indices.push_back(1);
            indices.push_back(8 + (i + 1) % sides);
            indices.push_back(8 + i);
        }
        
        // Side faces
        for (int i = 0; i < sides; i++) {
            int top1 = 2 + i;
            int top2 = 2 + (i + 1) % sides;
            int bot1 = 8 + i;
            int bot2 = 8 + (i + 1) % sides;
            
            indices.push_back(top1);
            indices.push_back(bot1);
            indices.push_back(bot2);
            
            indices.push_back(top1);
            indices.push_back(bot2);
            indices.push_back(top2);
        }
        
        indexCount = indices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};

#endif
