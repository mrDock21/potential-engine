#include "transform.hpp"

Components::Transform::Transform() 
: position(0.0f), scale(1.0f) {
    Quaternion identity = Quaternion();
    // initialize both rotations in zeros
    SetRotation(identity);
    SetGlobalRotation(identity);
}

Components::Transform::Transform(const Components::Transform& other)
: position(other.position), scale(other.scale),
  forward(other.forward), right(other.right), up(other.up), 
  rotation(other.rotation), globalRotation(other.globalRotation), 
  rotationMatrix(other.rotationMatrix), globalRotationMatrix(other.globalRotationMatrix) { }

/**
 * Translates position by given direction
*/
void Components::Transform::Translate(const Vector3& dir) {
    position += dir;
}

/**
 * Updates position with new given
*/
void Components::Transform::SetPosition(const Vector3& newPos) {
    position = newPos;
}

/**
 * Updates scale with new given
*/
void Components::Transform::SetScale(const Vector3& newScale) {
    scale = newScale;
}

/**
 * Updates scale uniformly
*/
void Components::Transform::SetScale(const float& s) {
    scale = s * scale;
}

/**
 * Overrides current rotation with given, in degrees
*/
void Components::Transform::SetEulerAngles(const Vector3& eulers) {
    SetRotation( Quaternion(eulers) );
}

/**
 * Rotates in LOCAL space by amount euler angles given
 * @param deltaEulers Rotation made (euler angles) in degrees
*/
void Components::Transform::RotateEulers(const Vector3& deltaEulers) {
    Rotate( Quaternion(deltaEulers) );
}

/**
 * Overrides current rotation with given
*/
void Components::Transform::SetRotation(const Quaternion& rot) {

    rotation = rot;
    rotationMatrix = Matrix4::FromQuaternion(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in LOCAL space by amount given
 * @param deltaRot Rotation made (quaternion)
*/
void Components::Transform::Rotate(const Quaternion& deltaRot) {
    rotation = rotation * deltaRot;
    rotationMatrix = Matrix4::FromQuaternion(rotation);

    ComputeForwardAndRight();
}

/**
 * Rotates in GLOBAL space by angle in given axis
 * @param angleDelta angle (degrees) to rotate
 * @param axis Rotation will be done around given axis
*/
void Components::Transform::Rotate(const float& angleDelta, const Vector3& axis) {
    globalRotation = globalRotation * Quaternion::AngleAxis(angleDelta, axis);
    globalRotationMatrix = Matrix4::FromQuaternion(globalRotation);

    ComputeForwardAndRight();
}

void Components::Transform::SetGlobalRotation(const Quaternion& globalRot) {
    globalRotation = globalRot;
    globalRotationMatrix = Matrix4::FromQuaternion(globalRotation);

    ComputeForwardAndRight();
}

/**
 * Gets position's copy
*/
Vector3 Components::Transform::Position() const { return position; }

/**
 * Gets scale's copy
*/
Vector3 Components::Transform::Scale() const { return scale;  }

/**
 * Gets rotation's copy
*/
Quaternion Components::Transform::Rotation() const { return rotation; }

/**
 * Gets local rotation as euler angles
*/
Vector3 Components::Transform::EulerAngles() const { 
    return rotation.EulerAngles(); 
}

/**
 * Gets rotation in world space as copy
*/
Quaternion Components::Transform::GlobalRotation() const { return globalRotation; }

/**
 * Gets global rotation as euler angles
*/
Vector3 Components::Transform::GlobalEulersAngles() const {
    return globalRotation.EulerAngles();
}

/**
 * Gets forward's vector copy
*/
Vector3 Components::Transform::Forward() const { return forward; }

/**
 * Gets right's vector copy
*/
Vector3 Components::Transform::Right() const { return right;  }

/**
 * Gets up's vector copy
*/
Vector3 Components::Transform::Up() const { return up; }

/**
 * Gets Model matrix of this transform
*/
Matrix4 Components::Transform::ModelMatrix() const {
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
void Components::Transform::ComputeForwardAndRight() {
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

Components::Transform& Components::Transform::operator= (const Components::Transform& other) {
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