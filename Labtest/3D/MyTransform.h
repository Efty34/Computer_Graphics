#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H

#include <glm/glm.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 1. Identity Matrix
// Returns a standard 4x4 Identity Matrix
inline glm::mat4 myIdentity() {
    glm::mat4 m(0.0f); // Initialize all to 0
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    return m;
}

// 2. Custom Translation
// Multiplies the input matrix 'm' by a translation matrix created from v(x,y,z)
inline glm::mat4 myTranslate(glm::mat4 m, glm::vec3 v) {
    glm::mat4 tMat = myIdentity();

    // Column 3 (Index 3) stores translation in Column-Major order
    tMat[3][0] = v.x;
    tMat[3][1] = v.y;
    tMat[3][2] = v.z;

    // Result = m * translation
    return m * tMat;
}

// 3. Custom Scaling
// Multiplies the input matrix 'm' by a scaling matrix
inline glm::mat4 myScale(glm::mat4 m, glm::vec3 v) {
    glm::mat4 sMat = myIdentity();

    sMat[0][0] = v.x;
    sMat[1][1] = v.y;
    sMat[2][2] = v.z;

    // Result = m * scale
    return m * sMat;
}

// 4. Custom Rotation (Arbitrary Axis)
// Implements Rodrigues' Rotation Formula manually
inline glm::mat4 myRotate(glm::mat4 m, float angleRadians, glm::vec3 axis) {
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    float omc = 1.0f - c; // (1 - cos)

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    glm::mat4 rMat(0.0f);

    // Column 0
    rMat[0][0] = x * x * omc + c;
    rMat[0][1] = y * x * omc + z * s;
    rMat[0][2] = x * z * omc - y * s;
    rMat[0][3] = 0.0f;

    // Column 1
    rMat[1][0] = x * y * omc - z * s;
    rMat[1][1] = y * y * omc + c;
    rMat[1][2] = y * z * omc + x * s;
    rMat[1][3] = 0.0f;

    // Column 2
    rMat[2][0] = x * z * omc + y * s;
    rMat[2][1] = y * z * omc - x * s;
    rMat[2][2] = z * z * omc + c;
    rMat[2][3] = 0.0f;

    // Column 3
    rMat[3][0] = 0.0f;
    rMat[3][1] = 0.0f;
    rMat[3][2] = 0.0f;
    rMat[3][3] = 1.0f;

    return m * rMat;
}

#endif