#include "camera.h"
#include "nuss_matrix.h"

void Camera::Roll(float angleDegrees){
    Matrix4f rot = Matrix4f::rotateVector(m_gaze, angleDegrees);
    Vector4f above = rot * Vector4f(m_above, 1);
    m_above = Vector3f(above.x, above.y, above.z).normalize();
    m_right = cross(m_gaze, m_above);
    m_transform = Matrix4f::cameraMatrix(m_position, m_gaze, m_right, m_above);
}

void Camera::Pitch(float angleDegrees){
    Matrix4f rot = Matrix4f::rotateVector(m_right, angleDegrees);
    Vector4f above = rot * Vector4f(m_above, 1);
    m_above = Vector3f(above.x, above.y, above.z).normalize();
    m_gaze = cross(m_above, m_right);
    m_transform = Matrix4f::cameraMatrix(m_position, m_gaze, m_right, m_above);
}

void Camera::Yaw(float angleDegrees){
    Matrix4f rot = Matrix4f::rotateVector(m_above, angleDegrees);
    Vector4f gaze = rot * Vector4f(m_gaze, 1);
    m_gaze = Vector3f(gaze.x, gaze.y, gaze.z).normalize();
    m_right = cross(m_gaze, m_above);
    m_transform = Matrix4f::cameraMatrix(m_position, m_gaze, m_right, m_above);
}

void Camera::Move(float val){
    m_position.x += m_gaze.x * val;
    m_position.y += m_gaze.y * val;
    m_position.z += m_gaze.z * val;
    m_transform = Matrix4f::cameraMatrix(m_position, m_gaze, m_right, m_above);
}

void Camera::SetCamera(
    const Vector3f& position,
    const Vector3f& lookAt,
    const Vector3f& up
    ){
    m_position = position;
    m_gaze = (lookAt - position).normalize();
    m_right = cross(m_gaze, up).normalize();
    m_above = cross(m_right, m_gaze);
    m_transform = Matrix4f::cameraMatrix(m_position, m_gaze, m_right, m_above);
}

const Matrix4f& Camera::GetViewTransform() const{
    return m_transform;
}
