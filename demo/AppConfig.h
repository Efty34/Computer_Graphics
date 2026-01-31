#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <glm/glm.hpp>
#include <string>

namespace AppConfig {
    
    // ============== WINDOW SETTINGS ==============
    namespace Window {
        const unsigned int WIDTH = 1600;
        const unsigned int HEIGHT = 800;
        const char* TITLE = "Spaceship - Isometric & Cockpit View";
        const glm::vec3 CLEAR_COLOR(0.05f, 0.05f, 0.08f);
    }
    
    // ============== CAMERA SETTINGS ==============
    namespace Camera {
        // Initial camera position for isometric view
        const glm::vec3 INITIAL_POSITION(5.0f, 5.0f, 5.0f);
        const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
        
        // Isometric view angles
        const float ISOMETRIC_YAW = -135.0f;
        const float ISOMETRIC_PITCH = -35.264f;
        
        // Cockpit camera (inside the ship looking forward)
        const glm::vec3 COCKPIT_POSITION(0.0f, 0.0f, 0.0f);
        const float COCKPIT_YAW = 0.0f;
        const float COCKPIT_PITCH = -10.0f;
        
        // Camera movement
        const float MOVEMENT_SPEED = 2.5f;
        const float MOUSE_SENSITIVITY = 0.1f;
        
        // Zoom settings
        const float ZOOM_MIN = 1.0f;
        const float ZOOM_MAX = 45.0f;
        const float ZOOM_DEFAULT = 45.0f;
        
        // Mouse initial position (half of window size)
        const float INITIAL_MOUSE_X = Window::WIDTH / 2.0f;
        const float INITIAL_MOUSE_Y = Window::HEIGHT / 2.0f;
    }
    
    // ============== PROJECTION SETTINGS ==============
    namespace Projection {
        // Perspective
        const float FOV = 45.0f;
        const float NEAR_PLANE = 0.1f;
        const float FAR_PLANE = 100.0f;
        
        // Orthographic (Isometric)
        const float ORTHO_SCALE = 5.0f;
    }
    
    // ============== SHADER PATHS ==============
    namespace Shaders {
        const char* VERTEX_SHADER = "vertexShader.vs";
        const char* FRAGMENT_SHADER = "fragmentShader.fs";
    }
    
    // ============== VIEW MODE ==============
    namespace ViewMode {
        const bool DEFAULT_ISOMETRIC = true;
    }
    
    // ============== INPUT SETTINGS ==============
    namespace Input {
        // Keyboard keys
        const int KEY_EXIT = GLFW_KEY_ESCAPE;
        const int KEY_FORWARD = GLFW_KEY_W;
        const int KEY_BACKWARD = GLFW_KEY_S;
        const int KEY_LEFT = GLFW_KEY_A;
        const int KEY_RIGHT = GLFW_KEY_D;
        const int KEY_UP = GLFW_KEY_SPACE;
        const int KEY_DOWN = GLFW_KEY_LEFT_SHIFT;
        const int KEY_PERSPECTIVE = GLFW_KEY_P;
        const int KEY_ISOMETRIC = GLFW_KEY_I;
        
        // Mouse
        const bool MOUSE_ENABLED = false; // Disable mouse camera by default for isometric
        const bool CURSOR_DISABLED = false;
    }
    
    // ============== RENDERING SETTINGS ==============
    namespace Rendering {
        const bool DEPTH_TEST_ENABLED = true;
        const bool VSYNC_ENABLED = true;
    }
}

#endif
