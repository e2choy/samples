#version 330 core

uniform mat4 modelview; 
uniform mat4 projection;
uniform mat4 normtrans;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 normal;

out Data{
    vec4 position;
    vec4 norm;
} Out;

void main(){
    //change the position of the normal
    Out.position = modelview * vertex;
    Out.norm = normalize( normtrans * normal);
    gl_Position = projection * modelview * vertex;
}