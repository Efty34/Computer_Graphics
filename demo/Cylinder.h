#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "Shader.h"

class Cylinder {
public:
    Cylinder(float baseRadius = 0.5f, float topRadius = 0.5f, float height = 1.0f, int sectorCount = 36) {
        setUpCylinder(baseRadius, topRadius, height, sectorCount);
    }

    ~Cylinder() {
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
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void setUpCylinder(float baseRadius, float topRadius, float height, int sectorCount) {
        float const PI = 3.14159265359f;
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;

        std::vector<float> unitCircleVertices;
        for(int i = 0; i <= sectorCount; ++i) {
            sectorAngle = i * sectorStep;
            unitCircleVertices.push_back(cos(sectorAngle)); // x
            unitCircleVertices.push_back(sin(sectorAngle)); // z
        }

        // Put vertices of side cylinder
        for(int i = 0; i < 2; ++i) {
            float h = -height / 2.0f + i * height;           // -h/2 to h/2
            float t = 1.0f - i;                              // top to bottom
            float r = (i==0) ? baseRadius : topRadius;      // 0=bottom, 1=top

            for(int j = 0; j <= sectorCount; ++j) {
                float ux = unitCircleVertices[2*j];
                float uz = unitCircleVertices[2*j+1];
                
                // Position
                vertices.push_back(ux * r);
                vertices.push_back(h);
                vertices.push_back(uz * r);
            }
        }

        // Base center vertex
        int baseCenterIndex = vertices.size() / 3;
        vertices.push_back(0); vertices.push_back(-height/2.0f); vertices.push_back(0);

        // Top center vertex
        int topCenterIndex = vertices.size() / 3;
        vertices.push_back(0); vertices.push_back(height/2.0f); vertices.push_back(0);

        // Indices
        int k1 = 0;
        int k2 = sectorCount + 1;

        // Side indices
        for(int i = 0; i < sectorCount; ++i) {
            indices.push_back(k1 + i);
            indices.push_back(k2 + i);
            indices.push_back(k1 + i + 1);

            indices.push_back(k2 + i);
            indices.push_back(k2 + i + 1);
            indices.push_back(k1 + i + 1);
        }

        // Bottom Cap
        for(int i = 0; i < sectorCount; ++i) {
             indices.push_back(baseCenterIndex);
             indices.push_back(i + 1);
             indices.push_back(i);
        }

        // Top Cap
        // Vertices for top circle start at k2
        for(int i = 0; i < sectorCount; ++i) {
             indices.push_back(topCenterIndex);
             indices.push_back(k2 + i);
             indices.push_back(k2 + i + 1);
        }

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
    }
};

#endif
