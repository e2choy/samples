/*
    This program is distributed for education purposes only and WITHOUT ANY WARRANTY.

    This file is governed by the GNU General Public License.

    Copyright 2014 Doron Nussbaum

    */

#ifndef NUSS_MATRIX
#define NUSS_MATRIX

#include <stdio.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include "assert.h"
#include "nuss_vector.h"

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))
#define RadianToDegrees(x) (float)(((x) * 180.0f / M_PI))
///////////////////////////////////////////////////////////////////////////////
class Matrix4f
{
public:
    Vector4f vm[4];

    Matrix4f()
    {
        reset(0.0);
    }

    Matrix4f(Vector4f v0, Vector4f v1, Vector4f v2, Vector4f v3)
    {
        vm[0] = v0;
        vm[1] = v1;
        vm[2] = v2;
        vm[3] = v3;
    }

    const float* data() const{
        return (float*)(&vm);
    }
    /********************************************************************************/
    // static functions
    // reurns an identity matrix 
    static Matrix4f identity()
    {
        Matrix4f m1;
        m1.reset(0);
        m1.vm[0].x = m1.vm[1].y = m1.vm[2].z = m1.vm[3].w = 1;
        return(m1);

    }

    // creates a transpose of the matrix
    // retruns a transposed matrix
    // note it does not change the internal values of the matrix
    static Matrix4f transpose(Matrix4f m1)
    {
        Matrix4f m2;
        m2.vm[0] = Vector4f(m1.vm[0].x, m1.vm[1].x, m1.vm[2].x, m1.vm[3].x);
        m2.vm[1] = Vector4f(m1.vm[0].y, m1.vm[1].y, m1.vm[2].y, m1.vm[3].y);
        m2.vm[2] = Vector4f(m1.vm[0].z, m1.vm[1].z, m1.vm[2].z, m1.vm[3].z);
        m2.vm[3] = Vector4f(m1.vm[0].w, m1.vm[1].w, m1.vm[2].w, m1.vm[3].w);
        return(m2);
    }

    // return a scale matrix.  Note internal parameters are not changes
    // Note it resets the data first
    static Matrix4f scale(float scaleX, float scaleY, float scaleZ)
    {
        Matrix4f m1 = identity();
        m1.vm[0].x = scaleX;
        m1.vm[1].y = scaleY;
        m1.vm[2].z = scaleZ;
        return m1;
    }

    // set the matrix as a pitch rotation matrix
    // if degree is true (!=0) then angle is given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateX(float angle, bool isDegree = true)
    {
        Matrix4f m1;
        if (isDegree) angle = DegreeToRadians(angle);
        m1 = identity();
        m1.vm[1].y = m1.vm[2].z = cos(angle);
        m1.vm[1].z = -sin(angle);
        m1.vm[2].y = -m1.vm[1].z;
        return(m1);
    }

    // set the matrix as a yaw rotation matrix
    // if degree is true (!=0) then angle is given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateY(float angle, bool isDegree = true)
    {
        Matrix4f m1;
        if (isDegree) angle = DegreeToRadians(angle);
        m1 = identity();
        m1.vm[0].x = m1.vm[2].z = cos(angle);
        m1.vm[0].z = sin(angle);
        m1.vm[2].x = -m1.vm[0].z;
        return(m1);
    }

    // set the matrix as a roll rotation matrix
    // if degree is true (!=0) then angle is given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateZ(float angle, bool isDegree = true)
    {
        Matrix4f m1;
        if (isDegree) angle = DegreeToRadians(angle);
        m1 = identity();
        m1.vm[0].x = m1.vm[1].y = cos(angle);
        m1.vm[0].y = -sin(angle);
        m1.vm[1].x = -m1.vm[0].y;
        return(m1);
    }

    // set the matrix as a roll pitch and yaw rotation matrix
    // the resulting matrix M=Mat(yaw)*mat(pitch)*mat(roll)
    // if degree is true (!=0) then all angles are given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateRollPitchYaw(float angleRoll, float anglePitch, float angleYaw, bool degree)
    {
        Matrix4f m1;
        m1 = identity();
        m1 = rotateY(angleYaw, degree)*rotateX(anglePitch, degree)*rotateZ(angleRoll, degree);
        return(m1);
    }

    // set the matrix as a rotation around a vector where only a rotation vector is given
    // if degree is true (!=0) then all angles are given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateVector(Vector3f u, float angle, bool isDegree = true)
    {
        u.normalize();
        if (isDegree) angle = DegreeToRadians(angle);
        float x = u.x;
        float y = u.y;
        float z = u.z;
        float c = cosf(angle);
        float s = sinf(angle);

        Matrix4f m1;
        m1 = identity();
        m1.vm[0] = Vector4f(c + x*x*(1 - c), x*y*(1 - c) - z*s, x*z*(1 - c) + y*s, 0);
        m1.vm[1] = Vector4f(x*y*(1 - c) + z*s, c + y*y*(1 - c), y*z*(1 - c) - x*s, 0);
        m1.vm[2] = Vector4f(z*x*(1 - c) - y*s, z*y*(1 - c) + x*s, c + z*z*(1 - c), 0);
        m1.vm[3] = Vector4f(0, 0, 0, 1);
        return(m1);
    }

    // set the matrix as a rotation around a vector where only a rotation vector is given
    // if degree is true (!=0) then all angles are given in degrees
    // otherwise angle is given in radians
    static Matrix4f rotateVector(Vector4f u, float angle, bool isDegree = true)
    {
        Matrix4f m1;
        Vector3f v = Vector3f(u.x, u.y, u.z);
        m1 = rotateVector(v, angle, isDegree);
        return(m1);
    }

    static Matrix4f translation(float dx, float dy, float dz)
    {
        Matrix4f m1;
        m1 = identity();
        m1.vm[0].w = dx;
        m1.vm[1].w = dy;
        m1.vm[2].w = dz;
        return(m1);
    }

    static Matrix4f cameraMatrix(Vector3f position, Vector3f lookAt, Vector3f up)
    {
        Matrix4f ret;

        //position vector
        Vector3f w = (position - lookAt).normalize();
        Vector3f u = cross(up, w).normalize();
        Vector3f v = cross(w, u);

        //define the translation matrix
        Matrix4f translation = Matrix4f::translation(-1 * position.x, -1 * position.y, -1 * position.z);

        //define the rotation matrix
        Matrix4f rotation = Matrix4f(
            Vector4f(u, 0),
            Vector4f(v, 0),
            Vector4f(w, 0),
            Vector4f(0, 0, 0, 1)
            );

        //camera is rotation * translation * pt
        ret = rotation * translation;
        return ret;
    }

    static Matrix4f cameraMatrix(const Vector3f& position, const Vector3f& gaze, const Vector3f& right, const Vector3f& above){
        //define the translation matrix
        Matrix4f translation = Matrix4f::translation(-1 * position.x, -1 * position.y, -1 * position.z);

        //define the rotation matrix
        Matrix4f rotation = Matrix4f(
            Vector4f(right, 0),
            Vector4f(above, 0),
            Vector4f(-1 * gaze, 0),
            Vector4f(0, 0, 0, 1)
            );

        //camera is rotation * translation * pt
        Matrix4f ret = rotation * translation;
        return ret;
    }

    static Matrix4f symmetricPerspectiveProjectionMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
    {
        float t = DegreeToRadians(fovDegrees);
        float n = nearPlane;
        float f = farPlane;
        Matrix4f m1;
        m1 = Matrix4f(
            Vector4f(1.0f / (tan(t / 2) * aspectRatio), 0, 0, 0),
            Vector4f(0, 1.0f / tan(t / 2), 0, 0),
            Vector4f(0, 0, (-f - n) / (f - n), (-2 * f*n) / (f - n)),
            Vector4f(0, 0, -1, 0)
            );
        return(m1);
    }


    static Matrix4f frustumProjectionMatrix(float winMinX, float winMinY, float winMaxX, float winMaxY, float nearPlane, float farPlane)
    {
        float n = nearPlane;
        float f = farPlane;
        float l = winMinX;
        float r = winMaxX;
        float t = winMaxY;
        float b = winMinY;

        Matrix4f m1 = Matrix4f(
            Vector4f((2 * n) / (r - l), 0, (r + l) / (r - l), 0),
            Vector4f(0, (2 * n) / (t - b), (t + b) / (t - b), 0),
            Vector4f(0, 0, (-f - n) / (f - n), (-2 * f*n) / (f - n)),
            Vector4f(0, 0, -1, 0)
            );
        return m1;
    }

    // print the matrix (prints the vectors)
    friend std::ostream& operator << (std::ostream& os, const Matrix4f& m1)
    {
        int i;
        for (i = 0; i <= 3; i++) {
            os << m1.vm[i] << std::endl;
        }
        return(os);
    }


    /***********************************************************************/
    // resets all the values of the matrix to "value"
    inline void reset(float value)
    {
        // sets all 16 values of the matrix to value 
        int i;
        for (i = 0; i <= 3; i++) {
            vm[i] = Vector4f(value, value, value, value);
        }
    }

    /****************************************************************************************/
    // operators 
    inline Matrix4f operator*(const Matrix4f& rhs) const
    {
        Matrix4f m1, m2;
        int i, j;

        m2 = transpose(rhs);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                m1.vm[i][j] = dot(vm[i], m2.vm[j]);
            }
        }

        return(m1);
    }

    inline Matrix4f operator*(const float f) const
    {
        Matrix4f m1;
        int i;

        for (i = 0; i < 4; i++) {
            m1.vm[i] = vm[i] * f;
        }
        return m1;
    }


    friend Matrix4f operator*(const float f, Matrix4f m1)
    {
        return(m1*f);
    }

    inline Matrix4f operator+(const Matrix4f& rhs) const
    {
        Matrix4f m1, m2;
        int i;
        for (i = 0; i < 4; i++) {
            m1.vm[i] = vm[i] + rhs.vm[i];
        }

        return(m1);
    }

    inline Matrix4f operator-(const Matrix4f& rhs) const
    {
        Matrix4f m1, m2;
        int i;
        for (i = 0; i < 4; i++) {
            m1.vm[i] = vm[i] - rhs.vm[i];
        }
        return(m1);
    }

    // multiply the matrix by a vector 
    Vector4f operator*(const Vector4f& v) const
    {
        Vector4f r;
        r.x = dot(vm[0], v);
        r.y = dot(vm[1], v);
        r.z = dot(vm[2], v);
        r.w = dot(vm[3], v);
        return r;
    }
};

#endif    

