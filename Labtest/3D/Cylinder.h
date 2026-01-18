#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Cylinder {
public:
    unsigned int VAO, VBO, EBO;
    int indexCount;

    Cylinder(float radius = 0.5f, float height = 1.0f, int segments = 36) {
        setupMesh(radius, height, segments);
    }

    void draw(unsigned int shaderProgram, glm::mat4 modelMatrix) {
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    void setupMesh(float radius, float height, int segments) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float halfHeight = height / 2.0f;

        // Center vertices for top and bottom caps
        // Top center (0)
        vertices.insert(vertices.end(), {0.0f, halfHeight, 0.0f, 1.0f, 0.0f, 0.0f}); // Red
        // Bottom center (1)
        vertices.insert(vertices.end(), {0.0f, -halfHeight, 0.0f, 0.0f, 1.0f, 0.0f}); // Green

        // Generate circle vertices for top and bottom
        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / (float)segments * 2.0f * M_PI;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Color varies around the circle
            float r = 0.5f + 0.5f * cos(angle);
            float g = 0.5f + 0.5f * sin(angle);
            float b = 0.5f;

            // Top circle vertex
            vertices.insert(vertices.end(), {x, halfHeight, z, r, g, b});
            
            // Bottom circle vertex
            vertices.insert(vertices.end(), {x, -halfHeight, z, r, g, b});
        }

        // Indices for top cap (triangle fan)
        for (int i = 0; i < segments; ++i) {
            indices.push_back(0);                   // Top center
            indices.push_back(2 + i * 2);           // Current top vertex
            indices.push_back(2 + (i + 1) * 2);     // Next top vertex
        }

        // Indices for bottom cap (triangle fan)
        for (int i = 0; i < segments; ++i) {
            indices.push_back(1);                   // Bottom center
            indices.push_back(3 + (i + 1) * 2);     // Next bottom vertex
            indices.push_back(3 + i * 2);           // Current bottom vertex
        }

        // Indices for side surface (quads as two triangles)
        for (int i = 0; i < segments; ++i) {
            int topCurrent = 2 + i * 2;
            int topNext = 2 + (i + 1) * 2;
            int bottomCurrent = 3 + i * 2;
            int bottomNext = 3 + (i + 1) * 2;

            // First triangle
            indices.push_back(topCurrent);
            indices.push_back(bottomCurrent);
            indices.push_back(topNext);

            // Second triangle
            indices.push_back(topNext);
            indices.push_back(bottomCurrent);
            indices.push_back(bottomNext);
        }

        indexCount = indices.size();

        // Create buffers and upload data
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

#endif
