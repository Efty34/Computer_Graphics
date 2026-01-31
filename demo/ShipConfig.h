#ifndef SHIP_CONFIG_H
#define SHIP_CONFIG_H

#include <glm/glm.hpp>

namespace ShipConfig {
    
    // ============== COLOR PALETTE ==============
    namespace Colors {
        const glm::vec3 DARK_GUNMETAL(0.15f, 0.15f, 0.2f);
        const glm::vec3 GUNMETAL(0.18f, 0.18f, 0.22f);
        const glm::vec3 DARK_GRAY(0.12f, 0.12f, 0.15f);
        const glm::vec3 VERY_DARK_GRAY(0.1f, 0.1f, 0.12f);
        const glm::vec3 DARKER_GRAY(0.08f, 0.08f, 0.1f);
        const glm::vec3 ALMOST_BLACK(0.05f, 0.05f, 0.08f);
        const glm::vec3 ARMOR_PLATE(0.2f, 0.2f, 0.25f);
        const glm::vec3 PANEL_DARK(0.1f, 0.1f, 0.1f);
        const glm::vec3 WEAPON_METAL(0.25f, 0.25f, 0.28f);
        
        const glm::vec3 CYAN_ACCENT(0.0f, 0.8f, 1.0f);
        const glm::vec3 BLUE_GLASS(0.1f, 0.6f, 0.9f);
        const glm::vec3 RED_ACCENT(0.8f, 0.1f, 0.1f);
        const glm::vec3 ORANGE_THRUST(1.0f, 0.4f, 0.1f);
        const glm::vec3 YELLOW_SENSOR(0.9f, 0.9f, 0.2f);
        const glm::vec3 NOSE_TIP(0.1f, 0.1f, 0.15f);
    }
    
    // ============== FUSELAGE ==============
    namespace Fuselage {
        const glm::vec3 MAIN_POS(0.0f, 0.0f, 0.0f);
        const glm::vec3 MAIN_ROT(0.0f, 0.0f, 90.0f);
        const glm::vec3 MAIN_SCALE(3.5f, 0.6f, 0.6f);
        
        const glm::vec3 ARMOR_POS(0.0f, 0.35f, 0.0f);
        const glm::vec3 ARMOR_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 ARMOR_SCALE(2.5f, 0.15f, 0.5f);
    }
    
    // ============== NOSE ==============
    namespace Nose {
        const glm::vec3 CONE_POS(2.3f, 0.0f, 0.0f);
        const glm::vec3 CONE_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 CONE_SCALE(0.5f, 1.2f, 0.5f);
        
        const glm::vec3 RING_POS(1.7f, 0.0f, 0.0f);
        const glm::vec3 RING_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 RING_SCALE(0.55f, 0.1f, 0.55f);
    }
    
    // ============== COCKPIT ==============
    namespace Cockpit {
        const glm::vec3 CANOPY_POS(0.8f, 0.4f, 0.0f);
        const glm::vec3 CANOPY_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 CANOPY_SCALE(0.8f, 0.4f, 0.5f);
        
        const glm::vec3 FRAME_POS(0.8f, 0.25f, 0.0f);
        const glm::vec3 FRAME_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 FRAME_SCALE(0.9f, 0.08f, 0.55f);
    }
    
    // ============== WINGS ==============
    namespace Wings {
        const glm::vec3 LEFT_MAIN_POS(-0.3f, -0.1f, 1.2f);
        const glm::vec3 LEFT_MAIN_ROT(90.0f, 0.0f, -20.0f);
        const glm::vec3 LEFT_MAIN_SCALE(2.0f, 1.8f, 0.12f);
        
        const glm::vec3 RIGHT_MAIN_POS(-0.3f, -0.1f, -1.2f);
        const glm::vec3 RIGHT_MAIN_ROT(-90.0f, 0.0f, -20.0f);
        const glm::vec3 RIGHT_MAIN_SCALE(2.0f, 1.8f, 0.12f);
        
        const glm::vec3 LEFT_TIP_POS(-0.5f, -0.1f, 2.0f);
        const glm::vec3 LEFT_TIP_ROT(0.0f, -30.0f, 0.0f);
        const glm::vec3 LEFT_TIP_SCALE(0.8f, 0.08f, 0.4f);
        
        const glm::vec3 RIGHT_TIP_POS(-0.5f, -0.1f, -2.0f);
        const glm::vec3 RIGHT_TIP_ROT(0.0f, 30.0f, 0.0f);
        const glm::vec3 RIGHT_TIP_SCALE(0.8f, 0.08f, 0.4f);
    }
    
    // ============== ENGINES ==============
    namespace Engines {
        const glm::vec3 LEFT_POD_POS(-0.8f, 0.0f, 0.9f);
        const glm::vec3 LEFT_POD_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 LEFT_POD_SCALE(0.25f, 1.5f, 0.25f);
        
        const glm::vec3 LEFT_INTAKE_POS(0.0f, 0.0f, 0.9f);
        const glm::vec3 LEFT_INTAKE_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 LEFT_INTAKE_SCALE(0.28f, 0.15f, 0.28f);
        
        const glm::vec3 LEFT_EXHAUST_POS(-1.6f, 0.0f, 0.9f);
        const glm::vec3 LEFT_EXHAUST_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 LEFT_EXHAUST_SCALE(0.22f, 0.22f, 0.22f);
        
        const glm::vec3 RIGHT_POD_POS(-0.8f, 0.0f, -0.9f);
        const glm::vec3 RIGHT_POD_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 RIGHT_POD_SCALE(0.25f, 1.5f, 0.25f);
        
        const glm::vec3 RIGHT_INTAKE_POS(0.0f, 0.0f, -0.9f);
        const glm::vec3 RIGHT_INTAKE_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 RIGHT_INTAKE_SCALE(0.28f, 0.15f, 0.28f);
        
        const glm::vec3 RIGHT_EXHAUST_POS(-1.6f, 0.0f, -0.9f);
        const glm::vec3 RIGHT_EXHAUST_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 RIGHT_EXHAUST_SCALE(0.22f, 0.22f, 0.22f);
    }
    
    // ============== STABILIZERS ==============
    namespace Stabilizers {
        const glm::vec3 MAIN_FIN_POS(-1.2f, 0.7f, 0.0f);
        const glm::vec3 MAIN_FIN_ROT(0.0f, 0.0f, -15.0f);
        const glm::vec3 MAIN_FIN_SCALE(0.8f, 1.0f, 0.08f);
        
        const glm::vec3 FIN_ACCENT_POS(-1.4f, 0.9f, 0.0f);
        const glm::vec3 FIN_ACCENT_ROT(0.0f, 0.0f, -15.0f);
        const glm::vec3 FIN_ACCENT_SCALE(0.5f, 0.08f, 0.1f);
    }
    
    // ============== WEAPONS ==============
    namespace Weapons {
        const glm::vec3 LEFT_CANNON_POS(1.0f, -0.15f, 0.5f);
        const glm::vec3 LEFT_CANNON_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 LEFT_CANNON_SCALE(0.08f, 1.8f, 0.08f);
        
        const glm::vec3 RIGHT_CANNON_POS(1.0f, -0.15f, -0.5f);
        const glm::vec3 RIGHT_CANNON_ROT(0.0f, 0.0f, -90.0f);
        const glm::vec3 RIGHT_CANNON_SCALE(0.08f, 1.8f, 0.08f);
    }
    
    // ============== DETAILS ==============
    namespace Details {
        const glm::vec3 SENSOR_POS(1.5f, 0.15f, 0.0f);
        const glm::vec3 SENSOR_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 SENSOR_SCALE(0.15f, 0.1f, 0.15f);
        
        const glm::vec3 LEFT_PANEL_POS(-0.2f, 0.0f, 0.35f);
        const glm::vec3 LEFT_PANEL_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 LEFT_PANEL_SCALE(1.5f, 0.3f, 0.05f);
        
        const glm::vec3 RIGHT_PANEL_POS(-0.2f, 0.0f, -0.35f);
        const glm::vec3 RIGHT_PANEL_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 RIGHT_PANEL_SCALE(1.5f, 0.3f, 0.05f);
        
        const glm::vec3 UNDERCARRIAGE_POS(0.0f, -0.35f, 0.0f);
        const glm::vec3 UNDERCARRIAGE_ROT(0.0f, 0.0f, 0.0f);
        const glm::vec3 UNDERCARRIAGE_SCALE(2.0f, 0.1f, 0.3f);
    }
}

#endif
