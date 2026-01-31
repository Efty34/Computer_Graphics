#include "Boilerplate.h"
#include "Shader.h"
#include "Basic_Camera.h"
#include "Ship.h"
#include "CockpitInterior.h"
#include "AppConfig.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- Globals ---
// Isometric view camera (external)
BasicCamera camera(
    AppConfig::Camera::INITIAL_POSITION, 
    AppConfig::Camera::WORLD_UP, 
    AppConfig::Camera::ISOMETRIC_YAW, 
    AppConfig::Camera::ISOMETRIC_PITCH
);

// Cockpit view camera (internal first-person)
BasicCamera cockpitCamera(
    AppConfig::Camera::COCKPIT_POSITION,
    AppConfig::Camera::WORLD_UP,
    AppConfig::Camera::COCKPIT_YAW,
    AppConfig::Camera::COCKPIT_PITCH
);

float lastX = AppConfig::Camera::INITIAL_MOUSE_X;
float lastY = AppConfig::Camera::INITIAL_MOUSE_Y;
bool firstMouse = true;
bool isIsometric = AppConfig::ViewMode::DEFAULT_ISOMETRIC;
float cockpitLookYaw = 0.0f;
float cockpitLookPitch = 0.0f;

// Input handling function
void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, AppConfig::Input::KEY_EXIT) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera Movement (Isometric view)
    if (glfwGetKey(window, AppConfig::Input::KEY_FORWARD) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, AppConfig::Input::KEY_BACKWARD) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, AppConfig::Input::KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, AppConfig::Input::KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
    // Toggle View (Perspective/Isometric)
    if (glfwGetKey(window, AppConfig::Input::KEY_PERSPECTIVE) == GLFW_PRESS)
        isIsometric = false;
    if (glfwGetKey(window, AppConfig::Input::KEY_ISOMETRIC) == GLFW_PRESS)
        isIsometric = true;

    // Cockpit camera look around (Arrow keys)
    float lookSpeed = 50.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cockpitLookYaw += lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cockpitLookYaw -= lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cockpitLookPitch += lookSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cockpitLookPitch -= lookSpeed;
    
    // Clamp cockpit look
    if (cockpitLookPitch > 30.0f) cockpitLookPitch = 30.0f;
    if (cockpitLookPitch < -30.0f) cockpitLookPitch = -30.0f;
    if (cockpitLookYaw > 60.0f) cockpitLookYaw = 60.0f;
    if (cockpitLookYaw < -60.0f) cockpitLookYaw = -60.0f;
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    // Only move camera with mouse if right click is held (common in editors) or just always?
    // For specific Isometric view, we might want to lock rotation, but let's allow it for inspection.
    /*
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    */
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.Zoom -= (float)yoffset;
    if (camera.Zoom < AppConfig::Camera::ZOOM_MIN)
        camera.Zoom = AppConfig::Camera::ZOOM_MIN;
    if (camera.Zoom > AppConfig::Camera::ZOOM_MAX)
        camera.Zoom = AppConfig::Camera::ZOOM_MAX;
}

int main() {
    Application app(
        AppConfig::Window::WIDTH, 
        AppConfig::Window::HEIGHT, 
        AppConfig::Window::TITLE
    );

    if (!app.initialize()) {
        return -1;
    }

    // Set callbacks
    glfwSetCursorPosCallback(app.getWindow(), mouse_callback);
    glfwSetScrollCallback(app.getWindow(), scroll_callback);
    
    if (AppConfig::Input::CURSOR_DISABLED) {
        glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Shader shader(
        AppConfig::Shaders::VERTEX_SHADER, 
        AppConfig::Shaders::FRAGMENT_SHADER
    );
    Ship ship;
    CockpitInterior cockpit;

    // Main loop via Application
    app.run(
        // Setup
        [&]() {
            if (AppConfig::Rendering::DEPTH_TEST_ENABLED) {
                glEnable(GL_DEPTH_TEST);
            }
            glEnable(GL_SCISSOR_TEST);
        },
        // Update
        [&](float deltaTime) {
            processInput(app.getWindow(), deltaTime);
        },
        // Render
        [&]() {
            unsigned int width = app.getWidth();
            unsigned int height = app.getHeight();
            unsigned int halfWidth = width / 2;

            shader.use();

            // ==================== LEFT VIEWPORT: ISOMETRIC VIEW ====================
            glViewport(0, 0, halfWidth, height);
            glScissor(0, 0, halfWidth, height);
            glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Isometric Projection
            glm::mat4 projection;
            float aspect = (float)halfWidth / (float)height;

            if (isIsometric) {
                float scale = AppConfig::Projection::ORTHO_SCALE;
                projection = glm::ortho(
                    -scale * aspect, scale * aspect, 
                    -scale, scale, 
                    AppConfig::Projection::NEAR_PLANE, 
                    AppConfig::Projection::FAR_PLANE
                );
            }
            else {
                projection = glm::perspective(
                    glm::radians(camera.Zoom), 
                    aspect, 
                    AppConfig::Projection::NEAR_PLANE, 
                    AppConfig::Projection::FAR_PLANE
                );
            }
            shader.setMat4("projection", projection);

            // Isometric View
            glm::mat4 view = camera.GetViewMatrix();
            shader.setMat4("view", view);

            // Draw Ship (external view)
            glm::mat4 model = glm::mat4(1.0f);
            ship.draw(shader, model);

            // ==================== RIGHT VIEWPORT: COCKPIT VIEW ====================
            glViewport(halfWidth, 0, halfWidth, height);
            glScissor(halfWidth, 0, halfWidth, height);
            glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Cockpit Perspective Projection (wider FOV for immersion)
            float cockpitFOV = 75.0f;
            projection = glm::perspective(
                glm::radians(cockpitFOV), 
                aspect, 
                0.01f,  // Near plane very close for cockpit
                50.0f
            );
            shader.setMat4("projection", projection);

            // Cockpit View - First person inside the ship
            glm::vec3 cockpitPos = AppConfig::Camera::COCKPIT_POSITION;
            
            // Calculate look direction from yaw and pitch
            float yaw = glm::radians(AppConfig::Camera::COCKPIT_YAW + cockpitLookYaw);
            float pitch = glm::radians(AppConfig::Camera::COCKPIT_PITCH + cockpitLookPitch);
            
            glm::vec3 cockpitFront;
            cockpitFront.x = cos(yaw) * cos(pitch);
            cockpitFront.y = sin(pitch);
            cockpitFront.z = sin(yaw) * cos(pitch);
            cockpitFront = glm::normalize(cockpitFront);
            
            glm::mat4 cockpitView = glm::lookAt(
                cockpitPos,
                cockpitPos + cockpitFront,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
            shader.setMat4("view", cockpitView);

            // Draw Cockpit Interior
            glm::mat4 cockpitModel = glm::mat4(1.0f);
            cockpit.draw(shader, cockpitModel);

            // Reset scissor for next frame
            glScissor(0, 0, width, height);
        }
    );

    app.shutdown();
    return 0;
}
