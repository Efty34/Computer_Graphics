#ifndef LAMP_H
#define LAMP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Lamp {
public:
    Cube cube; 

    // Lamp dimensions
    float baseSize;
    float standHeight;
    float standThickness;
    float armLength;
    float headWidth;
    float headHeight;
    float headDepth;
    glm::vec3 baseColor;
    glm::vec3 standColor;
    glm::vec3 headColor;

    Lamp() {
        baseSize = 0.2f;
        standHeight = 0.6f;
        standThickness = 0.03f;
        armLength = 0.3f;
        headWidth = 0.25f;
        headHeight = 0.15f;
        headDepth = 0.2f;
        baseColor = glm::vec3(0.2f, 0.2f, 0.2f); 
        standColor = glm::vec3(0.4f, 0.4f, 0.4f); 
        headColor = glm::vec3(1.0f, 0.9f, 0.7f); 
    }

    Lamp(float base, float height, float headW, float headH,
        glm::vec3 baseCol = glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3 standCol = glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3 headCol = glm::vec3(1.0f, 0.9f, 0.7f)) {
        baseSize = base;
        standHeight = height;
        standThickness = 0.03f;
        armLength = 0.3f;
        headWidth = headW;
        headHeight = headH;
        headDepth = headW * 0.8f;
        baseColor = baseCol;
        standColor = standCol;
        headColor = headCol;
    }

    
    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float lampHeadRotation = 0.0f, float armRotation = 0.0f) {


        glm::mat4 lampBase = glm::translate(parentModel, glm::vec3(tx, ty, tz));

        cube.draw(shader, lampBase, -baseSize / 2.0f, 0.0f, -baseSize / 2.0f,
            0.0f, 0.0f, 0.0f,
            baseSize, 0.05f, baseSize, baseColor);

        cube.draw(shader, lampBase, -standThickness / 2.0f, 0.05f, -standThickness / 2.0f,
            0.0f, 0.0f, 0.0f,
            standThickness, standHeight, standThickness, standColor);
        
        glm::mat4 armBase = glm::translate(lampBase, glm::vec3(0.0f, 0.05f + standHeight, 0.0f));
        armBase = glm::rotate(armBase, glm::radians(armRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        cube.draw(shader, armBase, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -45.0f,
            armLength, standThickness, standThickness, standColor);

        float armEndX = armLength * glm::cos(glm::radians(-45.0f));
        float armEndY = armLength * glm::sin(glm::radians(-45.0f));

        glm::mat4 headBase = glm::translate(lampBase, glm::vec3(0.0f, 0.05f + standHeight, 0.0f));
        headBase = glm::rotate(headBase, glm::radians(armRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        headBase = glm::translate(headBase, glm::vec3(armEndX, armEndY, 0.0f));

        headBase = glm::rotate(headBase, glm::radians(lampHeadRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        cube.draw(shader, headBase, -headWidth / 2.0f, 0.0f, -headDepth / 2.0f,
            0.0f, 0.0f, 0.0f,
            headWidth, headHeight, headDepth, headColor);


        glm::vec3 bulbColor = glm::vec3(1.0f, 1.0f, 0.5f);
        cube.draw(shader, headBase, -headWidth * 0.2f / 2.0f, headHeight * 0.3f, -headDepth * 0.2f / 2.0f,
            0.0f, 0.0f, 0.0f,
            headWidth * 0.2f, headHeight * 0.3f, headDepth * 0.2f, bulbColor);
    }

    void draw(Shader& shader, glm::vec3 position, float lampHeadRotation = 0.0f, float armRotation = 0.0f) {
        glm::mat4 identity = glm::mat4(1.0f);
        draw(shader, identity, position.x, position.y, position.z, lampHeadRotation, armRotation);
    }
};

#endif
