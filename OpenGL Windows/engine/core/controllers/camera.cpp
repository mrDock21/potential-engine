#include "camera.hpp"

//using namespace CustomOps;

/**
 * Initializes camera at position (0, 0, 3) and looking towards negative Z
*/
Camera::Camera() 
    : transform()
      { }

/**
 * Initializes camera at given position
 * @param initialPos Initial position
*/
Camera::Camera(const Vector3& initialPos) : Camera() { 
    transform.Position(initialPos);
}

void Camera::Move(const Vector3& dir) {
    transform.Translate(dir);
}

/**
 * Will rotate camera's local X axis with angle
 * @param angleX Angle (degrees) to rotate in X axis
*/
void Camera::RotatePitch(const float& angleX) {
    Quaternion oldRot = transform.Rotation();
    Vector3 forward, deltaRot(angleX,  0.0f, 0.0f);

    transform.RotateEulers(deltaRot);
    forward = transform.Forward();

    if (forward.Y() > 0.95f || forward.Y() < -0.95f)
        transform.Rotation(oldRot);
}

/**
 * Will rotate camera's global Y axis with angle
 * @param angleY Angle (degrees) to rotate in global Y axis
*/
void Camera::RotateYaw(const float& angleY) {
    transform.Rotate(angleY, Vector3::Up());
}

/**
 * Will rotate camera's local Z axis with angle
 * @param angleZ Angle (degrees) to rotate in Z axis
*/
void Camera::RotateRoll(const float& angleZ) {
    transform.RotateEulers(Vector3(0.0f,  0.0f, angleZ));
}

const Components::Transform& Camera::Transform() const {
    return transform;
}

/**
 * Gets view matrix
*/
Matrix4 Camera::GetViewMatrix() const {
    Vector3 pos(transform.Position()), forward(transform.Forward());
    // we send position, target, and genric up
    return Matrix4::LookAt(pos, pos + forward, transform.Up());
}