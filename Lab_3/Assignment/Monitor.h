#ifndef MONITOR_H
#define MONITOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Monitor {
public:
    Cube cube;

    Monitor() {}

    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float rx = 0.0f, float ry = 0.0f, float rz = 0.0f) {

        glm::mat4 monitorBase = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        monitorBase = glm::rotate(monitorBase, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        monitorBase = glm::rotate(monitorBase, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        monitorBase = glm::rotate(monitorBase, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

        // Base/Stand bottom
        cube.draw(shader, monitorBase, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.2f, 0.02f, 0.15f, glm::vec3(0.2f, 0.2f, 0.2f));

        // vertical pole
        cube.draw(shader, monitorBase, 0.0f, 0.02f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.04f, 0.3f, 0.04f, glm::vec3(0.2f, 0.2f, 0.2f));

        // Screen
        cube.draw(shader, monitorBase, -0.22f, 0.32f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.45f, 0.32f, 0.02f, glm::vec3(0.05f, 0.05f, 0.15f));

        //  frame/bezel
        cube.draw(shader, monitorBase, -0.23f, 0.31f, -0.01f, 0.0f, 0.0f, 0.0f,
            0.47f, 0.34f, 0.01f, glm::vec3(0.15f, 0.15f, 0.15f));
    }
};

#endif
