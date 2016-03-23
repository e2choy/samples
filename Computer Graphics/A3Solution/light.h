#ifndef _LIGHT_
#define _LIGHT_
#include "glew.h"
#include "nuss_vector.h"

class Shader;
class Camera;

class Light{
public:
    Light();
    void Create(
       const Vector3f& position,
       const Vector3f& ambient,
       const Vector3f& diffuse,
       const Vector3f& specular );

    void Use( Camera& camera );
    void SetAmbient( const Vector3f& ambient );
    void SetDiffuse( const Vector3f& diffuse );
    void SetSpecular( const Vector3f& specular );

private:
    float       m_data[16];
    GLuint      m_lightVbo;
    Vector4f    m_position;     
};

#endif