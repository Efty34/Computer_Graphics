#include "shader.h"
#include "basic_camera.h" 
#include "Cube.h"
#include "Table.h"
#include "Chair.h"
#include "Boundary.h"
#include "Monitor.h"
#include "Boilerplate.h"
#include "Lamp.h"
#include "Window.h"

#include <iostream>

using namespace std;

void processInput(GLFWwindow* window, float deltaTime);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
BasicCamera camera(glm::vec3(0.0f, 1.5f, 5.0f));

// --- STATE VARIABLES ---
bool lightOn = true;          // Key L
bool fanOn = false;           // Key G
float fanAngle = 0.0f;
bool doorOpen = false;        // Key to toggle door (Using 'K' for door, or custom)
float doorAngle = 0.0f;
bool birdView = false;        // Key B for Birds Eye
float lampRotation = 0.0f;    // Key H to rotate ceiling lamp
float lampSwingAngle = 0.0f;  // Key P to swing ceiling lamp

// Rotation for LookAt (Key F)
float orbitAngle = 1.0f;

// Object instances
Shader* ourShader = nullptr;
Cube* cube = nullptr;
Boundary* room = nullptr;
Table* teacherTable = nullptr;
Chair* teacherChair = nullptr;
Table* studentTable = nullptr;
Chair* studentChair = nullptr;
Monitor* monitor = nullptr;
Lamp* ceilingLamp = nullptr;
Window* classroomWindow = nullptr;

void printUsage() {
    cout << "W/S/A/D - Move camera" << endl;
    cout << "E/R - Move up/down" << endl;
    cout << "X - Pitch, Y - Yaw, Z - Roll (Hold Shift to reverse)" << endl;
    cout << "F - Orbit camera" << endl;
    cout << "G - Toggle fan" << endl;
    cout << "H - Rotate ceiling lamp" << endl;
    cout << "P - Swing ceiling lamp" << endl;
    cout << "L - Toggle lights" << endl;
    cout << "SPACE - Toggle door" << endl;
    cout << "B - Bird's eye view" << endl;
    cout << "ESC - Exit" << endl;
}

void setup() {
    // Initialize shaders and objects
    ourShader = new Shader("vertexShader.vs", "fragmentShader.fs");
    cube = new Cube();
    room = new Boundary();
    teacherTable = new Table(2.5f, 1.2f, 0.8f, glm::vec3(0.3f, 0.2f, 0.15f));
    teacherChair = new Chair(0.6f, 0.6f, 0.5f, glm::vec3(0.2f, 0.15f, 0.1f));
    studentTable = new Table(1.2f, 0.8f, 0.75f, glm::vec3(0.7f, 0.5f, 0.3f));
    studentChair = new Chair(0.5f, 0.5f, 0.45f, glm::vec3(0.6f, 0.45f, 0.25f));
    monitor = new Monitor();
    ceilingLamp = new Lamp();
    classroomWindow = new Window();

    printUsage();
}

void update(GLFWwindow* window, float deltaTime) {
    processInput(window, deltaTime);

    // Fan rotation
    if (fanOn) {
        fanAngle += 200.0f * deltaTime;
        if (fanAngle > 360.0f) fanAngle -= 360.0f;
    }

    // Door animation
    if (doorOpen && doorAngle < 90.0f) doorAngle += 100.0f * deltaTime;
    if (!doorOpen && doorAngle > 0.0f) doorAngle -= 100.0f * deltaTime;
}

void render() {
    ourShader->use();
    ourShader->setInt("lightOn", lightOn);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
        (float)SCR_WIDTH / (float)SCR_HEIGHT,
        0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);

    glm::mat4 identity = glm::mat4(1.0f);

    // 1. ROOM (Floor and Walls)
    room->drawRoom(*ourShader, identity);

    // 2. WINDOW on Left Wall
    // Left wall is at x = -5.0 with thickness 0.1, so window at -4.95 to avoid z-fighting
    // Window centered vertically at y = 1.0, horizontally at z = 0.0
    classroomWindow->draw(*ourShader, identity, -4.95f, 1.0f, 0.0f, 0.0f, 90.0f, 0.0f);

    // 3. CLASSROOM SETUP
    // Teacher's desk at front
    teacherTable->draw(*ourShader, identity, -1.25f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f);
    monitor->draw(*ourShader, identity, 1.0f, 0.8f, -3.0f, 0.0f, 0.0f, 0.0f);
    teacherChair->draw(*ourShader, identity, 0.40f, 0.0f, -4.3f, 0.0f, 180.0f, 0.0f);

    // Student desks - Row 1
    float row1Z = -2.0f;
    float row2Z = 0.5f;
    float spacing = 1.8f;
    float startX = -2.5f;

    // Row 1 - 3 desks
    for (int i = 0; i < 3; i++) {
        float xPos = startX + i * spacing;
        studentTable->draw(*ourShader, identity, xPos, 0.0f, row1Z, 0.0f, 0.0f, 0.0f);
        studentChair->draw(*ourShader, identity, xPos + 0.35f, 0.0f, row1Z + 0.9f, 0.0f, 0.0f, 0.0f);
    }

    // Row 2 - 3 desks (behind row 1)
    for (int i = 0; i < 3; i++) {
        float xPos = startX + i * spacing;
        studentTable->draw(*ourShader, identity, xPos, 0.0f, row2Z, 0.0f, 0.0f, 0.0f);
        studentChair->draw(*ourShader, identity, xPos + 0.35f, 0.0f, row2Z + 0.9f, 0.0f, 0.0f, 0.0f);
    }

    // 4. CEILING FAN
    glm::mat4 fanBase = glm::translate(identity, glm::vec3(0.0f, 3.5f, 0.0f));
    cube->draw(*ourShader, fanBase, -0.1f, 0.0f, -0.1f, 0.0f, fanAngle, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec3(0.2f, 0.2f, 0.2f));

    // Blades (rotate around hub)
    glm::mat4 bladeCenter = glm::rotate(fanBase, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    cube->draw(*ourShader, bladeCenter, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    cube->draw(*ourShader, bladeCenter, 0.1f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    cube->draw(*ourShader, bladeCenter, 0.0f, 0.0f, -0.1f, 0.0f, 180.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    cube->draw(*ourShader, bladeCenter, -0.1f, 0.0f, 0.0f, 0.0f, 270.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));

    // 5. DOOR (Animating with Frame and Handle)
    glm::vec3 doorFramePos = glm::vec3(2.5f, 0.0f, -5.0f);
    glm::vec3 frameColor = glm::vec3(0.15f, 0.1f, 0.08f);
    glm::vec3 doorColor = glm::vec3(0.8f, 0.6f, 0.3f);
    glm::vec3 handleColor = glm::vec3(0.7f, 0.7f, 0.1f);

    // Door frame
    cube->draw(*ourShader, identity, doorFramePos.x - 0.1f, 2.5f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 1.3f, 0.2f, 0.15f, frameColor);
    cube->draw(*ourShader, identity, doorFramePos.x - 0.1f, 0.0f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 0.1f, 2.7f, 0.15f, frameColor);
    cube->draw(*ourShader, identity, doorFramePos.x + 1.1f, 0.0f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 0.1f, 2.7f, 0.15f, frameColor);

    // Door
    glm::mat4 doorHinge = glm::translate(identity, doorFramePos);
    cube->draw(*ourShader, doorHinge, 1.0f, 0.0f, 0.0f, 0.0f, -doorAngle, 0.0f, 1.0f, 2.5f, 0.1f, doorColor);

    // Door handle
    glm::mat4 handleTransform = glm::rotate(doorHinge, glm::radians(-doorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    cube->draw(*ourShader, handleTransform, 0.2f, 1.2f, 0.15f, 0.0f, 0.0f, 90.0f, 0.3f, 0.05f, 0.05f, handleColor);

    // 7. CEILING LAMP
    // Position: mounted on ceiling at y=4.0f (ceiling height)
    ceilingLamp->draw(*ourShader, identity, 1.5f, 4.0f, 1.0f, lampRotation, lampSwingAngle);
}

void cleanup() {
    delete ourShader;
    delete cube;
    delete room;
    delete teacherTable;
    delete teacherChair;
    delete studentTable;
    delete studentChair;
    delete monitor;
    delete ceilingLamp;
    delete classroomWindow;
}

int main() {
    Application app(SCR_WIDTH, SCR_HEIGHT, "Assignment: 3D Lab");

    if (!app.initialize()) {
        return -1;
    }

    GLFWwindow* window = app.getWindow();

    app.run(
        []() { setup(); },
        [window](float deltaTime) { update(window, deltaTime); },
        []() { render(); }
    );

    cleanup();

    return 0;
}

void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Standard Movement (W, A, S, D) + Up/Down (E, R)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);

    // Pitch Key x - rotate up/down
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        camera.ProcessPitch(30.0f * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessPitch(-30.0f * deltaTime);

    // Yaw Key y - rotate left/right
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        camera.ProcessYaw(30.0f * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessYaw(-30.0f * deltaTime);

    // Roll Key z - tilt camera
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        camera.ProcessRoll(30.0f * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessRoll(-30.0f * deltaTime);

    // Fan Control (Key G)
    static bool gPressed = false;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gPressed) {
        fanOn = !fanOn;
        gPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) gPressed = false;

    // Ceiling Lamp Rotation Control (Key H)
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        lampRotation += 100.0f * deltaTime;
        if (lampRotation > 360.0f) lampRotation -= 360.0f;
    }

    // Ceiling Lamp Swing Control (Key P)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        lampSwingAngle += 50.0f * deltaTime;
        if (lampSwingAngle > 30.0f) lampSwingAngle = 30.0f; // Limit swing angle
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE && lampSwingAngle > 0.0f) {
        lampSwingAngle -= 50.0f * deltaTime;
        if (lampSwingAngle < 0.0f) lampSwingAngle = 0.0f;
    }

    // Light Control (Key L)
    static bool lPressed = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lPressed) {
        lightOn = !lightOn;
        lPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) lPressed = false;

    // Door Control (Key Space for easy access, or define custom)
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
        doorOpen = !doorOpen;
        spacePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) spacePressed = false;

    // Bird's Eye View (Key B)
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        camera.Position = glm::vec3(0.0f, 10.0f, 0.0f);
        camera.Yaw = -90.0f;
        camera.Pitch = -90.0f;
        camera.Roll = 0.0f;
        camera.updateCameraVectors();
    }

    // Look At Rotation (Key F) - Orbit around 0,0,0
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        float radius = 10.0f;
        orbitAngle += 50.0f * deltaTime;
        float camX = sin(glm::radians(orbitAngle)) * radius;
        float camZ = cos(glm::radians(orbitAngle)) * radius;
        camera.Position = glm::vec3(camX, 5.0f, camZ);
        camera.updateCameraVectors();
    }
}