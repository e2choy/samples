#ifndef _SCENE_NODE_
#define _SCENE_NODE_

#include <vector>
#include "nuss_vector.h"
#include "nuss_matrix.h"

class Camera;
class Model;
class SceneNode;
class Shader;
class SceneNode{
public:
    SceneNode( Model*  model = nullptr );
    ~SceneNode();

    void                SetTransform( const Matrix4f& mat ){ m_tr = mat; }
    const Matrix4f&     GetWorldTransform() const{ return m_worldTr; }

    const Vector3f&     GetLocalScale() const{ return m_scale; }
    void                SetLocalScale( const Vector3f& scale ){ m_scale = scale; }

    Model*              GetModel() { return m_model; }
    void                SetModel( Model* p ) { m_model = p; }

    SceneNode*		    CreateChild( Model* model = nullptr );

    void                Update();
    void                Draw( const Camera&, Shader& );
private:
    SceneNode*              m_parent;
    Model*			        m_model;
    Matrix4f                m_worldTr;
    Matrix4f                m_tr;
    Vector3f                m_scale;
    std::vector<SceneNode*> m_children;
};

#endif