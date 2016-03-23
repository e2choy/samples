#include <glew.h>
#include <cstddef>
#include <random>
#include <iterator>

#include "model.h"
#include "shader.h"
#include "material.h"
///////////////////////////////////////////////////////////////////////////////
Model::Model() : m_vao(-1), m_vbo(-1), m_idxVbo(-1){
}

void Model::Create( Material& material ){
    //create vertex array object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //create vertex buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    //copy the vertex position
    GLint vtxLoc = 0; 
    glEnableVertexAttribArray(vtxLoc);
    glVertexAttribPointer(vtxLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0 );

    //copy the vertex color
    GLint normLoc = 1; 
    glEnableVertexAttribArray(normLoc);
    glVertexAttribPointer(normLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(float) * 4) );

    //create index buffer
    glGenBuffers(1, &m_idxVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    //end creation
    glBindVertexArray(0);

    //create material
    m_material = &material;
}

void Model::Draw(){
    m_material->Use();

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::Create(
    int rings,
    int sectors,
    Material& material ){
    m_vertices.resize(rings * sectors);
    m_indices.resize((rings - 1) * (sectors - 1) * 6);

    //create the vertices
    float R = 1.0f / (rings - 1.0f);
    float S = 1.0f / (sectors - 1.0f);
    int i = 0;
    for (int r = 0; r < rings; ++r){
        for (int s = 0; s < sectors; ++s){
            float x = (float)(cos(2 * M_PI * s * S) * sin(M_PI * r * R));
            float y = (float) sin(-M_PI_2 + M_PI * r * R);
            float z = (float)(sin(2 * M_PI * s * S) * sin(M_PI * r * R));
            m_vertices[i] = Vertex(Vector3f(x, y, z), Vector3f(x, y, z));
            ++i;
        }
    }

    //create the indices
    i = 0;
    for (int r = 0; r < rings - 1; ++r){
        for (int s = 0; s < sectors - 1; ++s){
            m_indices[i++] = r * sectors + s;
            m_indices[i++] = (r + 1) * sectors + (s + 1);
            m_indices[i++] = r * sectors + (s + 1);

            m_indices[i++] = r * sectors + s;
            m_indices[i++] = (r + 1) * sectors + s;
            m_indices[i++] = (r + 1) * sectors + (s + 1);
        }
    }
    Model::Create( material );
}
///////////////////////////////////////////////////////////////////////////////
