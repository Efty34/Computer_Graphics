#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float PI = 3.14159265359f;

// global variables for transformations
glm::vec3 carPosition(0.0f, 0.0f, 0.0f);
float carRotation = 0.0f;
float carScale = 1.0f;
float wheelRotation = 0.0f;

// window interaction states
float frontWindowOffset = 0.0f;
float rearWindowOffset = 0.0f;

// shader helper class
class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath) {
        string vertexCode, fragmentCode;
        ifstream vShaderFile, fShaderFile;

        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (ifstream::failure& e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use() { glUseProgram(ID); }
    void setMat4(const string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setVec3(const string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
};

// geometry functions

void generateCircle(vector<float>& vertices, float radius, int segments) {
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * PI * float(i) / float(segments);
        vertices.push_back(radius * cosf(theta));
        vertices.push_back(radius * sinf(theta));
        vertices.push_back(0.0f);
    }
}

void generateSpokes(vector<float>& vertices, float radius) {
    float lines[] = { -radius, 0.0f, 0.0f, radius, 0.0f, 0.0f, 0.0f, -radius, 0.0f, 0.0f, 0.0f, radius, 0.0f };
    vertices.insert(vertices.end(), begin(lines), end(lines));
}

void generateCarChassis(vector<float>& vertices) {
    float profile[] = {
        0.0f, 0.0f, 0.0f,
       -0.4f, -0.15f, 0.0f,
        0.4f, -0.15f, 0.0f,
        0.4f,  0.05f, 0.0f,
        0.2f,  0.08f, 0.0f,
        0.1f,  0.25f, 0.0f,
       -0.2f,  0.25f, 0.0f,
       -0.3f,  0.08f, 0.0f,
       -0.4f,  0.05f, 0.0f,
       -0.4f, -0.15f, 0.0f
    };
    vertices.insert(vertices.end(), begin(profile), end(profile));
}

void generateFrontWindow(vector<float>& vertices) {
    float frontWindow[] = {
        0.02f, 0.10f, 0.0f,
        0.18f, 0.10f, 0.0f,
        0.12f, 0.23f, 0.0f,

        0.02f, 0.10f, 0.0f,
        0.12f, 0.23f, 0.0f,
        0.02f, 0.23f, 0.0f
    };
    vertices.insert(vertices.end(), begin(frontWindow), end(frontWindow));
}

void generateRearWindow(vector<float>& vertices) {
    float rearWindow[] = {
      -0.26f, 0.10f, 0.0f,
      -0.04f, 0.10f, 0.0f,
      -0.04f, 0.23f, 0.0f,

      -0.26f, 0.10f, 0.0f,
      -0.04f, 0.23f, 0.0f,
      -0.20f, 0.23f, 0.0f
    };
    vertices.insert(vertices.end(), begin(rearWindow), end(rearWindow));
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    // Car Movement
    float carSpeed = 0.005f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) carPosition.y += carSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) carPosition.y -= carSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) carPosition.x -= carSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) carPosition.x += carSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) carRotation += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) carRotation -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) carScale += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) carScale -= 0.001f;

    // Wheel Rotation
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) wheelRotation -= 3.0f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) wheelRotation += 3.0f;

    // Window Controls
    float windowSpeed = 0.002f;
    float maxDownOffset = -0.13f;

    // Front Window
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        frontWindowOffset -= windowSpeed;
        if (frontWindowOffset < maxDownOffset) frontWindowOffset = maxDownOffset;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        frontWindowOffset += windowSpeed;
        if (frontWindowOffset > 0.0f) frontWindowOffset = 0.0f;
    }

    // Rear Window
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        rearWindowOffset -= windowSpeed;
        if (rearWindowOffset < maxDownOffset) rearWindowOffset = maxDownOffset;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        rearWindowOffset += windowSpeed;
        if (rearWindowOffset > 0.0f) rearWindowOffset = 0.0f;
    }
}

void setupVAO(unsigned int& VAO, unsigned int& VBO, const vector<float>& vertices) {
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 1: Assignment", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    Shader ourShader("basic.vert", "basic.frag");

    // Generate Geometry Data
    vector<float> chassisVerts;   generateCarChassis(chassisVerts);
    vector<float> frontWinVerts;  generateFrontWindow(frontWinVerts);
    vector<float> rearWinVerts;   generateRearWindow(rearWinVerts);
    vector<float> tireVerts;      generateCircle(tireVerts, 0.13f, 36);
    vector<float> hubVerts;       generateCircle(hubVerts, 0.07f, 36);
    vector<float> spokeVerts;     generateSpokes(spokeVerts, 0.07f);

    // Setup VAOs/VBOs
    unsigned int VAO_Chassis, VBO_Chassis, VAO_FWin, VBO_FWin, VAO_RWin, VBO_RWin, VAO_Tire, VBO_Tire, VAO_Hub, VBO_Hub, VAO_Spoke, VBO_Spoke;
    setupVAO(VAO_Chassis, VBO_Chassis, chassisVerts);
    setupVAO(VAO_FWin, VBO_FWin, frontWinVerts);
    setupVAO(VAO_RWin, VBO_RWin, rearWinVerts);
    setupVAO(VAO_Tire, VBO_Tire, tireVerts);
    setupVAO(VAO_Hub, VBO_Hub, hubVerts);
    setupVAO(VAO_Spoke, VBO_Spoke, spokeVerts);

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();

        glm::mat4 projection = glm::ortho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", glm::mat4(1.0f));

        // Calculate Base Car Transform
        glm::mat4 modelCar = glm::mat4(1.0f);
        modelCar = glm::translate(modelCar, carPosition);
        modelCar = glm::rotate(modelCar, glm::radians(carRotation), glm::vec3(0.0f, 0.0f, 1.0f));
        modelCar = glm::scale(modelCar, glm::vec3(carScale, carScale, 1.0f));

        // Draw Car Body
        ourShader.setMat4("model", modelCar);
        ourShader.setVec3("objectColor", glm::vec3(0.1f, 0.7f, 0.1f));
        glBindVertexArray(VAO_Chassis);
        glDrawArrays(GL_TRIANGLE_FAN, 0, chassisVerts.size() / 3);

        // Draw Windows
        ourShader.setVec3("objectColor", glm::vec3(0.6f, 0.8f, 1.0f));

        // Front Window
        glm::mat4 modelFWindow = modelCar;
        modelFWindow = glm::translate(modelFWindow, glm::vec3(0.0f, frontWindowOffset, 0.0f));
        ourShader.setMat4("model", modelFWindow);
        glBindVertexArray(VAO_FWin);
        glDrawArrays(GL_TRIANGLES, 0, frontWinVerts.size() / 3);

        // Rear Window
        glm::mat4 modelRWindow = modelCar;
        modelRWindow = glm::translate(modelRWindow, glm::vec3(0.0f, rearWindowOffset, 0.0f));
        ourShader.setMat4("model", modelRWindow);
        glBindVertexArray(VAO_RWin);
        glDrawArrays(GL_TRIANGLES, 0, rearWinVerts.size() / 3);

        // Draw Wheels
        auto drawWheel = [&](glm::vec3 relativePos) {
            glm::mat4 modelWheel = modelCar;
            modelWheel = glm::translate(modelWheel, relativePos);
            modelWheel = glm::rotate(modelWheel, glm::radians(wheelRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            ourShader.setMat4("model", modelWheel);

            ourShader.setVec3("objectColor", glm::vec3(0.15f, 0.15f, 0.15f));
            glBindVertexArray(VAO_Tire);
            glDrawArrays(GL_TRIANGLE_FAN, 0, tireVerts.size() / 3);

            ourShader.setVec3("objectColor", glm::vec3(0.7f, 0.7f, 0.7f));
            glBindVertexArray(VAO_Hub);
            glDrawArrays(GL_TRIANGLE_FAN, 0, hubVerts.size() / 3);

            glLineWidth(3.0f);
            ourShader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
            glBindVertexArray(VAO_Spoke);
            glDrawArrays(GL_LINES, 0, spokeVerts.size() / 3);
            glLineWidth(1.0f);
            };

        drawWheel(glm::vec3(-0.26f, -0.15f, 0.0f));
        drawWheel(glm::vec3(0.26f, -0.15f, 0.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}