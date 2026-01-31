#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Sphere {
public:
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18) {
        setUpSphere(radius, sectorCount, stackCount);
    }

    ~Sphere() {
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

    void setUpSphere(float radius, int sectorCount, int stackCount) {
        float x, y, z, xy;                              // vertex position
        float PI = 3.14159265359f;
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                
                // Add vertex data
                vertices.push_back(x);
                vertices.push_back(z); // Swapped y and z to make Poles aligned with Y axis usually, but here z is up in math, let's keep standard OpenGL
                // Wait, in OpenGL Y is usually up. Let's align poles to Y.
                // Current: xy is horizon radius. z is height. 
                // Let's swap to: x, z (horizon), y (height)
                // vertices.push_back(x); vertices.push_back(z); vertices.push_back(y); 
                // Actually the math above: z = r*sin(stack). z is height. 
                // So Y-up means we map z->y.
                
                // Let's restart mapping for Y-UP
                // x = xy * cos(sector)
                // z = xy * sin(sector)
                // y = radius * sin(stack)
            }
        }
        
        // Clear and redo with Y-up standard
        vertices.clear();
        for(int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for(int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;
                x = xy * cosf(sectorAngle);
                z = xy * sinf(sectorAngle);
                
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        // Indices
        int k1, k2;
        for (int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
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
