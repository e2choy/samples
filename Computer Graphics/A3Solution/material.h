#ifndef _MATERIAL_
#define _MATERIAL_
#include "glew.h"

struct Vector3f;
class Material{
public:
    Material();
    void Create(
        const Vector3f& ambient,    
        const Vector3f& diffuse,   
        const Vector3f& specular, 
        float shininess 
        );

    void        Use();
    void        SetAmbient( const Vector3f& );
    void        SetDiffuse( const Vector3f& );
    void        SetSpecular( const Vector3f& );
    void        SetShininess( float shininess );
private:
    float       m_data[13];
    GLuint      m_matVbo;
};
#endif