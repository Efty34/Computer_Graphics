#ifndef MONITOR_H
#define MONITOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Cube.h"

class Monitor {
public:
    Cube cube;

    // Monitor colors
    glm::vec3 standColor;
    glm::vec3 bezelColor;
    glm::vec3 screenColor;

    Monitor() {
        standColor = glm::vec3(0.18f, 0.18f, 0.2f);   // Dark gunmetal stand
        bezelColor = glm::vec3(0.08f, 0.08f, 0.1f);   // Near-black bezel
        screenColor = glm::vec3(0.15f, 0.35f, 0.55f); // Blue-ish screen (like desktop wallpaper)
    }

    Monitor(glm::vec3 stand, glm::vec3 bezel, glm::vec3 screen) {
        standColor = stand;
        bezelColor = bezel;
        screenColor = screen;
    }

    void draw(Shader& shader, glm::mat4 parentModel, float tx, float ty, float tz,
        float rx = 0.0f, float ry = 0.0f, float rz = 0.0f) {

        glm::mat4 monitorBase = glm::translate(parentModel, glm::vec3(tx, ty, tz));
        monitorBase = glm::rotate(monitorBase, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
        monitorBase = glm::rotate(monitorBase, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
        monitorBase = glm::rotate(monitorBase, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

        // Base/Stand bottom - wider oval-ish base
        cube.draw(shader, monitorBase, -0.1f, 0.0f, -0.05f, 0.0f, 0.0f, 0.0f,
            0.25f, 0.02f, 0.18f, standColor);

        // Vertical pole/neck
        cube.draw(shader, monitorBase, 0.0f, 0.02f, 0.02f, 0.0f, 0.0f, 0.0f,
            0.05f, 0.28f, 0.05f, standColor);

        // Back panel/frame (outer bezel)
        cube.draw(shader, monitorBase, -0.24f, 0.30f, -0.01f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.36f, 0.025f, bezelColor);

        // Screen display area (slightly inset from bezel)
        cube.draw(shader, monitorBase, -0.22f, 0.32f, -0.012f, 0.0f, 0.0f, 0.0f,
            0.46f, 0.30f, 0.015f, screenColor);

        // Bottom bezel strip (thicker chin)
        cube.draw(shader, monitorBase, -0.24f, 0.30f, -0.012f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.025f, 0.02f, bezelColor);

        // Power LED indicator (small green dot)
        cube.draw(shader, monitorBase, 0.0f, 0.305f, -0.015f, 0.0f, 0.0f, 0.0f,
            0.015f, 0.015f, 0.005f, glm::vec3(0.2f, 0.8f, 0.2f));
    }
};

#endif
