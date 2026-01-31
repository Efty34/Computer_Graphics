#ifndef SHIP_H
#define SHIP_H

#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Wedge.h"
#include "Hexagon.h"
#include "ShipConfig.h"
#include <glm/glm.hpp>
#include "Shader.h"

class Ship {
private:
    Cube cube;
    Sphere sphere;
    Cylinder cylinder;
    Cylinder cone;
    Wedge wedge;
    Hexagon hexagon;

public:
    Ship() : sphere(0.5f, 36, 18), cylinder(0.5f, 0.5f, 1.0f, 36), cone(0.5f, 0.0f, 1.0f, 36) {}

    void draw(Shader& shader, glm::mat4 parentModel) {
        using namespace ShipConfig;
        
        // ============== MAIN FUSELAGE ==============
        hexagon.draw(shader, parentModel,
            Fuselage::MAIN_POS.x, Fuselage::MAIN_POS.y, Fuselage::MAIN_POS.z,
            Fuselage::MAIN_ROT.x, Fuselage::MAIN_ROT.y, Fuselage::MAIN_ROT.z,
            Fuselage::MAIN_SCALE.x, Fuselage::MAIN_SCALE.y, Fuselage::MAIN_SCALE.z,
            Colors::DARK_GUNMETAL);

        cube.draw(shader, parentModel,
            Fuselage::ARMOR_POS.x, Fuselage::ARMOR_POS.y, Fuselage::ARMOR_POS.z,
            Fuselage::ARMOR_ROT.x, Fuselage::ARMOR_ROT.y, Fuselage::ARMOR_ROT.z,
            Fuselage::ARMOR_SCALE.x, Fuselage::ARMOR_SCALE.y, Fuselage::ARMOR_SCALE.z,
            Colors::ARMOR_PLATE);

        // ============== NOSE SECTION ==============
        cone.draw(shader, parentModel,
            Nose::CONE_POS.x, Nose::CONE_POS.y, Nose::CONE_POS.z,
            Nose::CONE_ROT.x, Nose::CONE_ROT.y, Nose::CONE_ROT.z,
            Nose::CONE_SCALE.x, Nose::CONE_SCALE.y, Nose::CONE_SCALE.z,
            Colors::NOSE_TIP);

        cylinder.draw(shader, parentModel,
            Nose::RING_POS.x, Nose::RING_POS.y, Nose::RING_POS.z,
            Nose::RING_ROT.x, Nose::RING_ROT.y, Nose::RING_ROT.z,
            Nose::RING_SCALE.x, Nose::RING_SCALE.y, Nose::RING_SCALE.z,
            Colors::CYAN_ACCENT);

        // ============== COCKPIT ==============
        sphere.draw(shader, parentModel,
            Cockpit::CANOPY_POS.x, Cockpit::CANOPY_POS.y, Cockpit::CANOPY_POS.z,
            Cockpit::CANOPY_ROT.x, Cockpit::CANOPY_ROT.y, Cockpit::CANOPY_ROT.z,
            Cockpit::CANOPY_SCALE.x, Cockpit::CANOPY_SCALE.y, Cockpit::CANOPY_SCALE.z,
            Colors::BLUE_GLASS);

        cube.draw(shader, parentModel,
            Cockpit::FRAME_POS.x, Cockpit::FRAME_POS.y, Cockpit::FRAME_POS.z,
            Cockpit::FRAME_ROT.x, Cockpit::FRAME_ROT.y, Cockpit::FRAME_ROT.z,
            Cockpit::FRAME_SCALE.x, Cockpit::FRAME_SCALE.y, Cockpit::FRAME_SCALE.z,
            Colors::PANEL_DARK);

        // ============== WINGS ==============
        wedge.draw(shader, parentModel,
            Wings::LEFT_MAIN_POS.x, Wings::LEFT_MAIN_POS.y, Wings::LEFT_MAIN_POS.z,
            Wings::LEFT_MAIN_ROT.x, Wings::LEFT_MAIN_ROT.y, Wings::LEFT_MAIN_ROT.z,
            Wings::LEFT_MAIN_SCALE.x, Wings::LEFT_MAIN_SCALE.y, Wings::LEFT_MAIN_SCALE.z,
            Colors::GUNMETAL);

        wedge.draw(shader, parentModel,
            Wings::RIGHT_MAIN_POS.x, Wings::RIGHT_MAIN_POS.y, Wings::RIGHT_MAIN_POS.z,
            Wings::RIGHT_MAIN_ROT.x, Wings::RIGHT_MAIN_ROT.y, Wings::RIGHT_MAIN_ROT.z,
            Wings::RIGHT_MAIN_SCALE.x, Wings::RIGHT_MAIN_SCALE.y, Wings::RIGHT_MAIN_SCALE.z,
            Colors::GUNMETAL);

        cube.draw(shader, parentModel,
            Wings::LEFT_TIP_POS.x, Wings::LEFT_TIP_POS.y, Wings::LEFT_TIP_POS.z,
            Wings::LEFT_TIP_ROT.x, Wings::LEFT_TIP_ROT.y, Wings::LEFT_TIP_ROT.z,
            Wings::LEFT_TIP_SCALE.x, Wings::LEFT_TIP_SCALE.y, Wings::LEFT_TIP_SCALE.z,
            Colors::RED_ACCENT);

        cube.draw(shader, parentModel,
            Wings::RIGHT_TIP_POS.x, Wings::RIGHT_TIP_POS.y, Wings::RIGHT_TIP_POS.z,
            Wings::RIGHT_TIP_ROT.x, Wings::RIGHT_TIP_ROT.y, Wings::RIGHT_TIP_ROT.z,
            Wings::RIGHT_TIP_SCALE.x, Wings::RIGHT_TIP_SCALE.y, Wings::RIGHT_TIP_SCALE.z,
            Colors::RED_ACCENT);

        // ============== ENGINE NACELLES ==============
        cylinder.draw(shader, parentModel,
            Engines::LEFT_POD_POS.x, Engines::LEFT_POD_POS.y, Engines::LEFT_POD_POS.z,
            Engines::LEFT_POD_ROT.x, Engines::LEFT_POD_ROT.y, Engines::LEFT_POD_ROT.z,
            Engines::LEFT_POD_SCALE.x, Engines::LEFT_POD_SCALE.y, Engines::LEFT_POD_SCALE.z,
            Colors::DARK_GRAY);

        cylinder.draw(shader, parentModel,
            Engines::LEFT_INTAKE_POS.x, Engines::LEFT_INTAKE_POS.y, Engines::LEFT_INTAKE_POS.z,
            Engines::LEFT_INTAKE_ROT.x, Engines::LEFT_INTAKE_ROT.y, Engines::LEFT_INTAKE_ROT.z,
            Engines::LEFT_INTAKE_SCALE.x, Engines::LEFT_INTAKE_SCALE.y, Engines::LEFT_INTAKE_SCALE.z,
            Colors::ALMOST_BLACK);

        sphere.draw(shader, parentModel,
            Engines::LEFT_EXHAUST_POS.x, Engines::LEFT_EXHAUST_POS.y, Engines::LEFT_EXHAUST_POS.z,
            Engines::LEFT_EXHAUST_ROT.x, Engines::LEFT_EXHAUST_ROT.y, Engines::LEFT_EXHAUST_ROT.z,
            Engines::LEFT_EXHAUST_SCALE.x, Engines::LEFT_EXHAUST_SCALE.y, Engines::LEFT_EXHAUST_SCALE.z,
            Colors::ORANGE_THRUST);

        cylinder.draw(shader, parentModel,
            Engines::RIGHT_POD_POS.x, Engines::RIGHT_POD_POS.y, Engines::RIGHT_POD_POS.z,
            Engines::RIGHT_POD_ROT.x, Engines::RIGHT_POD_ROT.y, Engines::RIGHT_POD_ROT.z,
            Engines::RIGHT_POD_SCALE.x, Engines::RIGHT_POD_SCALE.y, Engines::RIGHT_POD_SCALE.z,
            Colors::DARK_GRAY);

        cylinder.draw(shader, parentModel,
            Engines::RIGHT_INTAKE_POS.x, Engines::RIGHT_INTAKE_POS.y, Engines::RIGHT_INTAKE_POS.z,
            Engines::RIGHT_INTAKE_ROT.x, Engines::RIGHT_INTAKE_ROT.y, Engines::RIGHT_INTAKE_ROT.z,
            Engines::RIGHT_INTAKE_SCALE.x, Engines::RIGHT_INTAKE_SCALE.y, Engines::RIGHT_INTAKE_SCALE.z,
            Colors::ALMOST_BLACK);

        sphere.draw(shader, parentModel,
            Engines::RIGHT_EXHAUST_POS.x, Engines::RIGHT_EXHAUST_POS.y, Engines::RIGHT_EXHAUST_POS.z,
            Engines::RIGHT_EXHAUST_ROT.x, Engines::RIGHT_EXHAUST_ROT.y, Engines::RIGHT_EXHAUST_ROT.z,
            Engines::RIGHT_EXHAUST_SCALE.x, Engines::RIGHT_EXHAUST_SCALE.y, Engines::RIGHT_EXHAUST_SCALE.z,
            Colors::ORANGE_THRUST);

        // ============== VERTICAL STABILIZERS ==============
        wedge.draw(shader, parentModel,
            Stabilizers::MAIN_FIN_POS.x, Stabilizers::MAIN_FIN_POS.y, Stabilizers::MAIN_FIN_POS.z,
            Stabilizers::MAIN_FIN_ROT.x, Stabilizers::MAIN_FIN_ROT.y, Stabilizers::MAIN_FIN_ROT.z,
            Stabilizers::MAIN_FIN_SCALE.x, Stabilizers::MAIN_FIN_SCALE.y, Stabilizers::MAIN_FIN_SCALE.z,
            Colors::GUNMETAL);

        cube.draw(shader, parentModel,
            Stabilizers::FIN_ACCENT_POS.x, Stabilizers::FIN_ACCENT_POS.y, Stabilizers::FIN_ACCENT_POS.z,
            Stabilizers::FIN_ACCENT_ROT.x, Stabilizers::FIN_ACCENT_ROT.y, Stabilizers::FIN_ACCENT_ROT.z,
            Stabilizers::FIN_ACCENT_SCALE.x, Stabilizers::FIN_ACCENT_SCALE.y, Stabilizers::FIN_ACCENT_SCALE.z,
            Colors::CYAN_ACCENT);

        // ============== WEAPONS / DETAILS ==============
        cylinder.draw(shader, parentModel,
            Weapons::LEFT_CANNON_POS.x, Weapons::LEFT_CANNON_POS.y, Weapons::LEFT_CANNON_POS.z,
            Weapons::LEFT_CANNON_ROT.x, Weapons::LEFT_CANNON_ROT.y, Weapons::LEFT_CANNON_ROT.z,
            Weapons::LEFT_CANNON_SCALE.x, Weapons::LEFT_CANNON_SCALE.y, Weapons::LEFT_CANNON_SCALE.z,
            Colors::WEAPON_METAL);

        cylinder.draw(shader, parentModel,
            Weapons::RIGHT_CANNON_POS.x, Weapons::RIGHT_CANNON_POS.y, Weapons::RIGHT_CANNON_POS.z,
            Weapons::RIGHT_CANNON_ROT.x, Weapons::RIGHT_CANNON_ROT.y, Weapons::RIGHT_CANNON_ROT.z,
            Weapons::RIGHT_CANNON_SCALE.x, Weapons::RIGHT_CANNON_SCALE.y, Weapons::RIGHT_CANNON_SCALE.z,
            Colors::WEAPON_METAL);

        sphere.draw(shader, parentModel,
            Details::SENSOR_POS.x, Details::SENSOR_POS.y, Details::SENSOR_POS.z,
            Details::SENSOR_ROT.x, Details::SENSOR_ROT.y, Details::SENSOR_ROT.z,
            Details::SENSOR_SCALE.x, Details::SENSOR_SCALE.y, Details::SENSOR_SCALE.z,
            Colors::YELLOW_SENSOR);

        // ============== HULL DETAILS ==============
        cube.draw(shader, parentModel,
            Details::LEFT_PANEL_POS.x, Details::LEFT_PANEL_POS.y, Details::LEFT_PANEL_POS.z,
            Details::LEFT_PANEL_ROT.x, Details::LEFT_PANEL_ROT.y, Details::LEFT_PANEL_ROT.z,
            Details::LEFT_PANEL_SCALE.x, Details::LEFT_PANEL_SCALE.y, Details::LEFT_PANEL_SCALE.z,
            Colors::VERY_DARK_GRAY);

        cube.draw(shader, parentModel,
            Details::RIGHT_PANEL_POS.x, Details::RIGHT_PANEL_POS.y, Details::RIGHT_PANEL_POS.z,
            Details::RIGHT_PANEL_ROT.x, Details::RIGHT_PANEL_ROT.y, Details::RIGHT_PANEL_ROT.z,
            Details::RIGHT_PANEL_SCALE.x, Details::RIGHT_PANEL_SCALE.y, Details::RIGHT_PANEL_SCALE.z,
            Colors::VERY_DARK_GRAY);

        cube.draw(shader, parentModel,
            Details::UNDERCARRIAGE_POS.x, Details::UNDERCARRIAGE_POS.y, Details::UNDERCARRIAGE_POS.z,
            Details::UNDERCARRIAGE_ROT.x, Details::UNDERCARRIAGE_ROT.y, Details::UNDERCARRIAGE_ROT.z,
            Details::UNDERCARRIAGE_SCALE.x, Details::UNDERCARRIAGE_SCALE.y, Details::UNDERCARRIAGE_SCALE.z,
            Colors::DARKER_GRAY);
    }
};

#endif
