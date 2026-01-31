#ifndef CHAIR_H
#define CHAIR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Chair {
public:
    Cube cube; // Use the existing Cube class for rendering

    // Chair dimensions (can be customized)
    float seatWidth;
    float seatDepth;
    float seatHeight;
    float seatThickness;
    float backrestHeight;
    float backrestThickness;
    float legThickness;
    glm::vec3 color;

    // Default constructor with standard chair dimensions
    Chair() {
        seatWidth = 0.5f;
        seatDepth = 0.5f;
        seatHeight = 0.45f;
        seatThickness = 0.05f;
        backrestHeight = 0.5f;
        backrestThickness = 0.05f;
        legThickness = 0.05f;
        color = glm::vec3(0.45f, 0.28f, 0.15f); // Warm walnut wood color
    }

    // Constructor with custom dimensions and color
    Chair(float width, float depth, float height, glm::vec3 chairColor = glm::vec3(0.4f, 0.3f, 0.2f)) {
        seatWidth = width;
        seatDepth = depth;
        seatHeight = height;
        seatThickness = 0.05f;
        backrestHeight = height * 1.1f; // Backrest is slightly taller
        backrestThickness = 0.05f;
        legThickness = 0.05f;
        color = chairColor;
    }

    // Draw the complete chair with transformations
    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float rx = 0.0f, float ry = 0.0f, float rz = 0.0f) {

        // Create base transformation for the entire chair
        glm::mat4 chairBase = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        chairBase = glm::rotate(chairBase, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        chairBase = glm::rotate(chairBase, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        chairBase = glm::rotate(chairBase, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

        // Draw seat
        cube.draw(shader, chairBase, 0.0f, seatHeight, 0.0f,
            0.0f, 0.0f, 0.0f,
            seatWidth, seatThickness, seatDepth, color);

        // Draw backrest (at the back of the seat)
        cube.draw(shader, chairBase, 0.0f, seatHeight + seatThickness, seatDepth - backrestThickness,
            0.0f, 0.0f, 0.0f,
            seatWidth, backrestHeight, backrestThickness, color);

        // Draw four legs (positioned at corners with small inset)
        float inset = 0.02f;

        // Front-left leg
        cube.draw(shader, chairBase, inset, 0.0f, inset,
            0.0f, 0.0f, 0.0f,
            legThickness, seatHeight, legThickness, color);

        // Front-right leg
        cube.draw(shader, chairBase, seatWidth - inset - legThickness, 0.0f, inset,
            0.0f, 0.0f, 0.0f,
            legThickness, seatHeight, legThickness, color);

        // Back-left leg (extends to support backrest)
        cube.draw(shader, chairBase, inset, 0.0f, seatDepth - inset - legThickness,
            0.0f, 0.0f, 0.0f,
            legThickness, seatHeight + seatThickness + backrestHeight, legThickness, color);

        // Back-right leg (extends to support backrest)
        cube.draw(shader, chairBase, seatWidth - inset - legThickness, 0.0f, seatDepth - inset - legThickness,
            0.0f, 0.0f, 0.0f,
            legThickness, seatHeight + seatThickness + backrestHeight, legThickness, color);
    }

    // Alternative draw method with separate position and rotation vectors
    void draw(Shader& shader, glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f)) {
        glm::mat4 identity = glm::mat4(1.0f);
        draw(shader, identity, position.x, position.y, position.z,
            rotation.x, rotation.y, rotation.z);
    }
};

#endif
