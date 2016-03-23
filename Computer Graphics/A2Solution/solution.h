#ifndef _SOLUTION_
#define _SOLUTION_

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "scene_node.h"

class Solution{
public:
    static void Solution_Draw();
    static void Solution_Size( int width, int height );
    static void Solution_Keyboard( unsigned char key, int x, int y );
    static void Solution_SpecialKeyboard( int key, int x, int y );

    Solution();
    void Create();

    void Draw();
    void Size( int width, int height );
    void Keyboard( unsigned char c, int x, int y );
    void SpecialKeyboard( int key, int x, int y );
private:
    Shader      m_shader;
    Camera      m_camera;
    SceneNode	m_scene;

    Sphere		m_sphere;
    Cube		m_cube;

    //Task 1
    SceneNode*	m_scSphere;

    //Task 2
    Vector3f	m_locBox;
    float		m_rotSphere1;
    float		m_rotSphere2;
    SceneNode*	m_scBox;
    SceneNode*	m_scSphere1;
    SceneNode*	m_scSphere2;
};

extern Solution solution;
#endif