#version 410

layout (location=0) in vec3 v_position;
layout (location=1) in vec3 v_normal;
layout (location=2) in vec3 v_texture;

out vec3 fN;
out vec3 fE;
out vec3 objPos;
out vec2 texture;
out float mat_text;

struct PositionalLight
{   vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

uniform PositionalLight light[1];
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 norm_matrix;

void main(void)
{
    vec4 v = vec4(v_position, 1.0);
    
    fN = (norm_matrix * vec4(v_normal,1.0)).xyz;
    fE = (viewMatrix * v).xyz;
    objPos = (modelMatrix * v).xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * v;
    texture = v_texture.xy;
    mat_text = v_texture.z;
}
