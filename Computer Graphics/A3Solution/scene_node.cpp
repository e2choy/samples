#include "scene_node.h"
#include "shader.h"
#include "model.h"
#include "camera.h"

SceneNode::SceneNode(Model* model) :
    m_parent(nullptr),
    m_model(model),
    m_worldTr(Matrix4f::identity()),
    m_tr(Matrix4f::identity()),
    m_scale(1.0f, 1.0f, 1.0f){
}

SceneNode::~SceneNode(){
    for (size_t i = 0; i < m_children.size(); ++i){
        delete m_children[i];
    }
}

SceneNode* SceneNode::CreateChild(Model* model){
    SceneNode* child = new SceneNode(model);
    m_children.push_back(child);
    child->m_parent = this;
    return child;
}

void SceneNode::Update(){
    if (m_parent){
        m_worldTr = m_parent->m_worldTr * m_tr;
    }
    else{
        m_worldTr = m_tr;
    }
    for (size_t i = 0; i < m_children.size(); ++i){
        m_children[i]->Update();
    }
}

void SceneNode::Draw(const Camera& camera){
    if (m_model){

        //create the modelview transform
        Matrix4f mv = camera.GetViewTransform() * m_worldTr;
        Matrix4f tr = mv * Matrix4f::scale(m_scale.x, m_scale.y, m_scale.z);
        CopyMatrixToShader(tr, "modelview");

        //create the normal transform
        //we need the inverse transpose of the function
        //rather than writing an inverse function we take
        //advantage of the orthogonality of the modelview matrix
        //where the inverse is equivalent to the transpose
        CopyMatrixToShader(mv  * Matrix4f::scale( 1.0f/m_scale.x, 1.0f/m_scale.y, 1.0f/m_scale.z), "normtrans" );

        m_model->Draw();
    }
    for (size_t i = 0; i < m_children.size(); ++i){
        m_children[i]->Draw(camera);
    }
}