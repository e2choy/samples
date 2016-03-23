#include "solution.h"
#include <glew.h>
#include <glut.h>
///////////////////////////////////////////////////////////////////
void Solution::Solution_Draw(){
    solution.Draw();
}
void Solution::Solution_Size( int width, int height ){
    solution.Size( width, height );
}
void Solution::Solution_Keyboard( unsigned char key, int x, int y ){
    solution.Keyboard( key, x, y );
}
void Solution::Solution_SpecialKeyboard( int key, int x, int y ){
    solution.SpecialKeyboard( key, x, y );
}
///////////////////////////////////////////////////////////////////
Solution::Solution():m_rotSphere1(0.0f),m_rotSphere2(0.0f){}
void Solution::Create(){
    //initialize OpenGL
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 800, 400 );
    glutCreateWindow( "Assignment 2 Solution" );
    glClearColor( 1.0, 1.0, 1.0, 1.0);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glutDisplayFunc( Solution_Draw );
    glutIdleFunc( Solution_Draw );
    glutReshapeFunc( Solution_Size );
    glutKeyboardFunc( Solution_Keyboard );
    glutSpecialFunc( Solution_SpecialKeyboard );
    
    glewInit();

    m_shader.Create( "scene.vert", "scene.frag" );
    m_camera.SetCamera( Vector3f(150,30,200), Vector3f(150,10,100), Vector3f(0,1,0) );
    CopyMatrixToShader( 
        Matrix4f::symmetricPerspectiveProjectionMatrix( 45.0f, 2.0f, 1.0f, 1000.0f ),
        "projection",
        m_shader
        );
  
    m_sphere.Create( m_shader, 30, 30 );
    m_cube.Create( m_shader );

    //task 1
    m_scSphere = m_scene.CreateChild( &m_sphere );
    m_scSphere->SetLocalScale( Vector3f(10,10,10) );
    m_scSphere->SetTransform( Matrix4f::translation(100,10,100) );

    //task 2
    SceneNode* node = nullptr;
    m_locBox = Vector3f(150,10,100);	
    m_scBox = m_scene.CreateChild( &m_cube );
    m_scBox->SetLocalScale( Vector3f(30,10,10) );
    m_scBox->SetTransform( Matrix4f::translation(m_locBox.x,m_locBox.y,m_locBox.z));

    node = m_scBox->CreateChild();
    node->SetTransform( Matrix4f::translation( -20.0f, 20.0f, 0.0f ));
    m_scSphere1 = node->CreateChild( &m_sphere );
    m_scSphere1->SetLocalScale( Vector3f(10,10,10) );

    node = m_scBox->CreateChild();
    node->SetTransform( Matrix4f::translation( 20.0f, 20.0f, 0.0f ));
    m_scSphere2 = node->CreateChild( &m_sphere );
    m_scSphere2->SetLocalScale( Vector3f(10,10,10) );

    m_scene.Update();
}

void Solution::Draw(){
    //rotate the spheres
    m_rotSphere1 = std::fmodf( m_rotSphere1 + 0.1f, 360.0f );
    m_rotSphere2 = std::fmodf( m_rotSphere2 - 0.2f, 360.0f );

    m_scSphere1->SetTransform( Matrix4f::rotateY(m_rotSphere1));
    m_scSphere1->Update();
    m_scSphere2->SetTransform( Matrix4f::rotateY(m_rotSphere2));
    m_scSphere2->Update();

    //draw the scene
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    m_scene.Draw( m_camera, m_shader );
    glutSwapBuffers();
}

void Solution::Size( int width, int height ){
    glViewport( 0, 0, width, height );
}

void Solution::Keyboard( unsigned char c, int x, int y ){
    float delta = 1.0f;
    switch(c){
    case 'i':
        //translate box along x-axis x = x - delta
        m_locBox.x -= delta;
        m_scBox->SetTransform( Matrix4f::translation(m_locBox.x,m_locBox.y,m_locBox.z));
        m_scBox->Update();
        break;
    case 'l':
        //translate box along x-axis x = x + delta
        m_locBox.x += delta;
        m_scBox->SetTransform( Matrix4f::translation(m_locBox.x,m_locBox.y,m_locBox.z));
        m_scBox->Update();
        break;
    case 'j':
        //translate box along z-axis z = z - delta
        m_locBox.z -= delta;
        m_scBox->SetTransform( Matrix4f::translation(m_locBox.x,m_locBox.y,m_locBox.z));
        m_scBox->Update();
        break;
    case 'k':
        //translate box along z-axis z = z + delta
        m_locBox.z += delta;
        m_scBox->SetTransform( Matrix4f::translation(m_locBox.x,m_locBox.y,m_locBox.z));
        m_scBox->Update();
        break;
    case 'a':
        m_camera.Roll( 1.0f );
        break;
    case 'd':
        m_camera.Roll( -1.0f );
        break;
    case 'w':
        m_camera.Move( 1.0f );
        break;
    case 's':
        m_camera.Move( -1.0f );
        break;
    default:
        break;
    };
}

void Solution::SpecialKeyboard( int key, int x, int y ){
    switch(key){
    case GLUT_KEY_UP:
        m_camera.Pitch( 1.0f );
        break;
    case GLUT_KEY_DOWN:
        m_camera.Pitch( -1.0f );
        break;
    case GLUT_KEY_RIGHT:
        m_camera.Yaw( -1.0f );
        break;
    case GLUT_KEY_LEFT:
        m_camera.Yaw( 1.0f );
        break;
    default:
        break;
    };
}