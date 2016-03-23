#version 330 core

layout(std140) uniform Material{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

layout(std140) uniform Light{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} light;

in Data{
    vec4 position;
    vec4 norm;
}In;

out vec4 color;

void main(){

    vec3 N = vec3(In.norm);
    vec3 V = vec3(In.position);

    //compute vector from vertex to light
    vec3 L = normalize( vec3(light.position) - V );
    vec3 R = reflect(-L,N);

    //compute vector from vertex to eye
    vec3 E = normalize(-V);

    //compute the ambient contribution
    vec3 ambient = vec3(light.ambient * material.ambient);

    //compute the diffuse contribution
    vec3 diffuse = vec3(light.diffuse * material.diffuse) * max(dot(L,N),0);

    //compute the specular contribution
    float specular_value = max(dot(R,E), 0.0);
    specular_value = pow(specular_value, material.shininess ); 
    vec3 specular = vec3(light.specular * material.specular) * specular_value;
    
    color = vec4(ambient + diffuse + specular,1);
}