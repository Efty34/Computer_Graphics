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
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;

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

// --- LIGHTING STATE VARIABLES ---
bool directionalLightOn = true;   // Key 1
bool pointLightOn = true;         // Key 2
bool spotLightOn = true;          // Key 3
bool ambientOn = true;            // Key 5
bool diffuseOn = true;            // Key 6
bool specularOn = true;           // Key 7

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
    cout << "=== CAMERA CONTROLS ===" << endl;
    cout << "W/S/A/D - Move camera" << endl;
    cout << "E/R - Move up/down" << endl;
    cout << "X - Pitch, Y - Yaw, Z - Roll (Hold Shift to reverse)" << endl;
    cout << "F - Orbit camera" << endl;
    cout << "B - Bird's eye view" << endl;
    cout << endl;
    cout << "=== OBJECT CONTROLS ===" << endl;
    cout << "G - Toggle fan" << endl;
    cout << "H - Rotate ceiling lamp" << endl;
    cout << "P - Swing ceiling lamp" << endl;
    cout << "SPACE - Toggle door" << endl;
    cout << endl;
    cout << "=== LIGHT TYPE CONTROLS ===" << endl;
    cout << "1 - Toggle Directional Light" << endl;
    cout << "2 - Toggle Point Lights" << endl;
    cout << "3 - Toggle Spot Light" << endl;
    cout << endl;
    cout << "=== LIGHT COMPONENT CONTROLS ===" << endl;
    cout << "5 - Toggle Ambient" << endl;
    cout << "6 - Toggle Diffuse" << endl;
    cout << "7 - Toggle Specular" << endl;
    cout << endl;
    cout << "ESC - Exit" << endl;
}

void setup() {
    // Initialize shaders and objects
    ourShader = new Shader("vertexShader.vs", "fragmentShader.fs");
    cube = new Cube();
    room = new Boundary();
    // Teacher's desk - rich dark mahogany wood
    teacherTable = new Table(2.5f, 1.2f, 0.8f, glm::vec3(0.35f, 0.18f, 0.1f));
    // Teacher's chair - dark walnut with hint of red
    teacherChair = new Chair(0.6f, 0.6f, 0.5f, glm::vec3(0.25f, 0.12f, 0.08f));
    // Student desks - lighter oak wood tone
    studentTable = new Table(1.2f, 0.8f, 0.75f, glm::vec3(0.72f, 0.53f, 0.35f));
    // Student chairs - natural beech wood
    studentChair = new Chair(0.5f, 0.5f, 0.45f, glm::vec3(0.8f, 0.65f, 0.45f));
    monitor = new Monitor();
    ceilingLamp = new Lamp();
    classroomWindow = new Window();

    printUsage();
}

// Function to set up all lighting uniforms
void setupLighting(Shader& shader, glm::vec3 viewPos) {
    shader.setVec3("viewPos", viewPos);

    // Light toggles
    shader.setBool("directionalLightOn", directionalLightOn);
    shader.setBool("pointLightOn", pointLightOn);
    shader.setBool("spotLightOn", spotLightOn);

    // Component toggles
    shader.setBool("ambientOn", ambientOn);
    shader.setBool("diffuseOn", diffuseOn);
    shader.setBool("specularOn", specularOn);

    // Directional Light (sunlight coming through window - from left side)
    // Window is on left wall (x = -5), so light direction points into room (+X, slightly down)
    shader.setVec3("directionalLight.direction", glm::vec3(1.0f, -0.3f, 0.2f));
    shader.setVec3("directionalLight.ambient", glm::vec3(0.2f, 0.2f, 0.25f));
    shader.setVec3("directionalLight.diffuse", glm::vec3(0.8f, 0.8f, 0.7f));
    shader.setVec3("directionalLight.specular", glm::vec3(0.5f, 0.5f, 0.4f));

    // Point Light 0 - Ceiling lamp position
    shader.setVec3("pointLights[0].position", glm::vec3(1.5f, 3.2f, 1.0f));
    shader.setVec3("pointLights[0].ambient", glm::vec3(0.1f, 0.1f, 0.08f));
    shader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.75f, 0.6f));
    shader.setVec3("pointLights[0].specular", glm::vec3(0.5f, 0.5f, 0.4f));
    shader.setFloat("pointLights[0].k_c", 1.0f);
    shader.setFloat("pointLights[0].k_l", 0.09f);
    shader.setFloat("pointLights[0].k_q", 0.032f);

    // Point Light 1 - Front left corner
    shader.setVec3("pointLights[1].position", glm::vec3(-3.0f, 3.0f, -3.0f));
    shader.setVec3("pointLights[1].ambient", glm::vec3(0.08f, 0.08f, 0.1f));
    shader.setVec3("pointLights[1].diffuse", glm::vec3(0.5f, 0.5f, 0.6f));
    shader.setVec3("pointLights[1].specular", glm::vec3(0.3f, 0.3f, 0.4f));
    shader.setFloat("pointLights[1].k_c", 1.0f);
    shader.setFloat("pointLights[1].k_l", 0.09f);
    shader.setFloat("pointLights[1].k_q", 0.032f);

    // Point Light 2 - Front right corner
    shader.setVec3("pointLights[2].position", glm::vec3(3.0f, 3.0f, -3.0f));
    shader.setVec3("pointLights[2].ambient", glm::vec3(0.08f, 0.08f, 0.1f));
    shader.setVec3("pointLights[2].diffuse", glm::vec3(0.5f, 0.5f, 0.6f));
    shader.setVec3("pointLights[2].specular", glm::vec3(0.3f, 0.3f, 0.4f));
    shader.setFloat("pointLights[2].k_c", 1.0f);
    shader.setFloat("pointLights[2].k_l", 0.09f);
    shader.setFloat("pointLights[2].k_q", 0.032f);

    // Point Light 3 - Back center
    shader.setVec3("pointLights[3].position", glm::vec3(0.0f, 3.0f, 3.0f));
    shader.setVec3("pointLights[3].ambient", glm::vec3(0.08f, 0.08f, 0.1f));
    shader.setVec3("pointLights[3].diffuse", glm::vec3(0.5f, 0.5f, 0.6f));
    shader.setVec3("pointLights[3].specular", glm::vec3(0.3f, 0.3f, 0.4f));
    shader.setFloat("pointLights[3].k_c", 1.0f);
    shader.setFloat("pointLights[3].k_l", 0.09f);
    shader.setFloat("pointLights[3].k_q", 0.032f);

    // Spot Light - Teacher's desk spotlight
    shader.setVec3("spotLight.position", glm::vec3(0.0f, 3.5f, -3.5f));
    shader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, -0.3f));
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(25.0f)));
    shader.setVec3("spotLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.setVec3("spotLight.diffuse", glm::vec3(0.9f, 0.9f, 0.8f));
    shader.setVec3("spotLight.specular", glm::vec3(0.7f, 0.7f, 0.6f));
    shader.setFloat("spotLight.k_c", 1.0f);
    shader.setFloat("spotLight.k_l", 0.07f);
    shader.setFloat("spotLight.k_q", 0.017f);

    // Default: not emissive
    shader.setBool("isEmissive", false);
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

// Helper function to draw the entire scene
void drawScene(Shader& shader, glm::mat4 identity) {
    // 1. ROOM (Floor and Walls)
    room->drawRoom(shader, identity);

    // 2. WINDOW on Left Wall
    classroomWindow->draw(shader, identity, -4.95f, 1.0f, 0.0f, 0.0f, 90.0f, 0.0f);

    // 3. CLASSROOM SETUP
    // Teacher's desk at front
    teacherTable->draw(shader, identity, -1.25f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f);
    monitor->draw(shader, identity, 1.0f, 0.8f, -3.0f, 0.0f, 0.0f, 0.0f);
    teacherChair->draw(shader, identity, 0.40f, 0.0f, -4.3f, 0.0f, 180.0f, 0.0f);

    // Student desks - Row 1
    float row1Z = -2.0f;
    float row2Z = 0.5f;
    float spacing = 1.8f;
    float startX = -2.5f;

    // Row 1 - 3 desks
    for (int i = 0; i < 3; i++) {
        float xPos = startX + i * spacing;
        studentTable->draw(shader, identity, xPos, 0.0f, row1Z, 0.0f, 0.0f, 0.0f);
        studentChair->draw(shader, identity, xPos + 0.35f, 0.0f, row1Z + 0.9f, 0.0f, 0.0f, 0.0f);
    }

    // Row 2 - 3 desks (behind row 1)
    for (int i = 0; i < 3; i++) {
        float xPos = startX + i * spacing;
        studentTable->draw(shader, identity, xPos, 0.0f, row2Z, 0.0f, 0.0f, 0.0f);
        studentChair->draw(shader, identity, xPos + 0.35f, 0.0f, row2Z + 0.9f, 0.0f, 0.0f, 0.0f);
    }

    // 4. CEILING FAN
    glm::mat4 fanBase = glm::translate(identity, glm::vec3(0.0f, 3.5f, 0.0f));
    cube->draw(shader, fanBase, -0.1f, 0.0f, -0.1f, 0.0f, fanAngle, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec3(0.15f, 0.15f, 0.18f));

    // Blades (rotate around hub) - off-white wooden blades
    glm::vec3 bladeColor = glm::vec3(0.92f, 0.9f, 0.85f);
    glm::mat4 bladeCenter = glm::rotate(fanBase, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    cube->draw(shader, bladeCenter, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.2f, 0.05f, 1.5f, bladeColor);
    cube->draw(shader, bladeCenter, 0.1f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.2f, 0.05f, 1.5f, bladeColor);
    cube->draw(shader, bladeCenter, 0.0f, 0.0f, -0.1f, 0.0f, 180.0f, 0.0f, 0.2f, 0.05f, 1.5f, bladeColor);
    cube->draw(shader, bladeCenter, -0.1f, 0.0f, 0.0f, 0.0f, 270.0f, 0.0f, 0.2f, 0.05f, 1.5f, bladeColor);

    // 5. DOOR (Animating with Frame and Handle)
    glm::vec3 doorFramePos = glm::vec3(2.5f, 0.0f, -5.0f);
    glm::vec3 frameColor = glm::vec3(0.22f, 0.12f, 0.06f);  // Dark walnut frame
    glm::vec3 doorColor = glm::vec3(0.45f, 0.28f, 0.15f);   // Rich teak door
    glm::vec3 handleColor = glm::vec3(0.72f, 0.58f, 0.2f);  // Brass door handle

    // Door frame
    cube->draw(shader, identity, doorFramePos.x - 0.1f, 2.5f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 1.3f, 0.2f, 0.15f, frameColor);
    cube->draw(shader, identity, doorFramePos.x - 0.1f, 0.0f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 0.1f, 2.7f, 0.15f, frameColor);
    cube->draw(shader, identity, doorFramePos.x + 1.1f, 0.0f, doorFramePos.z, 0.0f, 0.0f, 0.0f, 0.1f, 2.7f, 0.15f, frameColor);

    // Door
    glm::mat4 doorHinge = glm::translate(identity, doorFramePos);
    cube->draw(shader, doorHinge, 1.0f, 0.0f, 0.0f, 0.0f, -doorAngle, 0.0f, 1.0f, 2.5f, 0.1f, doorColor);

    // Door handle
    glm::mat4 handleTransform = glm::rotate(doorHinge, glm::radians(-doorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    cube->draw(shader, handleTransform, 0.2f, 1.2f, 0.15f, 0.0f, 0.0f, 90.0f, 0.3f, 0.05f, 0.05f, handleColor);

    // 6. CEILING LAMP
    ceilingLamp->draw(shader, identity, 1.5f, 4.0f, 1.0f, lampRotation, lampSwingAngle);
}

void render() {
    ourShader->use();

    glm::mat4 identity = glm::mat4(1.0f);
    
    // Viewport dimensions (half width, half height)
    int halfW = SCR_WIDTH / 2;
    int halfH = SCR_HEIGHT / 2;
    float aspect = (float)halfW / (float)halfH;

    // ============================================
    // TOP-LEFT VIEWPORT: Isometric View
    // ============================================
    glViewport(0, halfH, halfW, halfH);
    glScissor(0, halfH, halfW, halfH);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    {
        // Isometric camera position (elevated corner view)
        glm::vec3 isoPos = glm::vec3(12.0f, 10.0f, 12.0f);
        glm::vec3 isoTarget = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 isoView = glm::lookAt(isoPos, isoTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 isoProj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        
        ourShader->setMat4("projection", isoProj);
        ourShader->setMat4("view", isoView);
        setupLighting(*ourShader, isoPos);
        drawScene(*ourShader, identity);
    }

    // ============================================
    // TOP-RIGHT VIEWPORT: Top View (Bird's Eye)
    // ============================================
    glViewport(halfW, halfH, halfW, halfH);
    glScissor(halfW, halfH, halfW, halfH);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    {
        // Top-down view
        glm::vec3 topPos = glm::vec3(0.0f, 15.0f, 0.01f);
        glm::vec3 topTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 topView = glm::lookAt(topPos, topTarget, glm::vec3(0.0f, 0.0f, -1.0f));
        glm::mat4 topProj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        
        ourShader->setMat4("projection", topProj);
        ourShader->setMat4("view", topView);
        setupLighting(*ourShader, topPos);
        drawScene(*ourShader, identity);
    }

    // ============================================
    // BOTTOM-LEFT VIEWPORT: Front View
    // ============================================
    glViewport(0, 0, halfW, halfH);
    glScissor(0, 0, halfW, halfH);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    {
        // Front view (looking at the front wall/teacher's desk)
        glm::vec3 frontPos = glm::vec3(0.0f, 2.0f, 10.0f);
        glm::vec3 frontTarget = glm::vec3(0.0f, 1.5f, -5.0f);
        glm::mat4 frontView = glm::lookAt(frontPos, frontTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 frontProj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        
        ourShader->setMat4("projection", frontProj);
        ourShader->setMat4("view", frontView);
        setupLighting(*ourShader, frontPos);
        drawScene(*ourShader, identity);
    }

    // ============================================
    // BOTTOM-RIGHT VIEWPORT: Inside View (User Camera)
    // ============================================
    glViewport(halfW, 0, halfW, halfH);
    glScissor(halfW, 0, halfW, halfH);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    {
        // User-controlled camera (inside view)
        glm::mat4 insideView = camera.GetViewMatrix();
        glm::mat4 insideProj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
        
        ourShader->setMat4("projection", insideProj);
        ourShader->setMat4("view", insideView);
        setupLighting(*ourShader, camera.Position);
        drawScene(*ourShader, identity);
    }

    glDisable(GL_SCISSOR_TEST);
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

    // Door Control (Key Space for easy access, or define custom)
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
        doorOpen = !doorOpen;
        spacePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) spacePressed = false;

    // ========================================
    // LIGHTING CONTROLS
    // ========================================

    // Directional Light Toggle (Key 1)
    static bool key1Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed) {
        directionalLightOn = !directionalLightOn;
        cout << "Directional Light: " << (directionalLightOn ? "ON" : "OFF") << endl;
        key1Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) key1Pressed = false;

    // Point Lights Toggle (Key 2)
    static bool key2Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed) {
        pointLightOn = !pointLightOn;
        cout << "Point Lights: " << (pointLightOn ? "ON" : "OFF") << endl;
        key2Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) key2Pressed = false;

    // Spot Light Toggle (Key 3)
    static bool key3Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed) {
        spotLightOn = !spotLightOn;
        cout << "Spot Light: " << (spotLightOn ? "ON" : "OFF") << endl;
        key3Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) key3Pressed = false;

    // Ambient Toggle (Key 5)
    static bool key5Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !key5Pressed) {
        ambientOn = !ambientOn;
        cout << "Ambient: " << (ambientOn ? "ON" : "OFF") << endl;
        key5Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE) key5Pressed = false;

    // Diffuse Toggle (Key 6)
    static bool key6Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !key6Pressed) {
        diffuseOn = !diffuseOn;
        cout << "Diffuse: " << (diffuseOn ? "ON" : "OFF") << endl;
        key6Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE) key6Pressed = false;

    // Specular Toggle (Key 7)
    static bool key7Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !key7Pressed) {
        specularOn = !specularOn;
        cout << "Specular: " << (specularOn ? "ON" : "OFF") << endl;
        key7Pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_RELEASE) key7Pressed = false;

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