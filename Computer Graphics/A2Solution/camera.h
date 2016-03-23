#ifndef _CAMERA_
#define _CAMERA_
#include "nuss_matrix.h"
#include "nuss_vector.h"

class Camera{
public:
    void Roll   ( float angleDegrees );
    void Pitch  ( float angleDegrees );
    void Yaw    ( float angleDegrees );
    void Move   ( float val );

    void SetCamera( 
        const Vector3f& position,
        const Vector3f& lookAt,
        const Vector3f& up
        );

    const Matrix4f& GetViewTransform() const;
private:
    Vector3f m_position;
    Vector3f m_gaze;
    Vector3f m_right;
    Vector3f m_above;
    Matrix4f m_transform;	
};
#endif