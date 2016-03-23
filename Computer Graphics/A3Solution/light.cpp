#include "light.h"
#include "camera.h"

Light::Light(){
}

void Light::Create( 
   const Vector3f& position,
   const Vector3f& ambient,
   const Vector3f& diffuse,
   const Vector3f& specular )
{
    m_position = Vector4f( position, 1.0f);

    float data[] = { 
        position.x, position.y, position.z, 1.0f,
        ambient.x, ambient.y, ambient.z, 1.0f,
        diffuse.x, diffuse.y, diffuse.z, 1.0f,
        specular.x, specular.y, specular.z, 1.0f,
    };
    for( int i = 0; i < sizeof(data)/sizeof(float); ++i )
        m_data[i] = data[i];

    //create light 
    glGenBuffers( 1, &m_lightVbo );
    glBindBuffer( GL_UNIFORM_BUFFER, m_lightVbo );
    glBufferData( GL_UNIFORM_BUFFER, sizeof(m_data), m_data, GL_DYNAMIC_DRAW );
}

void Light::Use( Camera& camera ){
    //update position
    Vector4f position = camera.GetViewTransform() * m_position;
    m_data[0] = position.x; 
    m_data[1] = position.y; 
    m_data[2] = position.z;

    //get the current program
    GLint id; glGetIntegerv(GL_CURRENT_PROGRAM, &id );
    GLuint idxBlock = glGetUniformBlockIndex( id, "Light" );

    //copy data
    glUniformBlockBinding( id, 0, idxBlock );
    glBindBuffer( GL_UNIFORM_BUFFER, m_lightVbo );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(m_data), m_data );
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_lightVbo );
}

void Light::SetAmbient( const Vector3f& ambient ){
    m_data[4] = ambient.x;
    m_data[5] = ambient.y;
    m_data[6] = ambient.z;
}

void Light::SetDiffuse( const Vector3f& diffuse ){
    m_data[8] = diffuse.x;
    m_data[9] = diffuse.y;
    m_data[10] = diffuse.z;
}

void Light::SetSpecular( const Vector3f& specular ){
    m_data[12] = specular.x;
    m_data[13] = specular.y;
    m_data[14] = specular.z;
}