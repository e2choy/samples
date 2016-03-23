#include "solution.h"
#include <glew.h>
#include <glut.h>
#include <algorithm>

///////////////////////////////////////////////////////////////////
void Solution::Solution_Draw(){
    solution.Draw();
}
void Solution::Solution_Size(int width, int height){
    solution.Size(width, height);
}
void Solution::Solution_Keyboard(unsigned char key, int x, int y){
    solution.Keyboard(key, x, y);
}
void Solution::Solution_SpecialKeyboard(int key, int x, int y){
    solution.SpecialKeyboard(key, x, y);
}
///////////////////////////////////////////////////////////////////
Solution::Solution() :
    m_shininess(5.0f),
    m_baseScale(40.0f),
    m_scale(1.0f,1.0f,1.0f){
}

void Solution::Create(){
    //initialize OpenGL
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutCreateWindow("Assignment 3 Solution");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glutDisplayFunc(Solution_Draw);
    glutIdleFunc(Solution_Draw);
    glutReshapeFunc(Solution_Size);
    glutKeyboardFunc(Solution_Keyboard);
    glutSpecialFunc(Solution_SpecialKeyboard);
    glewInit();

    //create the shader
    Matrix4f proj = Matrix4f::symmetricPerspectiveProjectionMatrix(45.0f, 2.0f, 1.0f, 1000.0f);
    m_phong.Create( "phong.vert", "phong.frag" );
    CopyMatrixToShader( proj, "projection" );
    m_gourand.Create( "gouraud.vert", "gouraud.frag" );
    CopyMatrixToShader( proj, "projection" );

    //set the camera
    m_camera.SetCamera(Vector3f(200, 200, 200), Vector3f(100, 10, 100), Vector3f(0, 1, 0));

    //create the light
    m_light.Create( 
        Vector3f(200.0f,210.0f,200.0f),
        Vector3f(1.0f,1.0f,1.0f),
        Vector3f(1.0f,1.0f,1.0f),
        Vector3f(1.0f,1.0f,1.0f));

    //create the material
    m_material.Create(Vector3f(0.8f,0.4f,0.2f),Vector3f(0.75f,0.75f,0.5f),Vector3f(0.8f,0.8f,0.8f),5);

    //create the model sphere
    m_sphere.Create( 10, 10, m_material );
    m_scSphere = m_scene.CreateChild(&m_sphere);
    m_scSphere->SetLocalScale(Vector3f(m_baseScale, m_baseScale, m_baseScale));
    m_scSphere->SetTransform(Matrix4f::translation(100, 10, 100));

    m_scene.Update();
}

void Solution::Draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update the light
    m_light.Use( m_camera );

    //draw the scene
    m_scene.Draw(m_camera);
    glutSwapBuffers();
}

void Solution::Size(int width, int height){
    glViewport(0, 0, width, height);
}

void Solution::Keyboard(unsigned char c, int x, int y){
    float delta = 1.0f;
    switch (c){
    case 'i':
        m_scale.x += 0.5f;
        m_scSphere->SetLocalScale(
            Vector3f(
            m_baseScale * m_scale.x, 
            m_baseScale * m_scale.y, 
            m_baseScale * m_scale.z ));
        break;
    case 'j':
        m_scale.x = std::max( 1.0f, m_scale.x - 0.5f );
        m_scSphere->SetLocalScale(
            Vector3f(
            m_baseScale * m_scale.x, 
            m_baseScale * m_scale.y, 
            m_baseScale * m_scale.z ));
        break;
    case 'k':
        m_scale.y += 0.5f;
        m_scSphere->SetLocalScale(
            Vector3f(
            m_baseScale * m_scale.x, 
            m_baseScale * m_scale.y, 
            m_baseScale * m_scale.z ));
        break;
    case 'l':
        m_scale.y = std::max( 1.0f, m_scale.y - 0.5f );
        m_scSphere->SetLocalScale(
            Vector3f(
            m_baseScale * m_scale.x, 
            m_baseScale * m_scale.y, 
            m_baseScale * m_scale.z ));
        break;
    case 'r':
        m_scale = Vector3f( 1.0f, 1.0f, 1.0f );    
        m_scSphere->SetLocalScale(
            Vector3f(
            m_baseScale * m_scale.x, 
            m_baseScale * m_scale.y, 
            m_baseScale * m_scale.z ));
        break;
    case 'a':
        m_camera.Roll(1.0f);
        break;
    case 'd':
        m_camera.Roll(-1.0f);
        break;
    case 'w':
        m_camera.Move(1.0f);
        break;
    case 's':
        m_camera.Move(-1.0f);
        break;
    case '-':
        m_shininess = std::max( 1.0f, m_shininess - 5.0f );
        m_material.SetShininess( m_shininess );
        break;
    case '+':
        m_shininess += 5.0f;
        m_material.SetShininess( m_shininess );
        break;
    case 'M':
        m_material.SetAmbient( Vector3f(0.0f,0.0f,0.0f) );
        break;
    case 'm':
        m_material.SetAmbient( Vector3f(0.8f,0.4f,0.2f) );
        break;
    case 'N':
        m_material.SetDiffuse( Vector3f(0.0f,0.0f,0.0f) );
        break;
    case 'n':
        m_material.SetDiffuse( Vector3f(0.75f,0.75f,0.5f) );
        break;
    case 'B':
        m_material.SetSpecular( Vector3f(0.0f,0.0f,0.0f) );
        break;
    case 'b':
        m_material.SetSpecular( Vector3f(0.8f,0.8f,0.8f) );
        break;
    case 'c':
        m_light.SetAmbient(Vector3f(0.0f,1.0f,0.5f)); 
        m_light.SetDiffuse(Vector3f(0.7f,0.0f,0.7f)); 
        m_light.SetSpecular(Vector3f(1.0f,0.0f,0.0f)); 
        break;
    case 'C':
        m_light.SetAmbient(Vector3f(1.0f,1.0f,1.0f)); 
        m_light.SetDiffuse(Vector3f(1.0f,1.0f,1.0f)); 
        m_light.SetSpecular(Vector3f(1.0f,1.0f,1.0f)); 
        break;
    case 'p':
    case 'P':
        //use phong
        m_phong.UseProgram();
        break;
    case 'g':
    case 'G':
        //use gouraud
        m_gourand.UseProgram();
        break;
    default:
        break;
    };
}

void Solution::SpecialKeyboard(int key, int x, int y){
    switch (key){
    case GLUT_KEY_UP:
        m_camera.Pitch(1.0f);
        break;
    case GLUT_KEY_DOWN:
        m_camera.Pitch(-1.0f);
        break;
    case GLUT_KEY_RIGHT:
        m_camera.Yaw(-1.0f);
        break;
    case GLUT_KEY_LEFT:
        m_camera.Yaw(1.0f);
        break;
    default:
        break;
    };
}