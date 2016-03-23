#ifndef _MODEL_
#define _MODEL_
#include <glut.h>
#include <vector>

#include "nuss_vector.h"

///////////////////////////////////////////////////////////////////////////////
struct Vertex{
    Vertex(){}
    Vertex( const Vector3f pos, const Vector3f& norm ){
        float data[] = { 
            pos.x, pos.y, pos.z, 1.0f,
            norm.x, norm.y, norm.z, 0.0f
        };
        for( int i = 0; i < sizeof(data)/sizeof(float); ++i ){
            m_data[i] = data[i];
        }
    }
    
    float m_data[8];
};
typedef std::vector<Vertex>     Vertices;
typedef std::vector<GLuint>     Indices;
///////////////////////////////////////////////////////////////////////////////
class Shader;
class Material;
class Model{
public:
    Model();
    virtual ~Model(){};
    void Draw();
protected:
    void        Create( Material& material );
    Vertices    m_vertices;
    Indices     m_indices;
    GLuint      m_vao;            //vertex array object
    GLuint      m_vbo;            //vertex buffer object for vertices
    GLuint      m_idxVbo;         //vertex buffer object for indices
    GLuint      m_matVbo;         //material buffer object        
    Material*   m_material;
};
///////////////////////////////////////////////////////////////////////////////
class Sphere : public Model{
public:
    Sphere(){}
    void Create(
        int rings,                      //latitude
        int sectors,                    //longitute
        Material& material );                            
};

#endif
