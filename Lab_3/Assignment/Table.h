#ifndef TABLE_H
#define TABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Table {
public:
    Cube cube;

    // Table dimensions
    float topWidth;
    float topHeight;
    float topDepth;
    float legThickness;
    float legHeight;
    glm::vec3 color;

    Table() {
        topWidth = 1.5f;
        topHeight = 0.1f;
        topDepth = 1.0f;
        legThickness = 0.1f;
        legHeight = 0.8f;
        color = glm::vec3(0.55f, 0.35f, 0.18f); // Warm oak wood color
    }

    // constructor with custom dimensions
    Table(float width, float depth, float height, glm::vec3 tableColor = glm::vec3(0.6f, 0.4f, 0.2f)) {
        topWidth = width;
        topHeight = 0.1f;
        topDepth = depth;
        legThickness = 0.1f;
        legHeight = height;
        color = tableColor;
    }

    // Draw the complete table with transformations
    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float rx = 0.0f, float ry = 0.0f, float rz = 0.0f) {

        // Create base transformation for the entire table
        glm::mat4 tableBase = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        tableBase = glm::rotate(tableBase, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        tableBase = glm::rotate(tableBase, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        tableBase = glm::rotate(tableBase, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

        // Draw table top
        cube.draw(shader, tableBase, 0.0f, legHeight, 0.0f,
            0.0f, 0.0f, 0.0f,
            topWidth, topHeight, topDepth, color);

        // Draw four legs (positioned at corners with small inset)
        float inset = 0.05f;

        // Front-left leg
        cube.draw(shader, tableBase, inset, 0.0f, inset,
            0.0f, 0.0f, 0.0f,
            legThickness, legHeight, legThickness, color);

        // Front-right leg
        cube.draw(shader, tableBase, topWidth - inset - legThickness, 0.0f, inset,
            0.0f, 0.0f, 0.0f,
            legThickness, legHeight, legThickness, color);

        // Back-left leg
        cube.draw(shader, tableBase, inset, 0.0f, topDepth - inset - legThickness,
            0.0f, 0.0f, 0.0f,
            legThickness, legHeight, legThickness, color);

        // Back-right leg
        cube.draw(shader, tableBase, topWidth - inset - legThickness, 0.0f, topDepth - inset - legThickness,
            0.0f, 0.0f, 0.0f,
            legThickness, legHeight, legThickness, color);
    }

    // Alternative draw method with separate position and rotation vectors
    void draw(Shader& shader, glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f)) {
        glm::mat4 identity = glm::mat4(1.0f);
        draw(shader, identity, position.x, position.y, position.z,
            rotation.x, rotation.y, rotation.z);
    }
};

#endif
