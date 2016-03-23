#include "material.h"
#include "nuss_vector.h"

Material::Material(){
}

void Material::Create(
    const Vector3f& ambient,
    const Vector3f& diffuse,
    const Vector3f& specular, 
    float shininess){
    float data[] = { 
        ambient.x, ambient.y, ambient.z, 1.0f,
        diffuse.x, diffuse.y, diffuse.z, 1.0f,
        specular.x, specular.y, specular.z, 1.0f,
        shininess
    };
    for( int i = 0; i < sizeof(data)/sizeof(float); ++i )
        m_data[i] = data[i];

    //create material 
    glGenBuffers( 1, &m_matVbo );
    glBindBuffer( GL_UNIFORM_BUFFER, m_matVbo );
    glBufferData( GL_UNIFORM_BUFFER, sizeof(m_data), m_data, GL_DYNAMIC_DRAW );
}

void Material::Use(){
    //get the current program
    GLint id; glGetIntegerv(GL_CURRENT_PROGRAM, &id );
    GLuint idxBlock = glGetUniformBlockIndex( id, "Material" );

    //copy data
    glUniformBlockBinding( id, 1, idxBlock );
    glBindBuffer( GL_UNIFORM_BUFFER, m_matVbo );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(m_data), m_data );
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_matVbo );
}

void Material::SetAmbient( const Vector3f& ambient ){
    m_data[0] = ambient.x;
    m_data[1] = ambient.y;
    m_data[2] = ambient.z;
}

void Material::SetDiffuse( const Vector3f& diffuse ){
    m_data[4] = diffuse.x;
    m_data[5] = diffuse.y;
    m_data[6] = diffuse.z;
}

void Material::SetSpecular( const Vector3f& specular ){
    m_data[8] = specular.x;
    m_data[9] = specular.y;
    m_data[10] = specular.z;
}

void Material::SetShininess( float shininess ){
    m_data[12] = shininess;
}