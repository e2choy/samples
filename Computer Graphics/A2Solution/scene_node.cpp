#include "scene_node.h"
#include "shader.h"
#include "model.h"
#include "camera.h"

SceneNode::SceneNode( Model* model ):
    m_parent(nullptr),
    m_model(model),
    m_worldTr(Matrix4f::identity()), 
    m_tr(Matrix4f::identity()),
    m_scale(1.0f,1.0f,1.0f){
}

SceneNode::~SceneNode(){
    for( size_t i = 0; i < m_children.size(); ++i ){
        delete m_children[i];
    }
}

SceneNode* SceneNode::CreateChild( Model* model ){
    SceneNode* child = new SceneNode( model );
    m_children.push_back( child );
    child->m_parent = this;
    return child;
}

void SceneNode::Update(){
    if( m_parent ){
        m_worldTr = m_parent->m_worldTr * m_tr;
    }else{
        m_worldTr = m_tr;
    }
    for( size_t i = 0; i < m_children.size(); ++i ){
        m_children[i]->Update();
    }
}

void SceneNode::Draw( const Camera& camera, Shader& shader ){
    if( m_model ){
        Matrix4f tr = 
            camera.GetViewTransform() * 
            m_worldTr * 
            Matrix4f::scale( m_scale.x, m_scale.y, m_scale.z );
        CopyMatrixToShader( tr, "modelview", shader );
        m_model->Draw();
    }
    for( size_t i = 0; i < m_children.size(); ++i ){
        m_children[i]->Draw( camera, shader );
    }
}