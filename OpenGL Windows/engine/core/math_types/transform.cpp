#include "transform.hpp"

using namespace CEngine;

Transform::Transform() 
: position(0.0f), scale(1.0f) {
    Quaternion identity = Quaternion();
    // initialize both rotations in zeros
    Rotation(identity);
    SetGlobalRotation(identity);
}

Transform::Transform(const Transform& other)
: position(other.position), scale(other.scale),
  forward(other.forward), right(other.right), up(other.up), 
  rotation(other.rotation), globalRotation(other.globalRotation), 
  rotationMatrix(other.rotationMatrix), globalRotationMatrix(other.globalRotationMatrix) { }

/**
 * Translates position by given direction
*/
void Transform::Translate(const Vector3& dir) {
    position += dir;
}

/**
 * Updates position with new given
*/
void Transform::Position(const Vector3& newPos) {
    position = newPos;
}

/**
 * Updates scale with new given
*/
void Transform::Scale(const Vector3& newScale) {
    scale = newScale;
}

/**
 * Updates scale uniformly
*/
void Transform::Scale(const float& s) {
    scale = s * scale;
}

/**
 * Overrides current rotation with given, in degrees
*/
void Transform::EulerAngles(const Vector3& eulers) {
    Rotation( Quaternion(eulers) );
}

/**
 * Rotates in LOCAL space by amount euler angles given
 * @param deltaEulers Rotation made (euler angles) in degrees
*/
void Transform::RotateEulers(const Vector3& deltaEulers) {
    Rotate( Quaternion(deltaEulers) );
}

/**
 * Overrides current rotation with given
*/
void Transform::Rotation(const Quaternion& rot) {

    rotation = rot;
    rotationMatrix = Matrix4::FromQuaternion(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in LOCAL space by amount given
 * @param deltaRot Rotation made (quaternion)
*/
void Transform::Rotate(const Quaternion& deltaRot) {
    rotation = rotation * deltaRot;
    rotationMatrix = Matrix4::FromQuaternion(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in GLOBAL space by angle in given axis
 * @param angleDelta angle (degrees) to rotate
 * @param axis Rotation will be done around given axis
*/
void Transform::Rotate(const float& angleDelta, const Vector3& axis) {
    globalRotation = globalRotation * Quaternion::AngleAxis(angleDelta, axis);
    globalRotationMatrix = Matrix4::FromQuaternion(globalRotation);

    ComputeForwardAndRight();
}

void Transform::SetGlobalRotation(const Quaternion& globalRot) {
    globalRotation = globalRot;
    globalRotationMatrix = Matrix4::FromQuaternion(globalRotation);

    ComputeForwardAndRight();
}

/**
* Rotates in
*/
void Transform::Forward(const Vector3& dir) {
    float angleX, angleY, angleZ;
    Vector3 uDir = dir.Normalized();
    Vector2 dirXZ(uDir.X(), uDir.Z()), 
            dirZY(uDir.Z(), uDir.Y()),
            _forward(forward.X(), forward.Z());

    // all of these are in LOCAL space
    angleY = 90 - 90 * Vector2::Dot(dirXZ, _forward); // convert dot result to degrees [0-180]
    angleX = 90 - 90 * Vector2::Dot(dirZY, _forward);
    // Z angle is ignored since its rotation does not affect the forward vector
    angleZ = EulerAngles().Z();
    EulerAngles(Vector3(angleX, angleY, angleZ));
}

/**
 * Gets position's copy
*/
Vector3 Transform::Position() const { return position; }

/**
 * Gets scale's copy
*/
Vector3 Transform::Scale() const { return scale;  }

/**
 * Gets rotation's copy
*/
Quaternion Transform::Rotation() const { return rotation; }

/**
 * Gets local rotation as euler angles
*/
Vector3 Transform::EulerAngles() const { 
    return rotation.EulerAngles(); 
}

/**
 * Gets rotation in world space as copy
*/
Quaternion Transform::GlobalRotation() const { return globalRotation; }

/**
 * Gets global rotation as euler angles
*/
Vector3 Transform::GlobalEulersAngles() const {
    return globalRotation.EulerAngles();
}

/**
 * Gets forward's vector copy
*/
Vector3 Transform::Forward() const { return forward; }

/**
 * Gets right's vector copy
*/
Vector3 Transform::Right() const { return right;  }

/**
 * Gets up's vector copy
*/
Vector3 Transform::Up() const { return up; }

/**
 * Gets Model matrix of this transform
*/
Matrix4 Transform::ModelMatrix() const {
    // at last: translate
    Matrix4 model = Matrix4::Translate(Matrix4::Indentity(), position);
    // third: global space rotation
    model = model * globalRotationMatrix;
    // second: rotate in local space
    model = model * rotationMatrix;
    // first scale
    model = Matrix4::Scale(model, scale);

    return model;
}

/**
 * Computes new forward vector based on given rotation (radians)
 * @param radX Rotation on X axis
 * @param radY Rotation on Y axis
*/
void Transform::ComputeForwardAndRight() {
    Matrix4 rotMat = globalRotationMatrix * rotationMatrix;
    Vector4 f, u, r;
    // rotate global forward by local rot
    f = Matrix4::Multiply( rotMat, Vector4(-Vector3::Forward(), 0.0f) );
    // rotate global right by local rot
    r = Matrix4::Multiply( rotMat, Vector4(Vector3::Right(), 0.0f) );
    // rotate global up by local rot
    u = Matrix4::Multiply( rotMat, Vector4(Vector3::Up(), 0.0f) );

    forward = f.ToVector3();
    right = r.ToVector3();
    up = u.ToVector3();
}

Transform& Transform::operator= (const Transform& other) {
    this->position = other.position;
    this->scale = other.scale;

    this->rotation = other.rotation;
    this->rotationMatrix = other.rotationMatrix;

    this->globalRotation = other.globalRotation;
    this->globalRotationMatrix = other.globalRotationMatrix;

    this->forward = other.forward;
    this->right = other.right;
    this->up = other.up;
    return *this;
}