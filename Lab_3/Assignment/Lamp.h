#ifndef LAMP_H
#define LAMP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Lamp {
public:
    Cube cube;

    // Ceiling lamp dimensions
    float mountingPlateSize;
    float rodLength;
    float rodThickness;
    float shadeWidth;
    float shadeHeight;
    float shadeDepth;
    glm::vec3 mountingColor;
    glm::vec3 rodColor;
    glm::vec3 shadeColor;

    Lamp() {
        mountingPlateSize = 0.3f;
        rodLength = 0.4f;
        rodThickness = 0.05f;
        shadeWidth = 0.5f;
        shadeHeight = 0.3f;
        shadeDepth = 0.5f;
        mountingColor = glm::vec3(0.2f, 0.2f, 0.2f);
        rodColor = glm::vec3(0.3f, 0.3f, 0.3f);
        shadeColor = glm::vec3(1.0f, 0.9f, 0.7f);
    }

    Lamp(float plateSize, float rod, float shadeW, float shadeH,
        glm::vec3 mountCol = glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3 rodCol = glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3 shadeCol = glm::vec3(1.0f, 0.9f, 0.7f)) {
        mountingPlateSize = plateSize;
        rodLength = rod;
        rodThickness = 0.05f;
        shadeWidth = shadeW;
        shadeHeight = shadeH;
        shadeDepth = shadeW;
        mountingColor = mountCol;
        rodColor = rodCol;
        shadeColor = shadeCol;
    }

    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float lampRotation = 0.0f, float swingAngle = 0.0f) {

        // Ceiling mount base - mounted at the ceiling position
        glm::mat4 ceilingMount = glm::translate(parentModel, glm::vec3(tx, ty, tz));

        // Mounting plate on ceiling
        cube.draw(shader, ceilingMount, -mountingPlateSize / 2.0f, 0.0f, -mountingPlateSize / 2.0f,
            0.0f, 0.0f, 0.0f,
            mountingPlateSize, 0.05f, mountingPlateSize, mountingColor);

        // Apply swing rotation at the mounting point
        glm::mat4 swingBase = glm::rotate(ceilingMount, glm::radians(swingAngle), glm::vec3(1.0f, 0.0f, 0.0f));

        // Hanging rod (goes downward from ceiling)
        cube.draw(shader, swingBase, -rodThickness / 2.0f, -rodLength, -rodThickness / 2.0f,
            0.0f, 0.0f, 0.0f,
            rodThickness, rodLength, rodThickness, rodColor);

        // Lamp shade at the bottom of the rod
        glm::mat4 shadeBase = glm::translate(swingBase, glm::vec3(0.0f, -rodLength, 0.0f));
        shadeBase = glm::rotate(shadeBase, glm::radians(lampRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Outer shade (lampshade housing)
        cube.draw(shader, shadeBase, -shadeWidth / 2.0f, -shadeHeight, -shadeDepth / 2.0f,
            0.0f, 0.0f, 0.0f,
            shadeWidth, shadeHeight, shadeDepth, shadeColor);

        // Light bulb inside the shade
        glm::vec3 bulbColor = glm::vec3(1.0f, 1.0f, 0.5f);
        cube.draw(shader, shadeBase, -shadeWidth * 0.15f / 2.0f, -shadeHeight * 0.5f, -shadeDepth * 0.15f / 2.0f,
            0.0f, 0.0f, 0.0f,
            shadeWidth * 0.15f, shadeHeight * 0.4f, shadeDepth * 0.15f, bulbColor);
    }

    void draw(Shader& shader, glm::vec3 position, float lampRotation = 0.0f, float swingAngle = 0.0f) {
        glm::mat4 identity = glm::mat4(1.0f);
        draw(shader, identity, position.x, position.y, position.z, lampRotation, swingAngle);
    }
};

#endif
