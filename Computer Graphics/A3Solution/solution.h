#ifndef _SOLUTION_
#define _SOLUTION_

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "scene_node.h"
#include "material.h"
#include "light.h"
////////////////////////////////////////////////////////////
class Solution{
public:
    static void Solution_Draw();
    static void Solution_Size(int width, int height);
    static void Solution_Keyboard(unsigned char key, int x, int y);
    static void Solution_SpecialKeyboard(int key, int x, int y);

    Solution();
    void Create();

    void Draw();
    void Size(int width, int height);
    void Keyboard(unsigned char c, int x, int y);
    void SpecialKeyboard(int key, int x, int y);
private:
    Shader      m_gourand;
    Shader      m_phong;

    Camera      m_camera;
    SceneNode   m_scene;

    //Scene
    Material    m_material;
    Light       m_light;
    Sphere      m_sphere;

    //Controls
    float       m_shininess;
    float       m_baseScale;
    Vector3f    m_scale;

    SceneNode*  m_scSphere;
};

extern Solution solution;
#endif