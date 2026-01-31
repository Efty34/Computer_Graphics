#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Boundary {
public:
    Cube cube;

    // Room dimensions
    float roomWidth;
    float roomDepth;
    float roomHeight;
    float wallThickness;
    glm::vec3 wallColor;
    glm::vec3 floorColor;

    // Default constructor
    Boundary() {
        roomWidth = 10.0f;
        roomDepth = 10.0f;
        roomHeight = 4.0f;
        wallThickness = 0.1f;
        wallColor = glm::vec3(0.65f, 0.67f, 0.68f); // Concrete gray for walls
        floorColor = glm::vec3(0.55f, 0.35f, 0.2f); // Warm brown wooden floor
    }

    // Constructor with custom dimensions and colors
    Boundary(float width, float depth, float height,
        glm::vec3 wColor = glm::vec3(0.7f, 0.8f, 0.9f),
        glm::vec3 fColor = glm::vec3(0.5f, 0.5f, 0.5f)) {
        roomWidth = width;
        roomDepth = depth;
        roomHeight = height;
        wallThickness = 0.1f;
        wallColor = wColor;
        floorColor = fColor;
    }

    // Draw floor
    void drawFloor(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        float halfWidth = roomWidth / 2.0f;
        float halfDepth = roomDepth / 2.0f;
        cube.draw(shader, parentModel, -halfWidth, 0.0f, -halfDepth,
            0.0f, 0.0f, 0.0f,
            roomWidth, wallThickness, roomDepth, floorColor);
    }

    // Draw back wall
    void drawBackWall(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        float halfWidth = roomWidth / 2.0f;
        float halfDepth = roomDepth / 2.0f;
        cube.draw(shader, parentModel, -halfWidth, 0.0f, -halfDepth,
            0.0f, 0.0f, 0.0f,
            roomWidth, roomHeight, wallThickness, wallColor);
    }

    // Draw left wall
    void drawLeftWall(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        float halfWidth = roomWidth / 2.0f;
        float halfDepth = roomDepth / 2.0f;
        cube.draw(shader, parentModel, -halfWidth, 0.0f, -halfDepth,
            0.0f, 0.0f, 0.0f,
            wallThickness, roomHeight, roomDepth, wallColor);
    }

    // Draw right wall
    void drawRightWall(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        float halfWidth = roomWidth / 2.0f;
        float halfDepth = roomDepth / 2.0f;
        cube.draw(shader, parentModel, halfWidth - wallThickness, 0.0f, -halfDepth,
            0.0f, 0.0f, 0.0f,
            wallThickness, roomHeight, roomDepth, wallColor);
    }

    // Draw all walls (back, left, right)
    void drawWalls(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        drawBackWall(shader, parentModel);
        drawLeftWall(shader, parentModel);
        drawRightWall(shader, parentModel);
    }

    // Draw complete room (floor + walls)
    void drawRoom(Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f)) {
        drawFloor(shader, parentModel);
        drawWalls(shader, parentModel);
    }
};

#endif
