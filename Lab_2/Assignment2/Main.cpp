#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h" 
#include "Cube.h"
#include "Table.h"
#include "Chair.h"
#include "Boundary.h"         

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
BasicCamera camera(glm::vec3(0.0f, 1.5f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- STATE VARIABLES FOR ASSIGNMENT ---
bool lightOn = true;          // Key L
bool fanOn = false;           // Key G
float fanAngle = 0.0f;
bool doorOpen = false;        // Key to toggle door (Using 'K' for door, or custom)
float doorAngle = 0.0f;
bool birdView = false;        // Key B for Birds Eye

// Rotation for LookAt (Key F)
float orbitAngle = 0.0f;
bool isOrbiting = false;

void printUsage() {
    cout << "=========================================================" << endl;
    cout << "   CSE 4208: Computer Graphics Laboratory - Assignment   " << endl;
    cout << "=========================================================" << endl;
    cout << "CAMERA CONTROLS:" << endl;
    cout << "  [W]             : Move Forward" << endl;
    cout << "  [S]             : Move Backward" << endl;
    cout << "  [A]             : Move Left" << endl;
    cout << "  [D]             : Move Right" << endl;
    cout << "  [E]             : Move Up (Fly Up)" << endl;
    cout << "  [R]             : Move Down (Fly Down)" << endl;
    cout << endl;
    cout << "ROTATION CONTROLS (Assignment Req 2):" << endl;
    cout << "  [X]             : Pitch (Look Up/Down)" << endl;
    cout << "  [Y]             : Yaw (Look Left/Right)" << endl;
    cout << "  [Z]             : Roll (Tilt Camera)" << endl;
    cout << "  [Shift + Z]     : Reset Roll" << endl;
    cout << endl;
    cout << "INTERACTION CONTROLS:" << endl;
    cout << "  [F]             : Orbit / Look At Center (Req 4)" << endl;
    cout << "  [G]             : Toggle Ceiling Fan (Req 5)" << endl;
    cout << "  [L]             : Toggle Lights On/Off (Req 5)" << endl;
    cout << "  [SPACE]         : Open/Close Door (Req 6)" << endl;
    cout << "  [B]             : Toggle Bird's Eye View (Req 1)" << endl;
    cout << "  [ESC]           : Exit Program" << endl;
    cout << "=========================================================" << endl;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment: 3D Lab", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Cube cube; // Our modular cube object
    
    // Room boundaries
    Boundary room; // Default room with walls and floor
    
    // Teacher's desk and chair (big, different color)
    Table teacherTable(2.5f, 1.2f, 0.8f, glm::vec3(0.3f, 0.2f, 0.15f)); // Big dark wood table
    Chair teacherChair(0.6f, 0.6f, 0.5f, glm::vec3(0.2f, 0.15f, 0.1f)); // Dark leather-like chair
    
    // Student desks and chairs (standard size, lighter color)
    Table studentTable(1.2f, 0.8f, 0.75f, glm::vec3(0.7f, 0.5f, 0.3f)); // Light wood table
    Chair studentChair(0.5f, 0.5f, 0.45f, glm::vec3(0.6f, 0.45f, 0.25f)); // Light wood chair

    printUsage();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // -- LOGIC UPDATES --
        if (fanOn) {
            fanAngle += 200.0f * deltaTime;
            if (fanAngle > 360.0f) fanAngle -= 360.0f;
        }

        // Smooth door opening
        if (doorOpen && doorAngle < 90.0f) doorAngle += 100.0f * deltaTime;
        if (!doorOpen && doorAngle > 0.0f) doorAngle -= 100.0f * deltaTime;

        // -- RENDER --
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setInt("lightOn", lightOn); // Pass light state

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 identity = glm::mat4(1.0f);

        // 1. ROOM (Floor and Walls)
        room.drawRoom(ourShader, identity);

        // 2. CLASSROOM SETUP
        // Teacher's desk at front (facing students)
        teacherTable.draw(ourShader, identity, -1.25f, 0.0f, -4.0f, 0.0f, 0.0f, 0.0f);
        teacherChair.draw(ourShader, identity, 0.40f, 0.0f, -4.3f, 0.0f, 180.0f, 0.0f); // Behind teacher's desk, centered
        
        // Student desks - Row 1 (closer to teacher)
        float row1Z = -2.0f;
        float row2Z = 0.5f;
        float spacing = 1.8f; // Space between desks
        float startX = -2.5f; // Starting X position for first desk
        
        // Row 1 - 3 desks
        for (int i = 0; i < 3; i++) {
            float xPos = startX + i * spacing;
            studentTable.draw(ourShader, identity, xPos, 0.0f, row1Z, 0.0f, 0.0f, 0.0f);
            studentChair.draw(ourShader, identity, xPos + 0.35f, 0.0f, row1Z + 0.9f, 0.0f, 0.0f, 0.0f); // Chair behind desk, backside visible
        }
        
        // Row 2 - 3 desks (behind row 1)
        for (int i = 0; i < 3; i++) {
            float xPos = startX + i * spacing;
            studentTable.draw(ourShader, identity, xPos, 0.0f, row2Z, 0.0f, 0.0f, 0.0f);
            studentChair.draw(ourShader, identity, xPos + 0.35f, 0.0f, row2Z + 0.9f, 0.0f, 0.0f, 0.0f); // Chair behind desk, backside visible
        }

        // 4. CEILING FAN (Hierarchical)
        glm::mat4 fanBase = glm::translate(identity, glm::vec3(0.0f, 3.5f, 0.0f)); // Center of room ceiling
        // Hub
        cube.draw(ourShader, fanBase, -0.1f, 0.0f, -0.1f, 0.0f, fanAngle, 0.0f, 0.2f, 0.5f, 0.2f, glm::vec3(0.2f, 0.2f, 0.2f));

        // Blades (rotate around hub)
        glm::mat4 bladeCenter = glm::rotate(fanBase, glm::radians(fanAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        // Blade 1
        cube.draw(ourShader, bladeCenter, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        // Blade 2
        cube.draw(ourShader, bladeCenter, 0.1f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        // Blade 3
        cube.draw(ourShader, bladeCenter, 0.0f, 0.0f, -0.1f, 0.0f, 180.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        // Blade 4
        cube.draw(ourShader, bladeCenter, -0.1f, 0.0f, 0.0f, 0.0f, 270.0f, 0.0f, 0.2f, 0.05f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));

        // 5. DOOR (Animating)
        // Hinge is at the left side of the door
        glm::mat4 doorHinge = glm::translate(identity, glm::vec3(-2.0f, 0.0f, -5.0f)); // On back wall
        cube.draw(ourShader, doorHinge, 0.0f, 0.0f, 0.0f, 0.0f, doorAngle, 0.0f, 1.0f, 2.5f, 0.1f, glm::vec3(0.4f, 0.2f, 0.1f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
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

    // Roll (Key Z)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera.ProcessRoll(20.0f * deltaTime);
    // Reset Roll (Key X or Shift+Z)
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) camera.ProcessRoll(-20.0f * deltaTime);

    // Fan Control (Key G)
    static bool gPressed = false;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gPressed) {
        fanOn = !fanOn;
        gPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) gPressed = false;

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
        camera.Position = glm::vec3(0.0f, 10.0f, 0.0f); // High up
        camera.Yaw = -90.0f;
        camera.Pitch = -90.0f; // Look straight down
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
        // Force camera to look at center
        camera.Front = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - camera.Position);
        camera.updateCameraVectors(); // Note: this might conflict with mouse movement if not handled carefully
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}