#include <glew.h>
#include <cstddef>
#include <random>
#include <iterator>

#include "model.h"
#include "shader.h"

///////////////////////////////////////////////////////////////////////////////
static std::default_random_engine engine(1);
float Random(){
    std::uniform_real_distribution<float> dist;
    return dist(engine);
}

Vertex::Vertex( const Vector3f& _pos ){
    pos[0] = _pos.x;
    pos[1] = _pos.y;
    pos[2] = _pos.z;
    pos[3] = 1.0f;
    col[0] = Random(); 
    col[1] = Random(); 
    col[2] = Random(); 
    col[3] = 1.0f;
}
///////////////////////////////////////////////////////////////////////////////
Model::Model():m_shader(nullptr),m_vao(-1),m_vbo(-1),m_idxVbo(-1){
}

void Model::Create( Shader& shader ){
    //create vertex array object
    glGenVertexArrays( 1, &m_vao );
    glBindVertexArray( m_vao );

    //create vertex buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer( GL_ARRAY_BUFFER, m_vbo );		
    glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    //copy the vertex position
    GLint vtxLoc = glGetAttribLocation( shader.GetProgId(), "vertex");
    glEnableVertexAttribArray( vtxLoc );
    glVertexAttribPointer( vtxLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    //copy the vertex color
    GLint colLoc = glGetAttribLocation( shader.GetProgId(), "color");
    glEnableVertexAttribArray( colLoc );
    glVertexAttribPointer( colLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, col));

    //create index buffer
    glGenBuffers( 1, &m_idxVbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_idxVbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW );

    //end creation
    glBindVertexArray( 0 );
}

void Model::Draw(){
    glBindVertexArray( m_vao );
    glDrawElements( GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL );
    glBindVertexArray( 0 );
}
///////////////////////////////////////////////////////////////////////////////
void Sphere::Create(
        Shader& shader,
        int rings,
        int sectors ){
    m_vertices.resize( rings * sectors );
    m_indices.resize( (rings-1) * (sectors-1) * 6 );

    //create the vertices
    float R = 1.0f / ( rings - 1.0f );
    float S = 1.0f / ( sectors - 1.0f );
    int i = 0;
    for( int r = 0; r < rings; ++r ){
        for( int s = 0; s < sectors; ++s ){
            float x = (float) (cos( 2 * M_PI * s * S ) * sin( M_PI * r * R ));
            float y = (float) sin( -M_PI_2 + M_PI * r * R);
            float z = (float) (sin( 2 * M_PI * s * S ) * sin( M_PI * r * R));
            m_vertices[i] = Vertex( Vector3f( x, y, z ) );
            ++i;
        }
    }

    //create the indices
    i = 0;
    for( int r = 0; r < rings - 1; ++r ){
        for( int s = 0; s < sectors - 1; ++s ){
            m_indices[i++] = r * sectors + s; 
            m_indices[i++] = ( r + 1 ) * sectors + ( s + 1 ); 
            m_indices[i++] = r * sectors + ( s + 1 ); 

            m_indices[i++] = r * sectors + s; 
            m_indices[i++] = ( r + 1 ) * sectors + s; 
            m_indices[i++] = ( r + 1 ) * sectors + ( s + 1 ); 
        }
    }
    Model::Create( shader );
}
///////////////////////////////////////////////////////////////////////////////
void Cylinder::Create(
        Shader& shader,
        int sectors ){
    m_vertices.resize( (sectors * 2) + 2 );
    m_indices.resize( (sectors - 1) * 12 );

    //create the vertices
    float S = 1.0f / ( sectors - 1.0f );
    int i = 0;
    for( int s = 0; s < sectors; ++s ){
        float x = (float) cos( 2 * M_PI * s * S );
        float z = (float) sin( 2 * M_PI * s * S );
        m_vertices[s]           = Vertex( Vector3f( x, 1.0f, z ) );
        m_vertices[s+sectors]   = Vertex( Vector3f( x, -1.0f, z ) );
    }
    m_vertices[sectors*2]           = Vertex( Vector3f( 0, 1.0f, 0 ));
    m_vertices[sectors*2 + 1]       = Vertex( Vector3f( 0, -1.0f, 0 ));

    //create the indices
    i = 0;
    for( int s = 0; s < sectors - 1; ++s ){
        m_indices[i++] = s + 1; 
        m_indices[i++] = s + sectors;
        m_indices[i++] = s; 

        m_indices[i++] = s + 1 + sectors;
        m_indices[i++] = s + sectors;
        m_indices[i++] = s + 1;
    }
    for( int s = 0; s < sectors - 1; ++s ){
        m_indices[i++] = s + 1;
        m_indices[i++] = s;
        m_indices[i++] = sectors * 2;

        m_indices[i++] = s + 1 + sectors;
        m_indices[i++] = s + sectors;
        m_indices[i++] = sectors * 2 + 1;
    }
    Model::Create( shader );
}
///////////////////////////////////////////////////////////////////////////////
void Cube::Create( Shader& shader ){
    m_vertices.resize( 8 );
    m_vertices[0] = Vertex( Vector3f(-1, -1, 1) );
    m_vertices[1] = Vertex( Vector3f( 1, -1, 1) );
    m_vertices[2] = Vertex( Vector3f( 1,  1, 1) ); 
    m_vertices[3] = Vertex( Vector3f(-1,  1, 1) );

    m_vertices[4] = Vertex( Vector3f(-1, -1, -1) );
    m_vertices[5] = Vertex( Vector3f( 1, -1, -1) );
    m_vertices[6] = Vertex( Vector3f( 1,  1, -1) ); 
    m_vertices[7] = Vertex( Vector3f(-1,  1, -1) );

    m_indices.resize( 36 );
    int indices[] = { 
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        3, 2, 6,
        6, 7, 3,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // left
        4, 0, 3,
        3, 7, 4,
        // right
        1, 5, 6,
        6, 2, 1
    };
    m_indices.assign( std::begin(indices), std::end(indices) );
    Model::Create( shader );
}
