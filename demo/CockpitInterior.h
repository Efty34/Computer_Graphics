#ifndef COCKPIT_INTERIOR_H
#define COCKPIT_INTERIOR_H

#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <glm/glm.hpp>
#include "Shader.h"

class CockpitInterior {
private:
    Cube cube;
    Sphere sphere;
    Cylinder cylinder;

    // Colors
    const glm::vec3 SEAT_BACK = glm::vec3(0.15f, 0.15f, 0.18f);
    const glm::vec3 SEAT_CUSHION = glm::vec3(0.25f, 0.22f, 0.2f);
    const glm::vec3 METAL_FRAME = glm::vec3(0.3f, 0.3f, 0.32f);

public:
    CockpitInterior() {}

    // Camera at (0,0,0) looking toward +X
    void draw(Shader& shader, glm::mat4 parentModel) {

        // ===== LEFT SEAT (pilot's view of co-pilot seat on left) =====
        // Seat back
        cube.draw(shader, parentModel,
            0.8f, 0.0f, 0.4f,
            0.0f, 0.0f, 0.0f,
            0.08f, 0.5f, 0.4f,
            SEAT_BACK);
        // Headrest
        cube.draw(shader, parentModel,
            0.8f, 0.35f, 0.4f,
            0.0f, 0.0f, 0.0f,
            0.06f, 0.15f, 0.2f,
            SEAT_CUSHION);

        // ===== RIGHT SEAT (pilot's view of another seat on right) =====
        // Seat back
        cube.draw(shader, parentModel,
            0.8f, 0.0f, -0.4f,
            0.0f, 0.0f, 0.0f,
            0.08f, 0.5f, 0.4f,
            SEAT_BACK);
        // Headrest
        cube.draw(shader, parentModel,
            0.8f, 0.35f, -0.4f,
            0.0f, 0.0f, 0.0f,
            0.06f, 0.15f, 0.2f,
            SEAT_CUSHION);

        // ===== FRONT WINDOW FRAME =====
        // Top horizontal bar
        cube.draw(shader, parentModel,
            1.5f, 0.6f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.05f, 0.05f, 0.65f,
            METAL_FRAME);

        // Bottom horizontal bar
        cube.draw(shader, parentModel,
            1.2f, -0.1f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.05f, 0.05f, 0.65f,
            METAL_FRAME);

        // Left front corner pillar (A-pillar)
        cube.draw(shader, parentModel,
            1.35f, 0.25f, 0.6f,
            0.0f, 0.0f, -15.0f,
            0.05f, 0.5f, 0.05f,
            METAL_FRAME);

        // Right front corner pillar (A-pillar)
        cube.draw(shader, parentModel,
            1.35f, 0.25f, -0.6f,
            0.0f, 0.0f, 15.0f,
            0.05f, 0.5f, 0.05f,
            METAL_FRAME);

        // Center vertical strut
        cube.draw(shader, parentModel,
            1.35f, 0.25f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.03f, 0.45f, 0.03f,
            METAL_FRAME);

        // ===== LEFT SIDE WINDOW FRAME =====
        // Left side top bar (connects to A-pillar)
        cube.draw(shader, parentModel,
            0.6f, 0.45f, 0.6f,
            0.0f, 0.0f, 0.0f,
            0.8f, 0.04f, 0.04f,
            METAL_FRAME);
        // Left side bottom bar
        cube.draw(shader, parentModel,
            0.6f, -0.1f, 0.6f,
            0.0f, 0.0f, 0.0f,
            0.8f, 0.04f, 0.04f,
            METAL_FRAME);
        // Left side rear vertical (B-pillar)
        cube.draw(shader, parentModel,
            -0.2f, 0.17f, 0.6f,
            0.0f, 0.0f, 0.0f,
            0.04f, 0.35f, 0.04f,
            METAL_FRAME);

        // ===== RIGHT SIDE WINDOW FRAME =====
        // Right side top bar (connects to A-pillar)
        cube.draw(shader, parentModel,
            0.6f, 0.45f, -0.6f,
            0.0f, 0.0f, 0.0f,
            0.8f, 0.04f, 0.04f,
            METAL_FRAME);
        // Right side bottom bar
        cube.draw(shader, parentModel,
            0.6f, -0.1f, -0.6f,
            0.0f, 0.0f, 0.0f,
            0.8f, 0.04f, 0.04f,
            METAL_FRAME);
        // Right side rear vertical (B-pillar)
        cube.draw(shader, parentModel,
            -0.2f, 0.17f, -0.6f,
            0.0f, 0.0f, 0.0f,
            0.04f, 0.35f, 0.04f,
            METAL_FRAME);
    }
};

#endif
